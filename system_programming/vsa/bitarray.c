/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad    	     *
*	Date: 01/12/22	             *
*	Valgrind & GDB passed        *
*	Subject: Bit_array	     *
*				     *
**************************************/

#include <stdio.h> /* sprintf */
#include <limits.h> /* CHAR_BIT */
#include <assert.h> /* assert */

#include "bitarray.h" /* my_array */

#define SIZE_OF_BITARR sizeof(size_t) * CHAR_BIT
#define RANGE_OF_CHAR 256		

static char bits_lut[RANGE_OF_CHAR] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 
				3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4,
				4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 
				4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 
				4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 
				6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 
				4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 
				6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 
				5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 
				6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 
				6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7,
				7, 8};

static unsigned char mirror_lut[RANGE_OF_CHAR] = {0, 128, 64, 192, 32, 160, 96, 
224, 16, 144, 80, 208, 48, 176, 112, 240, 8, 136, 72, 200, 40, 168, 104, 232, 24, 
152, 88, 216, 56, 184, 120, 248, 4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 
212, 52, 180, 116, 244, 12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220,
60, 188, 124, 252, 2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178,
114, 242, 10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 
250, 6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246, 
14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254, 1, 
129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241, 9, 137, 
73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249, 5, 133, 69, 
197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245, 13, 141, 77, 205, 
45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253, 3, 131, 67, 195, 35, 
163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243, 11, 139, 75, 203, 43, 171, 
107, 235, 27, 155, 91, 219, 59, 187, 123, 251, 7, 135, 71, 199, 39, 167, 103, 
231, 23, 151, 87, 215, 55, 183, 119, 247, 15, 143, 79, 207, 47, 175, 111, 239, 
31, 159, 95, 223, 63, 191, 127, 255};


bit_array_t BitArrSetOn(bit_array_t bitarr, size_t i)
{
	return (bitarr | (size_t)bit_on << i);
}

bit_array_t BitArrSetOff(bit_array_t bitarr, size_t i)
{
	size_t mask_off = ~0;
	mask_off = mask_off ^ ((size_t)bit_on << i);
	
	return bitarr & mask_off;
}

bit_array_t BitArrSetAll(bit_array_t bitarr)
{			
	return ~BitArrUnsetAll(bitarr); 
}


bit_array_t BitArrUnsetAll(bit_array_t bitarr)
{	
	return bitarr & 0;
}

bit_array_t BitArrFlipBit(bit_array_t bitarr, size_t i)
{
	size_t mask_flip = (size_t)bit_on << i;
	
	return (bitarr ^ mask_flip); 
}

int BitArrGetVal(bit_array_t bitarr, size_t i)
{
	return (bitarr >> (i) & (size_t)bit_on);
}

bit_array_t BitArrSetVal(bit_array_t bitarr, size_t i, bit_val_t value)
{
	
	return (bitarr = (value ? BitArrSetOn(bitarr, i):
			          BitArrSetOff(bitarr, i)));
}

size_t BitArrCountSetBitsHamming(bit_array_t bitarr)
{
	bitarr = bitarr - ((bitarr >> 1) & 0x5555555555555555);
	bitarr = (bitarr & 0x3333333333333333) + 
		((bitarr >> 2) & 0x3333333333333333);
	bitarr = (bitarr + (bitarr >> 4)) & 0x0F0F0F0F0F0F0F0F;
	bitarr = bitarr + (bitarr >> 8);
	bitarr = bitarr + (bitarr >> 16);
	bitarr = bitarr + (bitarr >> 32);
	
	return bitarr & 0x000000000000003F;
}


size_t BitArrCountSetBits(bit_array_t bitarr)
{
	int count = 0;
	
	for (count = 0; 0 != bitarr; ++count)
	{
		bitarr = bitarr & (bitarr - 1);
	}
	
	return count;
}



bit_array_t BitArrFlipAll(bit_array_t bitarr)
{
	return ~bitarr;
}


bit_array_t BitArrRotateLeft(bit_array_t bitarr, size_t n)
{	
	 return ((bitarr << n) ^ (bitarr >> (SIZE_OF_BITARR - n)));
}

bit_array_t BitArrRotateRight(bit_array_t bitarr, size_t n)
{	
	 return ((bitarr >> n) ^ (bitarr << (SIZE_OF_BITARR - n)));
}


char *BitArrToString(bit_array_t bitarr, char *buffer)
{
	size_t i = 0;
	
	assert(NULL != buffer);
	
	for (i = 0; SIZE_OF_BITARR > i; ++i)
	{
		sprintf((buffer + i), "%d", BitArrGetVal(bitarr, i));
	}
	
	return buffer;
}


bit_array_t BitArrMirror(bit_array_t bitarr)
{
	size_t mirror = 0;
	size_t i = 0;
	
	for (i = 0; SIZE_OF_BITARR > i; ++i)
	{
		mirror <<= 1;
		mirror = mirror | (1 & bitarr);
		bitarr >>= 1;
	}
	
	return mirror;
}
size_t BitArrCountOffBits(bit_array_t bitarr)
{
	return (SIZE_OF_BITARR - BitArrCountSetBits(bitarr));
}

bit_array_t BitArrMirrorO1(bit_array_t bitarr)
{
	bitarr = (((bitarr & 0xFFFFFFFF00000000) >> 32) |
                 ((bitarr  & 0x00000000FFFFFFFF) << 32));
	bitarr = (((bitarr & 0xFFFF0000FFFF0000) >> 16) |
                 ((bitarr  & 0x0000FFFF0000FFFF) << 16));
	bitarr = (((bitarr & 0xFF00FF00FF00FF00) >> 8)  |
                 ((bitarr  & 0x00FF00FF00FF00FF) << 8)) ;
	bitarr = (((bitarr & 0xF0F0F0F0F0F0F0F0) >> 4)  |
                 ((bitarr  & 0x0F0F0F0F0F0F0F0F) << 4)) ;
	bitarr = (((bitarr & 0xCCCCCCCCCCCCCCCC) >> 2)  |
                 ((bitarr  & 0x3333333333333333) << 2)) ;
	bitarr = (((bitarr & 0xAAAAAAAAAAAAAAAA) >> 1)  |
                 ((bitarr  & 0x5555555555555555) << 1)) ;
    
	return bitarr;
}

size_t BitArrCountSetBitsLut(bit_array_t bitarr)
{
	unsigned char mask = 0xff;	
	
	return ((bits_lut[bitarr & mask]) + (bits_lut[(bitarr >> 8) & mask]) +
	(bits_lut[(bitarr >> 16) & mask]) + (bits_lut[(bitarr >> 24) & mask]) +
	(bits_lut[(bitarr >> 32) & mask] ) + (bits_lut[(bitarr >> 40) & mask])+
	(bits_lut[(bitarr >> 48) & mask]) + (bits_lut[(bitarr >> 56) & mask]));
}

bit_array_t BitArrMirrorLut(bit_array_t bitarr)
{
	size_t i = 0;
	size_t mask = 0xFF;
	size_t mirror = 0;
	
	for (i = 0; i < CHAR_BIT; ++i)
	{
		mirror <<= CHAR_BIT; 
		mirror |= mirror_lut[bitarr & mask];
		bitarr >>= CHAR_BIT;
	}
	
	return mirror;
}









