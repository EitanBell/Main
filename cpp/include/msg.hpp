/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 10/07/23                    *
*	Subject: User Msg 		          *
**************************************/
#ifndef ILRD_RD136_7_MSG_HPP
#define ILRD_RD136_7_MSG_HPP

namespace ilrd
{

template <typename KEY, typename DATA>
class Msg
{
public:
    virtual ~Msg() = default;
    virtual KEY GetKey() const =0;
    virtual DATA GetData() const =0;
    virtual void operator()() =0;
};

enum Mode : int
{
    READ,
    WRITE,
    EXCEPT
};

} // namespace ilrd

#endif /* ILRD_RD136_7_MSG_HPP */
