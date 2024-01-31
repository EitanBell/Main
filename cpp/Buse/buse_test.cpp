/*--------------------------------------------------------------------------*/
/* DEVELOPER: NOY ELANKRY 													                        */
/* DATE MODIFIED: 10/07/2023											                        	*/
/* WORKSHEET SUBJECT: NBD COMMUNICATOR         			              					*/
/*--------------------------------------------------------------------------*/
/*
 * busexmp - example memory-based block device using BUSE
 * Copyright (C) 2013 Adam Cozzette
 */

#include <argp.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buse.hpp"

/* BUSE callbacks */
static void *data;

static int xmp_read(void *buf, u_int32_t len, u_int64_t offset, void *userdata)
{
  if (*(int *)userdata)
  {
    fprintf(stderr, "R - %lu, %u\n", offset, len);
  }

  memcpy(buf, (char *)data + offset, len);
  return 0;
}

static int xmp_write(const void *buf, u_int32_t len, u_int64_t offset, void *userdata)
{
  if (*(int *)userdata)
  {
    fprintf(stderr, "W - %lu, %u\n", offset, len);
  }

  memcpy((char *)data + offset, buf, len);
  return 0;
}

static void xmp_disc(void *userdata)
{
  if (*(int *)userdata)
  {
    fprintf(stderr, "Received a disconnect request.\n");
  }
}

static int xmp_flush(void *userdata)
{
  if (*(int *)userdata)
  {
    fprintf(stderr, "Received a flush request.\n");
  }

  return 0;
}

static int xmp_trim(u_int64_t from, u_int32_t len, void *userdata)
{
  if (*(int *)userdata)
  {
    fprintf(stderr, "T - %lu, %u\n", from, len);
  }

  return 0;
}

/* argument parsing using argp */

static struct argp_option options[] = 
{
    {"verbose", 'v', 0, 0, "Produce verbose output", 0},
    {0},
};

struct arguments
{
  unsigned long long size;
  char *device;
  int verbose;
};

static unsigned long long strtoullWithPrefix(const char *str, char **end)
{
  unsigned long long v = strtoull(str, end, 0);
  switch (**end)
  {
  case 'K':
    v *= 1024;
    *end += 1;
    break;
  case 'M':
    v *= 1024 * 1024;
    *end += 1;
    break;
  case 'G':
    v *= 1024 * 1024 * 1024;
    *end += 1;
    break;
  }
  return v;
}

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = static_cast<struct arguments*>(state->input);
  char *endptr;

  switch (key)
  {

  case 'v':
    arguments->verbose = 1;
    break;

  case ARGP_KEY_ARG:
    switch (state->arg_num)
    {

    case 0:
      arguments->size = strtoullWithPrefix(arg, &endptr);
      if (*endptr != '\0')
      {
        errx(EXIT_FAILURE, "SIZE must be an integer");
      }
      break;

    case 1:
      arguments->device = arg;
      break;

    default:
      /* Too many arguments. */
      return ARGP_ERR_UNKNOWN;
    }
    break;

  case ARGP_KEY_END:
    if (state->arg_num < 2)
    {
      warnx("not enough arguments");
      argp_usage(state);
    }
    break;

  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static struct argp argp = 
{
    .options = options,
    .parser = parse_opt,
    .args_doc = "SIZE DEVICE",
    .doc = "BUSE virtual block device that stores its content in memory.\n"
           "`SIZE` accepts suffixes K, M, G. `DEVICE` is path to block device, for example \"/dev/nbd0\".",
};

int main(int argc, char *argv[])
{
  struct arguments arguments = 
  {
      .verbose = 0,
  };
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  struct BuseOperations aop = 
  {
      .Read = xmp_read,
      .Write = xmp_write,
      .Disconnect = xmp_disc,
      .Flush = xmp_flush,
      .Trim = xmp_trim,
      .m_size = arguments.size,
  };

  data = malloc(aop.m_size);
  if (data == NULL)
  {
    err(EXIT_FAILURE, "failed to alloc space for data");
  }

  return BuseMain(arguments.device, &aop, (void *)&arguments.verbose);
}
