/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	 	          	      *
*	Date: 14/05/23	                  *
*	Subject: BitArray			      *
*				                	  *
**************************************/

#include <iostream> // ostream

#include "bitarray_ph1.hpp" // bitarray_ph1 API 

using namespace ilrd;
using namespace std;

void BitArrayTest1();
void BitArrayTest2();
/*------------------------------- BitArray TEST ------------------------------*/

int main(void)
{
    BitArrayTest1();
    BitArrayTest2();

	return 0;
}

/*---------------------------- BitArrayTest1 TEST ----------------------------*/

void BitArrayTest1()
{
    BitArray ba1;
    BitArray ba2;

    size_t status = 0;
    size_t i = 0;

    for (i = 0; i < 64; ++i)
    {
        ba1[i] = i % 2;
    }

    for (i = 0; i < 64; ++i)
    {
        ba2[i] = ba1[i + 1];
    }

    for (i = 0; i < 64; ++i)
    {
        status <<= 1;

        status |= ba1[i] & ba2[i];
    }

    cout << "Phase 1 BitProxy operator[] Test: ";
    (status == 0) ? cout << "SUCCESS" : cout << "FAIL";
    cout << endl;
}

void BitArrayTest2()
{
    const BitArray b1(2);
    BitArray b2(2);

    if (b1[1])
    {
        if (b2[1])
        {
            cout << "Phase 1 bool operator[] Test: SUCCESS" << endl;
        }
        else
        {
            cout << "Phase 1 bool operator[] Test: FAIL" << endl;
        }
    }
}