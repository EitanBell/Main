/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 14/05/23	                  *
*	Subject: BitArray_phase_2	      *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_BITARRAY_PHASE2_HPP
#define ILRD_RD136_7_BITARRAY_PHASE2_HPP

#include <string>       //std::transform
#include <algorithm>    //equal, fill

namespace ilrd
{

const static int NUM_BITS_IN_BYTES = 8;
const static int WORDSIZE = (sizeof(std::size_t) * NUM_BITS_IN_BYTES);

template <std::size_t SIZE>
class BitArray
{
class BitProxy;

public:
    explicit BitArray();						
	
    bool operator[](std::size_t index) const;
    BitProxy operator[](std::size_t index);
	
    bool operator==(const BitArray& other) const;
    bool operator!=(const BitArray& other) const;
    BitArray& operator|=(const BitArray& other);
    BitArray& operator&=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);
    BitArray& operator>>=(std::size_t n);
    BitArray& operator<<=(std::size_t n);
	
    bool Get(std::size_t index) const;
    BitArray& Set();
    BitArray& Set(std::size_t index, bool val = true);
    BitArray& Flip();
    BitArray& Flip(std::size_t index);
    std::size_t Count() const;
    std::string ToString() const;

private:
	static const std::size_t CountSetBitsLUT[256];

    class BitProxy
    {
    public:
        explicit BitProxy(std::size_t& bitarr, std::size_t index);

        operator bool() const; 
        BitProxy& operator=(const BitProxy& other); // b[i] = b[j]
        BitProxy& operator=(bool bit);              // b[i] = true
        
    private:
        std::size_t& m_bitarr;
        std::size_t m_index;
    };

	static const std::size_t ARR_SIZE = (((SIZE + (WORDSIZE - 1UL)) & 
												 ~(WORDSIZE - 1UL)) / WORDSIZE);
	std::size_t m_bitarr[ARR_SIZE];
	
	static std::size_t SetOffExtraBits(std::size_t bitarr);
	static void MoveBits(std::size_t *bitarr, std::size_t n);
	void CheckOutOfRange(std::size_t index) const;

	/*------------------------------ Functors --------------------------------*/
	class FunctorOr
	{
	public:
		std::size_t operator()(std::size_t num1, std::size_t num2);
	};

	class FunctorAnd
	{
	public:
		std::size_t operator()(std::size_t num1, std::size_t num2);
	};

	class FunctorXor
	{
	public:
		std::size_t operator()(std::size_t num1, std::size_t num2);
	};

	class FunctorFlip
	{
	public:
		std::size_t operator()(std::size_t num);
	};

	class FunctorCount
	{
	public:
		FunctorCount();
		void operator()(const unsigned char& ch);
		std::size_t m_count;
	};

	class FunctorLeftShift
    {
    public:
        FunctorLeftShift(std::size_t num);
        void operator()(std::size_t& num);
    private:
        int m_nShifts;
        std::size_t m_restOfBits;
    };

	class FunctorRightShift
    {
    public:
        FunctorRightShift(std::size_t num);
        void operator()(std::size_t& num);
    private:
        int m_nShifts;
        std::size_t m_restOfBits;
    };
};

template<std::size_t SIZE>
const std::size_t BitArray<SIZE>::CountSetBitsLUT[256] = {0, 1, 1, 2, 1, 2, 2, 
3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 
2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 
5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 
3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 
5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 
4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 
4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 
3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 
6, 7, 6, 7, 7, 8}; 

/*------------------------- operators implementation -------------------------*/

template<std::size_t SIZE>
BitArray<SIZE>::BitProxy::BitProxy(std::size_t& bitarr, std::size_t index) : 
											 m_bitarr(bitarr), m_index(index) {}

template <std::size_t SIZE>
BitArray<SIZE>::BitProxy::operator bool() const
{
    return (m_bitarr & (1UL << m_index));
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=
										                 (const BitProxy& other) // b[i] = b[j]
{
   *this = (1UL & (other.m_bitarray >> m_index));

   return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitProxy& BitArray<SIZE>::BitProxy::operator=(bool bit) // b[i] = TRUE
{
    m_bitarr = ((m_bitarr & ~(1UL << m_index)) | 
                (static_cast<size_t>(bit) << m_index));

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>::BitArray()
{
	std::fill(m_bitarr, m_bitarr + ARR_SIZE, 0);
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator[](std::size_t index) const
{
	CheckOutOfRange(index);

    std::size_t word_block = index / WORDSIZE;
	std::size_t idx_in_block = index & (WORDSIZE - 1UL);
    return (1UL & (m_bitarr[word_block] >> idx_in_block));
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitProxy BitArray<SIZE>::operator[](std::size_t index)
{
	CheckOutOfRange(index);

    std::size_t word_block = index / WORDSIZE;
	std::size_t idx_in_block = index & (WORDSIZE - 1UL);
    return (BitProxy(m_bitarr[word_block], idx_in_block));
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray& other) const
{
    return std::equal(m_bitarr, m_bitarr + ARR_SIZE, other.m_bitarr);
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray& other) const
{
	return (!(*this == other));
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::operator|=
														 (const BitArray& other)
{
	std::transform(m_bitarr, m_bitarr + ARR_SIZE, other.m_bitarr,
				                                         m_bitarr, FunctorOr());
	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::operator&=
														 (const BitArray& other)
{
	std::transform(m_bitarr, m_bitarr + ARR_SIZE, other.m_bitarr,
				                                        m_bitarr, FunctorAnd());
	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::operator^=
														 (const BitArray& other)
{
	std::transform(m_bitarr, m_bitarr + ARR_SIZE, other.m_bitarr,
				                                        m_bitarr, FunctorXor());
	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::operator<<=(std::size_t num)
{
	if (WORDSIZE <= num)
	{
		MoveBits(m_bitarr ,num);
	}

	std::for_each(m_bitarr, m_bitarr + ARR_SIZE,
				  					    FunctorLeftShift(num & (WORDSIZE - 1)));
	m_bitarr[ARR_SIZE - 1] = SetOffExtraBits(m_bitarr[ARR_SIZE - 1]);

	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::operator>>=(std::size_t num)
{
	std::reverse(m_bitarr, m_bitarr + ARR_SIZE);
	
	if (WORDSIZE <= num)
	{
		MoveBits(m_bitarr ,num);
	}

	std::for_each(m_bitarr, m_bitarr + ARR_SIZE,
				  					   FunctorRightShift(num & (WORDSIZE - 1)));
	
	std::reverse(m_bitarr, m_bitarr + ARR_SIZE);


	return *this;
}

/*--------------------------- Methods implementation -------------------------*/

template <std::size_t SIZE>
bool BitArray<SIZE>::Get(std::size_t index) const
{
	return ((*this)[index]);
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::Set()
{
	std::fill(m_bitarr, m_bitarr + ARR_SIZE, ~0UL);

	m_bitarr[ARR_SIZE - 1] = SetOffExtraBits(m_bitarr[ARR_SIZE - 1]);
	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::Set(std::size_t index,
                                                                       bool val)
{
	CheckOutOfRange(index);

	(*this)[index] = static_cast<std::size_t>(val);

	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::Flip()
{
	std::transform(m_bitarr, m_bitarr + ARR_SIZE, m_bitarr, FunctorFlip());

	m_bitarr[ARR_SIZE - 1] = SetOffExtraBits(m_bitarr[ARR_SIZE - 1]);
	return *this;
}

template <std::size_t SIZE>
typename BitArray<SIZE>::BitArray& BitArray<SIZE>::Flip(std::size_t index)
{
	(*this)[index] = ((*this)[index]) ^ 1UL;

	return *this;
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::Count() const
{
	const unsigned char *arrAsChars =
							  reinterpret_cast<const unsigned char *>(m_bitarr);
	
	return (std::for_each(arrAsChars, arrAsChars + (ARR_SIZE * sizeof(size_t)),
													   FunctorCount()).m_count);
}

template <std::size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    std::string str;
    
    for (int i = static_cast<int>(SIZE - 1); 0 <= i; --i)
    {
        str += '0' + (*this)[i];
    }

    return str;
}

/*-------------------------- Functors implementation -------------------------*/

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorOr::operator()(size_t num1, size_t num2)
{
	return (num1 | num2);
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorAnd::operator()(size_t num1, size_t num2)
{
	return (num1 & num2);
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorXor::operator()(size_t num1, size_t num2)
{
	return (num1 ^ num2);
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorFlip::operator()(std::size_t num)
{
	return ~num;
}

template <std::size_t SIZE>
BitArray<SIZE>::FunctorCount::FunctorCount(): m_count(0) {}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorCount::operator()(const unsigned char& ch)
{
    m_count += CountSetBitsLUT[ch];
}

template <std::size_t SIZE>
BitArray<SIZE>::FunctorLeftShift::FunctorLeftShift(std::size_t num): 
											  m_nShifts(num), m_restOfBits(0) {}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorLeftShift::operator()(std::size_t& num)
{
	std::size_t tmp = num >> (WORDSIZE - m_nShifts);
	num = ((num << m_nShifts) | (m_restOfBits));
	m_restOfBits = tmp;
}

template <std::size_t SIZE>
BitArray<SIZE>::FunctorRightShift::FunctorRightShift(std::size_t num): 
											  m_nShifts(num), m_restOfBits(0) {}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorRightShift::operator()(std::size_t& num)
{
	std::size_t tmp = num << (WORDSIZE - m_nShifts);
	num = ((num >> m_nShifts) | (m_restOfBits));
	m_restOfBits = tmp;
}

/*------------------------------ Helper Functions ----------------------------*/

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::SetOffExtraBits(std::size_t bitarr)
{
	std::size_t BitMask = (1UL << (SIZE % WORDSIZE)) - 1;

	if (0 != BitMask)
	{
		bitarr &= BitMask;
	}

	return bitarr;
}

template <std::size_t SIZE>
void BitArray<SIZE>::MoveBits(std::size_t *bitarr, std::size_t n)
{
	std::move(bitarr, bitarr + ARR_SIZE - (n / WORDSIZE),
			                                           bitarr + (n / WORDSIZE));

	std::fill(bitarr, bitarr + (n / WORDSIZE), 0UL);
}

template <std::size_t SIZE>
void BitArray<SIZE>::CheckOutOfRange(std::size_t index) const
{
	if (index >= SIZE)
	{
		throw std::out_of_range ("Index out of range");
	}
}

} // namespace ilrd;

#endif /* ILRD_RD136_7_BITARRAY_PHASE2_HPP */