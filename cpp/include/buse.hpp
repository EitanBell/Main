 /**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Date: 14/07/23	                  *
*	Subject: BUSE                 	  *
*				                	  *
**************************************/

#ifndef BUSE_H_INCLUDED
#define BUSE_H_INCLUDED

#ifdef __cplusplus
extern "C" 
{
#endif

#include <sys/types.h>

  struct BuseOperations 
  {
    int (*Read)(void *buf, u_int32_t len, u_int64_t offset, void *userdata);
    int (*Write)(const void *buf, u_int32_t len, u_int64_t offset, void *userdata);
    void (*Disconnect)(void *userdata);
    int (*Flush)(void *userdata);
    int (*Trim)(u_int64_t from, u_int32_t len, void *userdata);

    // either set size, OR set both blksize and size_blocks
    u_int64_t m_size;
    u_int32_t m_blkSize;
    u_int64_t m_sizeBlocks;
  };

  int BuseMain(const char* deviceFile, const struct BuseOperations *bop, void *userdata);

#ifdef __cplusplus
}
#endif

#endif /* BUSE_H_INCLUDED */
