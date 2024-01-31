/**************************************
 *	Developer: Eitan Bellaiche        *
 *	Date: 05/07/23	                  *
 *	Subject: Min sequence of I/D 	  *
 **************************************/

#include <iostream> 		// std
#include <string> 			// string

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

namespace ilrd
{

void AddFromPrevIToCurrI(std::string *resStr, int prev, int curr)
{
    for (; curr > prev; --curr)
    {
        *resStr += std::to_string(curr);
    }
}

std::size_t MinNumFromDISequence(const std::string& sequence)
{
	std::string resStr = "";
	std::size_t curr = 0;

	std::size_t i = 0;
	while ('\0' != sequence[i])
	{
		if ('I' == sequence[i])
		{
			AddFromPrevIToCurrI(&resStr, curr, i + 1);
			curr = i + 1;
		}
		else if ('D' != sequence[i])
		{
			throw std::invalid_argument("Invalid input"); 
		}

		++i;
	}

	AddFromPrevIToCurrI(&resStr, curr, i + 1);

	return std::stoi(resStr);
}

} // namespace ilrd;

using namespace ilrd;


int main(void)
{
	int status = 0;

	status += (21 == MinNumFromDISequence("D"));
	status += (12 == MinNumFromDISequence("I"));
	status += (321 == MinNumFromDISequence("DD"));
	status += (123 == MinNumFromDISequence("II"));
	status += (21435 == MinNumFromDISequence("DIDI"));
	status += (126543 == MinNumFromDISequence("IIDDD"));
	status += (321654798 == MinNumFromDISequence("DDIDDIID"));

	(status == 7) ? std::cout << "Test result: " << TEST_RES(0) << std::endl :
					std::cout << "Test result: " << TEST_RES(1) << std::endl;
}
