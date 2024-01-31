/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Matan          	      *
*	Date: 30/04/23	                  *
*	Subject: Tree			   		  *
*				                	  *
**************************************/
#include <iostream> // cout

#include "tree.hpp"	// tree API

int main(void)
{
	std::cout << std::endl << "Test for VALID directory:" << std::endl << std::endl;
	
    ilrd::Tree ValidTree("/home/eitan/git/system_programming");
    ValidTree.Print();
    
    std::cout << std::endl << "Test for INVALID directory:" << std::endl << std::endl;
    
    ilrd::Tree *InvalidTree = 0;
    
    try
    {
    	InvalidTree = new ilrd::Tree("/home/not_eitan/git");
    }
    
	catch (std::exception& e)
	{
		std::cerr << "From user: Failed to open directory" << std::endl;
		return 1;
	}
	
	InvalidTree->Print();
	
    return 0;
}


