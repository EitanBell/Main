/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 14/05/23	                  *
*	Subject: BitArray Phase 2         *
*				                	  *     
**************************************/
#include <iostream>			// cout, endl

#include "bitarray_ph2.hpp"	// API

using namespace ilrd;
using namespace std;

void TestPhase2(void);

int main(void)
{
	TestPhase2();
	
	return 0;
}

void TestPhase2(void)
{
	cout << "Testing Assignment operation, result: ";
	BitArray<64> b1;
	BitArray<500> b2;

	b1[0] = true;
	b1[2] = false;
	b1[63] = true;
	b2[300] = true;

	if (b1[0] && !b1[2] && b1[63] && b2[300])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing Set bit and Get, result: ";
	b1.Set(37);
	b1.Set(0, false);
	b2.Set(400);

	if (!b1.Get(0) && b1.Get(37) && b2.Get(400))
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing set all, result: ";
	b1.Set();
	b2.Set();

	bool is_all_set = true;
	for (int i = 0; i < 64; ++i)
	{
		if (!b1[i])
		{
			cout << "am i here?\n";
			is_all_set = false;
		}
	}
	for (int i = 0; i < 500; ++i)
	{
		if (!b2[i])
		{
			is_all_set = false;
		}
	}

	if (is_all_set)
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing Flip bit, result: ";
	b1.Flip(5);
	b1.Flip(15);
	b1.Flip(50);
	b1.Flip(50);
	b2.Flip(333);

	if(!b1[5] && !b1[15] && b1[50] && !b2[333])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing Flip all, result: ";
	b1.Flip();
	b2.Flip();

	if (!b1[0] && b1[5] && b1[15] && !b1[50] && b2[333])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing operator ^=, result: ";
	BitArray<100> b3;
	BitArray<100> b4;

	b3.Set();
	b4[0] = true;

	b3 ^= b4;
	if (!b3[0] && b3[50] && b3[99])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing operator &=, result: ";
	
	b3.Set();
	b3 &= b4;
	if (b3[0] && !b3[50] && !b3[99])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing operator |=, result: ";
	
	b3.Set();
	b3[0] = false;
	b4[0] = false;

	b3 |= b4;
	if (!b3[0] && b3[50] && b3[99])
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing operator == & operator !=, result: ";
	b3.Set();
	b4.Set();
	BitArray<100> b5;
	b5 = b4;
	b5[50] = false;

	if(b3 == b4 && b3 != b5)
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing ToString, result: ";
	BitArray<10> b6;
	for (int i = 0; i < 10; i += 2)
	{
		b6[i] = true;
	}
	cout << b6.ToString() << " - ";
	if ("0101010101" == b6.ToString())
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing Count, result: ";
	BitArray<20> b7;
	
	for (int i = 0; i < 10; ++i)
	{
		b7[i] = true;
	}
	cout << b7.Count() << "/10 - ";
	if (10 == b7.Count())
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing Count and handling extra bit problem, result: ";
	BitArray<150> b8;
	b8.Set();
	cout << b8.Count() << "/150 - ";
	if (150 == b8.Count())
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing <<= operation, result: ";
	b1.Set();
	b1 <<= 32;

	BitArray<150> b9(b8);

	b8[115] = false;
	b8 <<= 30;

	b9 <<= 140;

	if (32 == b1.Count() && 119 == b8.Count() && 10 == b9.Count())
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	cout << "Testing >>= operation, result: ";
	b1.Set();
	b1 >>= 32;

	b8.Set();
	b8[115] = false;
	b8 >>= 30;

	b9.Set();
	b9 >>= 140;
	if (32 == b1.Count()  && 119 == b8.Count() && 10 == b9.Count())
	{
		cout << "\033[0;32mPASS\033[0m" << endl;
	}
	else
	{
		cout << "\033[0;31mFAIL\033[0m" << endl;
	}

	const size_t var = 8;
	BitArray<var> b;
}
