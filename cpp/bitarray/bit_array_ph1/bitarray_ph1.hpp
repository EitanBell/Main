/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 14/05/23	                  *
*	Subject: BitArray			      *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_BITARRAY_HPP
#define ILRD_RD136_7_BITARRAY_HPP

namespace ilrd
{
class BitArray
{
class BitProxy;

public:
    explicit BitArray(std::size_t val = 0);

    bool operator[](std::size_t index) const;
    BitProxy operator[](std::size_t index);

private:
    size_t m_bitarr;
    
    class BitProxy
    {
    public:
        explicit BitProxy(std::size_t& bitarr, std::size_t index);
	
		operator bool() const;
        BitProxy& operator=(const BitProxy& other); // b[i] = b[j]
        BitProxy& operator=(bool bit); // b[i] = TRUE
        
    private:
        std::size_t& m_bitarray;
        std::size_t m_index;
    };
};

/*-------------------------------- BitProxy ----------------------------------*/

BitArray::BitProxy::BitProxy(std::size_t& bitarr, std::size_t index):
                                           m_bitarray(bitarr), m_index(index) {}

BitArray::BitProxy& BitArray::BitProxy::operator=(const BitProxy& other) // b[i] = b[j]
{
    std::size_t bit_val = 1UL & (other.m_bitarray >> other.m_index);
    std::size_t mask = 1UL << m_index;

    m_bitarray = (m_bitarray & ~mask) | (bit_val << m_index);

    return *this;
}

BitArray::BitProxy& BitArray::BitProxy::operator=(bool bit) // b[i] = TRUE
{
    std::size_t bit_val = bit;
    std::size_t mask = 1UL << m_index;

    m_bitarray = (m_bitarray & ~mask) | (bit_val << m_index);

    return *this;
}

BitArray::BitProxy::operator bool() const
{
    return (m_bitarray & (1UL << m_index));
}

/*-------------------------------- BitArray ----------------------------------*/

BitArray::BitArray(std::size_t bitarrVal): m_bitarr(bitarrVal) {}

bool BitArray::operator[](std::size_t index) const
{
    return (1 & (m_bitarr >> index));
}

BitArray::BitProxy BitArray::operator[](std::size_t index)
{
    return (BitProxy(m_bitarr, index));
}

} // ilrd

#endif	// ILRD_RD136_7_BITARRAY_HPP

