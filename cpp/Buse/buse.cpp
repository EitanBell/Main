/*--------------------------------------------------------------------*/
/* Block-device USErspace extensions copyright (C) 2013 Adam Cozzette */
/*--------------------------------------------------------------------*/
/* BUSE (NBD COMMUNICATOR) program modified in 2023 by Noy Elankry    */
/*--------------------------------------------------------------------*/
/* DEVELOPER: Eitan Bellaiche 													              */
/* DATE MODIFIED: 14/07/2023												                  */
/* WORKSHEET SUBJECT: NBD COMMUNICATOR         								        */
/*--------------------------------------------------------------------*/

#define _POSIX_C_SOURCE (200809L)

#include <assert.h>       // assert
#include <errno.h>        // errno
#include <err.h>          // err
#include <fcntl.h>        // open
#include <linux/nbd.h>    // nbd request, nbd reply
#include <netinet/in.h>   // ntohl (byte order)
#include <signal.h>       // sigaction
#include <stdio.h>        // printf
#include <stdlib.h>       // malloc, free
#include <string.h>       // memcpy
#include <sys/ioctl.h>    // ioctl
#include <sys/socket.h>   // socketpair
#include <sys/stat.h>     // open
#include <sys/wait.h>     // waitpid
#include <unistd.h>       // read, write

#include <mutex>
#include <thread>

#include "buse.hpp"       // struct buseOperations

#ifndef BUSE_DEBUG
#define BUSE_DEBUG (0)
#endif

#ifdef WORDS_BIGENDIAN
u_int64_t ntohll(u_int64_t a)
{
  return a;
}
#else
u_int64_t ntohll(u_int64_t a)
{
  u_int32_t lo = a & 0xffffffff;
  u_int32_t hi = a >> 32U;
  lo = ntohl(lo);
  hi = ntohl(hi);
  return ((u_int64_t)lo) << 32U | hi;
}
#endif
#define htonll ntohll

static int nbdDeviceToDisconnect = -1;
/*--------------------------------------------------------------------*/
/*               HELPER FUNCTIONS FORWARD DECLARATIONS                */
/*--------------------------------------------------------------------*/
typedef const struct BuseOperations BuseOp_t;

static int ServeNbd(int socketFd, BuseOp_t *aop, void *userdata);
void ChildProcessHandleInterrupts(int nbd, int socketFd, int flags, int sp[]);
static int ReadAll(int fd, char *buf, size_t count);
static int WriteAll(int fd, char *buf, size_t count);
void SetNbdParams(int nbd, BuseOp_t *aop);
static void DisconnectNbd(int signal);
static int SetSigaction(int sig, const struct sigaction *act);
void ReadCommand(u_int64_t from, u_int32_t len, int socketFd, 
    struct nbd_reply *reply, BuseOp_t *aop, void *userdata);
void WriteCommand(u_int64_t from, u_int32_t len, int socketFd, 
    struct nbd_reply *reply, BuseOp_t *aop, void *userdata);
static void ReqHandler(int socketFd, nbd_request req, BuseOp_t *aop, void *userdata);

/*--------------------------------------------------------------------*/
/*                              BUSE MAIN                             */
/*--------------------------------------------------------------------*/
int BuseMain(const char *deviceFile, BuseOp_t *aop, void *userdata)
{
  int sp[2];
  int nbd = 0;
  int socketFd = 0; 
  int err = 0;
  int flags = 0;

  err = socketpair(AF_UNIX, SOCK_STREAM, 0, sp);
  assert(!err);

  nbd = open(deviceFile, O_RDWR);
  if (nbd == -1)
  {
    fprintf(stderr,
            "Failed to open `%s': %s\n"
            "Is kernel module `nbd' loaded and you have permissions "
            "to access the device?\n",
            deviceFile, strerror(errno));
    return 1;
  }

  SetNbdParams(nbd, aop);

  pid_t pid = fork();
  if (pid == 0)
  {
    ChildProcessHandleInterrupts(nbd, socketFd, flags, sp);
  }

  assert(nbdDeviceToDisconnect == -1);

  nbdDeviceToDisconnect = nbd;
  struct sigaction act;
  act.sa_handler = DisconnectNbd;
  act.sa_flags = SA_RESTART;

  if (sigemptyset(&act.sa_mask) != 0 ||
      sigaddset(&act.sa_mask, SIGINT) != 0 ||
      sigaddset(&act.sa_mask, SIGTERM) != 0)
  {
    warn("failed to prepare signal mask in parent");
    return EXIT_FAILURE;
  }

  if (SetSigaction(SIGINT, &act) != 0 ||
      SetSigaction(SIGTERM, &act) != 0)
  {
    warn("failed to register signal handlers in parent");
    return EXIT_FAILURE;
  }

  close(sp[1]);

  int status = 0;

  status = ServeNbd(sp[0], aop, userdata);

  if (close(sp[0]))
  {
    warn("problem closing server side nbd socket");
  }

  if (status)
  {  
    return status;
  }

  if (waitpid(pid, &status, 0) == -1)
  {
    warn("waitpid failed");
    return EXIT_FAILURE;
  }

  if (WEXITSTATUS(status))
  {
    return WEXITSTATUS(status);
  }

  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------*/
/*                              NBD SERVER                            */
/*--------------------------------------------------------------------*/
static int ServeNbd(int socketFd, BuseOp_t *aop, void *userdata)
{
  ssize_t bytes_read;
  struct nbd_request request;

  while ((bytes_read = read(socketFd, &request, sizeof(request))) > 0)
  {
    assert(bytes_read == sizeof(request));

    if (request.type != NBD_CMD_DISC)
    {
      std::thread(&ReqHandler, socketFd, request, aop, userdata).detach();
    }
    else
    {
      fprintf(stderr, "Got NBD_CMD_DISC\n");
      if (aop->Disconnect)
      {
        aop->Disconnect(userdata);
      }

      return EXIT_SUCCESS;
    }
  }

  if (bytes_read == -1)
  {
    warn("error reading userside of nbd socket");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void ReqHandler(int socketFd, nbd_request req, BuseOp_t *aop, void *userdata)
{
    nbd_reply reply;

    reply.magic = htonl(NBD_REPLY_MAGIC);
    reply.error = htonl(0);

    memcpy(reply.handle, req.handle, sizeof(reply.handle));
    reply.error = htonl(0);

    u_int64_t len = ntohl(req.len);
    u_int64_t from = ntohll(req.from);

    assert(req.magic == htonl(NBD_REQUEST_MAGIC));

    switch (ntohl(req.type))
    {
      case NBD_CMD_READ:
      {
        ReadCommand(from, len, socketFd, &reply, aop, userdata);
        break;
      }
      case NBD_CMD_WRITE:
      {
        WriteCommand(from, len, socketFd, &reply, aop, userdata);
        break;
      }
    }
}

/*--------------------------------------------------------------------*/
/*                          HELPER FUNCTIONS                          */
/*--------------------------------------------------------------------*/
void ChildProcessHandleInterrupts(int nbd, int socketFd, int flags, int sp[])
{
  sigset_t sigset;
  int err = 0;

  if (sigfillset(&sigset) != 0 ||
      sigprocmask(SIG_SETMASK, &sigset, NULL) != 0)
  {
    warn("failed to block signals in child");
    exit(EXIT_FAILURE);
  }

  close(sp[0]);
  socketFd = sp[1];

  if (ioctl(nbd, NBD_SET_SOCK, socketFd) == -1)
  {
    fprintf(stderr, "ioctl(nbd, NBD_SET_SOCK, socketFd) failed.[%s]\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  else
  {
#if defined NBD_SET_FLAGS
    flags = 0;
#if defined NBD_FLAG_SEND_TRIM
    flags |= NBD_FLAG_SEND_TRIM;
#endif
#if defined NBD_FLAG_SEND_FLUSH
    flags |= NBD_FLAG_SEND_FLUSH;
#endif
    if (flags != 0 && ioctl(nbd, NBD_SET_FLAGS, flags) == -1)
    {
      fprintf(stderr, "ioctl(nbd, NBD_SET_FLAGS, %d) failed.[%s]\n", flags, strerror(errno));
      exit(EXIT_FAILURE);
    }
#endif

    err = ioctl(nbd, NBD_DO_IT);
    fprintf(stderr, "nbd device terminated with code %d\n", err);

    if (err == -1)
    {
      warn("NBD_DO_IT terminated with error");
      exit(EXIT_FAILURE);
    }
  }

  if (ioctl(nbd, NBD_CLEAR_QUE) == -1 ||
      ioctl(nbd, NBD_CLEAR_SOCK) == -1)
  {
    warn("failed to perform nbd cleanup actions");
    exit(EXIT_FAILURE);
  }

  exit(0);
}

static int ReadAll(int fd, char *buf, size_t count)
{
  int bytes_read;

  while (count > 0)
  {
    bytes_read = read(fd, buf, count);

    assert(bytes_read > 0);

    buf += bytes_read;
    count -= bytes_read;
  }

  assert(!count);

  return 0;
}

static int WriteAll(int fd, char *buf, size_t count)
{
  int bytes_written;

  while (count > 0)
  {
    bytes_written = write(fd, buf, count);

    assert(bytes_written > 0);

    buf += bytes_written;
    count -= bytes_written;
  }

  assert(!count);

  return 0;
}

void SetNbdParams(int nbd, BuseOp_t *aop)
{
  int err = 0;

  if (aop->m_blkSize)
  {
    err = ioctl(nbd, NBD_SET_BLKSIZE, aop->m_blkSize);
    assert(err != -1);
  }

  if (aop->m_size)
  {
    err = ioctl(nbd, NBD_SET_SIZE, aop->m_size);
    assert(err != -1);
  }

  if (aop->m_sizeBlocks)
  {
    err = ioctl(nbd, NBD_SET_SIZE_BLOCKS, aop->m_sizeBlocks);
    assert(err != -1);
  }

  err = ioctl(nbd, NBD_CLEAR_SOCK);
  assert(err != -1);
}

std::mutex g_mutex;

void ReadCommand(u_int64_t from, u_int32_t len, int socketFd, nbd_reply *reply,
                BuseOp_t *aop, void *userdata)
{
  void *chunk = NULL;

  fprintf(stderr, "Request for read of size %d\n", len);
  
  chunk = malloc(len);
  if (aop->Read)
  {
    reply->error = aop->Read(chunk, len, from, userdata);
  }
  else
  {
    reply->error = htonl(EPERM);
  }

  g_mutex.lock();
  WriteAll(socketFd, (char *)reply, sizeof(nbd_reply));
  WriteAll(socketFd, (char *)chunk, len);
  g_mutex.unlock();

  free(chunk);
  chunk = NULL;
}

void WriteCommand(u_int64_t from, u_int32_t len, int socketFd, nbd_reply *reply,
                BuseOp_t *aop, void *userdata)
{
  char *chunk = NULL;

  fprintf(stderr, "Request for write of size %d\n", len);
  
  chunk = static_cast<char *>(malloc(len));
  ReadAll(socketFd, chunk, len);

  if (aop->Write)
  {
    reply->error = aop->Write(chunk, len, from, userdata);
  }
  else
  {
    reply->error = htonl(EPERM);
  }

  free(chunk);
  chunk = NULL;

  g_mutex.lock();
  WriteAll(socketFd, (char *)reply, sizeof(nbd_reply));
  g_mutex.unlock();

}
/*--------------------------------------------------------------------*/
/*                           SIGNAL HANDLERS                          */
/*--------------------------------------------------------------------*/
static void DisconnectNbd(int signal)
{
  (void)signal;

  if (nbdDeviceToDisconnect != -1)
  {
    if (ioctl(nbdDeviceToDisconnect, NBD_DISCONNECT) == -1)
    {
      warn("failed to request disconect on nbd device");
    }
    else
    {
      nbdDeviceToDisconnect = -1;
      fprintf(stderr, "sucessfuly requested disconnect on nbd device\n");
    }
  }
}

static int SetSigaction(int sig, const struct sigaction *act)
{
  struct sigaction oact;
  int r = sigaction(sig, act, &oact);

  if (r == 0 && oact.sa_handler != SIG_DFL)
  {
    warnx("overriden non-default signal handler (%d: %s)", sig, strsignal(sig));
  }

  return r;
}