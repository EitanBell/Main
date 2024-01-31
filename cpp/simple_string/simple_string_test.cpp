/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 23/03/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Simple String   		  *
*				                	  *
**************************************/				

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)
	   			        
#include <iostream>		// cout
#include <cstring>		// strcpy, strlen 

#include "simple_string.hpp" // simple_string API 

using ilrd::String;

int main(void)
{
    String s1("The lenght of this should be 31");
    String s2(s1);
    String s3("The lenght of this should be 31");
    String s4("SHORT");
    s1 = s2;
    
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t\tLength and Cstr     Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(s1.Length() == s2.Length()));
	std::cout << "\n\t\tCopy and Assignment Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(0 ==	strcmp(s1.Cstr(), s2.Cstr())));
	std::cout << "\n\t\tOperator ==, >, <   Test:   " 
				<< TEST_RES(BOOL_TO_STATUS((s1 == s3) && (s2 > s4) && (s4 < s1)));
	std::cout << "\n\t\tcout	   	    Test:   ";
	std::cout << s1 << std::endl;
							
	std::cout << "\n\n\t\t     *** Valgrind checked ***";	
								     
	std::cout << "\n\n\t****************** TESTS FINISHED ******************\n\n";
}


