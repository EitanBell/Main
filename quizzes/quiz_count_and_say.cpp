/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 29/06/23	                  *
*	Subject: Count And Say algorithm  *
**************************************/

#include <iostream>			// std::cout, std::endl

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) (res ^ 1)

std::string CountAndSay(std::string& curr);

int main(void)
{
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	std::string curr1 = "1";
	std::string curr2 = "11";
	std::string curr3 = "111221";
	std::string curr4 = "31131211131221";

	std::cout << "\t       Count And Say Test #1:   " << 
			   TEST_RES(BOOL_TO_STATUS(("11" == CountAndSay(curr1)))) << std::endl;

	std::cout << "\t\t\t     Test #2:   " << 
			TEST_RES(BOOL_TO_STATUS(("21" == CountAndSay(curr2)))) << std::endl;

	std::cout << "\t\t\t     Test #3:   " << 
    TEST_RES(BOOL_TO_STATUS(("312211" == CountAndSay(curr3)))) << std::endl;

	std::cout << "\t\t\t     Test #4:   " << 
	TEST_RES(BOOL_TO_STATUS(("13211311123113112211" == CountAndSay(curr4)))) << std::endl;
							
	std::cout << "\n\t****************** TESTS FINISHED ******************\n\n";
	
	return 0;
}

std::string CountAndSay(std::string& curr)
{
	std::string resStr;
	char digit;
	std::size_t digitCount = 0;
	std::string::iterator it = curr.begin();
	
	while ('\0' != *it)
	{
		do
		{
			digit = *it;
			++digitCount;
			++it;
		} while (digit == *it);

		resStr += std::to_string(digitCount);
		resStr += digit;

		digitCount = 0;
	}

	return resStr;
}


