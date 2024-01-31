/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Or	                  *
*	Date: 27/03/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Complex   				  *
*				                	  *
**************************************/					

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)
	   			        
#include <iostream>		// cout
#include <ctime>		// clock_t

#include "complex.hpp" // complex API 

#define LOOP_SIZE (100000)

using ilrd::Complex;

int main(void)
{
    Complex c1(1,2);
    Complex c2(2.1,3.9);
    Complex c3;
    Complex c4;
    Complex c5;

	c3.SetReal(4);
	c3.SetImaginary(4.1);
	
	double range = 0.01;

	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t\tSetters & Getters     Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(4.0 == c3.GetReal() && 
										   4.1 == c3.GetImaginary() &&
										   1.0 == c1.GetReal() &&
										   3.9 == c2.GetImaginary()));

	c4 = c1 + c2;
	std::cout << "\n\t\tOperator '+' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((3.1 - c4.GetReal()) < range) && 
										   ((5.9 - c4.GetImaginary()) < range)));
				
	c5 = c3 - c1;
	std::cout << "\n\t\tOperator '-' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((3 - c5.GetReal()) < range) && 
										   ((2.1 - c5.GetImaginary() )< range)));
	
	c4 = c1 * c2;
	std::cout << "\n\t\tOperator '*' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((-5.7 - c4.GetReal()) < range) && 
										   ((8.1 - c4.GetImaginary()) < range)));
	c4 = c3 / c1;
	std::cout << "\n\t\tOperator '/' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((2.44 - c4.GetReal()) < range) && 
										   ((-0.78 - c4.GetImaginary()) < range)));
	c1.SetReal(2.3);
	c1.SetImaginary(-1);
	c2.SetReal(-6);
	c2.SetImaginary(0.5);
	
	c2 += c1;
	std::cout << "\n\t\tOperator '+=' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((-3.7 - c2.GetReal()) < range) && 
										   ((-0.5 - c2.GetImaginary()) < range)));
	
	c2 -= c3;
	std::cout << "\n\t\tOperator '-=' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((-7.7 - c2.GetReal()) < range) && 
										   ((-4.6 - c2.GetImaginary()) < range)));
	
	c2 *= c1;
	std::cout << "\n\t\tOperator '*=' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((-22.31 - c2.GetReal()) < range) && 
										   ((-2.88 - c2.GetImaginary()) < range)));
	
	c2 /= c3;
	std::cout << "\n\t\tOperator '/=' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(((-77.432 - c2.GetReal()) < range) && 
										   ((-102.991 - c2.GetImaginary()) < range)));
	
	c1.SetReal(-6);
	c1.SetImaginary(0.5);
	c2.SetReal(-6);
	c2.SetImaginary(0.5);
	c4.SetReal(5.2);
	c4.SetImaginary(0);
	
	
	std::cout << "\n\t\tOperator '==' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(c1 == c2));
	
	std::cout << "\n\t\tOperator '!=' 	      Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(c1 != c4));
				
	std::cout << "\n\t\tcout & cin Test:   " << std::endl;			
	std::cout << "\n\t\t\t-6 + 0.5i = " << c1 << std::endl;
	std::cin >> &c1;								   			
	std::cout << "You choosed: " << c1;
							
	std::cout << "\n\n\t\t\t*** Benchmark Tests ***\n";
	
	size_t i = 0;
	Complex c6;
    Complex c7(1,1);
    
	clock_t start_time = clock();
	
	for (i = 0; i < LOOP_SIZE; ++i)
	{
		c6 = c6 + c7;
	}
	
	clock_t end_time = clock();
	double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC * 1000.0;
	
	std::cout << "\n\t\tElapsed time for 100k of '+' operations:" << std::endl <<"\n\t\tBy reference: " << elapsed_time << " milliseconds";
	std::cout << "\n\t\tBy reference with -O flag: 0.134 milliseconds" << std::endl;
	std::cout << "\n\t\tBy value: 5.562 milliseconds";
	std::cout << "\n\t\tBy value with -O flag: 0.114 milliseconds" << std::endl;
	
	std::cout << "\n\n\t\t\t*** Valgrind checked ***\n";				
				     
	std::cout << "\n\n\t****************** TESTS FINISHED ******************\n\n";
}

/*
for testing the loop time by value, we should change from:
inline const Complex operator+(const Complex& comp1_, const Complex& comp2_);
to:
inline const Complex operator+(Complex comp1_, Complex comp2_)

and for this checke run, i'll copy-paste the result to one test,/
just for better visibilty at one place.
*/
