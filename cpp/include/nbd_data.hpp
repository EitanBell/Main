/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 19/07/23                    *
*	Subject: NBD_data                 *
**************************************/
#ifndef ILRD_RD136_7_NBD_DATA_HPP
#define ILRD_RD136_7_NBD_DATA_HPP

namespace ilrd
{

struct NBD_data
{
    int m_socketFd;
    nbd_request m_req;
    nbd_reply m_rep;
    void *m_nbdCom;
};


}   // ilrd namespace

#endif /* ILRD_RD136_7_NBD_DATA_HPP */
