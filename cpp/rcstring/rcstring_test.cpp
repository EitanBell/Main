/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah	              *
*	Date: 01/04/23	                  *
*	Valgrind & gdb tested             *
*	Subject: RCString		   		  *
*				                	  *
**************************************/		

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)
	   			        
#include <iostream>		// cout
#include <cstring>		// strcpy, strlen 

#include "rcstring.hpp"	// rcstring API

using ilrd::RCString;

int main(void)
{
    RCString s1("Eitan");	
    RCString s2(s1);
    RCString s3 = "Bell";
    RCString s4 = s3;
    RCString s5 = s1;
    
    s5[0] = 'T';
    
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t\tLength and Cstr     Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(s1.Length() == s2.Length()));
	std::cout << "\n\t\tCopy and Assignment Test:   " 
				<< TEST_RES(BOOL_TO_STATUS(0 ==	strcmp(s1.Cstr(), s2.Cstr())));
	std::cout << "\n\t\tOperator ==, >, <   Test:   " 
				<< TEST_RES(BOOL_TO_STATUS((s1 == s2) && (s2 > s4) && (s4 < s1)));
	std::cout << "\n\t\tcout	   	    Test:   ";
	std::cout << s1 << " " << s3 << std::endl;
	std::cout << "\t\tOperator []         Test:   " 
				<< TEST_RES(BOOL_TO_STATUS('B' == s3[0] && (0 == strcmp("Titan", s5.Cstr()))));

	std::cout << "\n\n\t\t     *** Valgrind checked ***";	
								     
	std::cout << "\n\n\t****************** TESTS FINISHED ******************\n\n";
}


