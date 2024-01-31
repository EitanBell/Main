/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Aviv	                  *
*	Date: 05/04/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Standard Containers   	  *
*				                	  *
**************************************/
#include <iostream>		// cout, cin, getline
#include <queue>		// push, pop, empty, front
#include <map>			// iterator, find, end, first, second

namespace ilrd
{
void GetInput(std::queue<std::string>& que, std::map<std::string, int>& map);
void PrintHist(std::queue<std::string>& que, std::map<std::string, int>& map);

/*--------------------------------- Histo ------------------------------------*/

void Histo(void)
{
	std::queue<std::string> str_que;
	std::map<std::string, int> histo_map;
	
	GetInput(str_que, histo_map);
	PrintHist(str_que, histo_map); 
}

/*--------------------------------- GetInput ---------------------------------*/

void GetInput(std::queue<std::string>& que, std::map<std::string, int>& map)
{
	std::cout << "Please enter your input:" << std::endl;
	std::cout << "Finish by entering a dot (.)." << std::endl;

	std::map<std::string, int>::iterator val_iter;
	std::string curr_input;
	
	do
	{
		std::getline(std::cin, curr_input);
		
		val_iter = map.find(curr_input);
		
		if (val_iter != map.end())
		{
			++(val_iter->second);
		}
		else
		{
			map[curr_input] = 1;
			que.push(curr_input);
		}
		
	} while ("." != curr_input);
}

/*--------------------------------- PrintHist --------------------------------*/

void PrintHist(std::queue<std::string>& que, std::map<std::string, int>& map)
{
	std::cout << "The Hist output is:" << std::endl;
	
	std::map<std::string, int>::iterator val_iter;
	
	while (!que.empty())
	{
		val_iter = map.find(que.front());
		if (1 < val_iter->second)
		{
			std::cout << val_iter->second << " x ";
		}
		
		std::cout << val_iter->first << std::endl;
		
		que.pop();
	}
}

} // ilrd

/*------------------------------------ main ----------------------------------*/

int main(void)
{
	ilrd::Histo();
	
	return 0;
}


