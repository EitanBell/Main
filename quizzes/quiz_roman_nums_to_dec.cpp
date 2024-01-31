/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 27/06/23	                  *
*	Subject: Roman To Decimal 		  *
**************************************/

#include <iostream>			// std::cout, std::endl
#include <unordered_map>	// std::unordered_map


#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) (res ^ 1)

std::size_t RomanToDecimal(std::string romNUm);
std::unordered_map<char, std::size_t> InitRomMap(void);

int main(void)
{
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t       Roman To Decimal Test #1:   " << 
			   TEST_RES(BOOL_TO_STATUS((4 == RomanToDecimal("IV")))) << std::endl;

	std::cout << "\t       Roman To Decimal Test #2:   " << 
			TEST_RES(BOOL_TO_STATUS((49 == RomanToDecimal("XLIX")))) << std::endl;

	std::cout << "\t       Roman To Decimal Test #3:   " << 
    TEST_RES(BOOL_TO_STATUS((3724 == RomanToDecimal("MMMDCCXXIV")))) << std::endl;

	std::cout << "\t       Roman To Decimal Test #4:   " << 
	TEST_RES(BOOL_TO_STATUS((3278 == RomanToDecimal("MMMCCLXXVIII")))) << std::endl;
							
	std::cout << "\n\t****************** TESTS FINISHED ******************\n\n";
	
	return 0;
}

std::size_t RomanToDecimal(std::string romNum)
{
	static std::unordered_map<char, std::size_t> romMap = InitRomMap();
	std::size_t tmpSum1 = 0;
	std::size_t tmpSum2 = 0;

	for (std::string::iterator it = romNum.begin(); romNum.end() != it; ++it)
	{
		if (0 == tmpSum1)
		{
			tmpSum1 = romMap[*it];
			++it;
		}
		
		if (romMap[*it] > tmpSum1)
		{
			tmpSum1 = romMap[*it] - tmpSum1;
		}
		else if (romMap[*it] == tmpSum1)
		{
			tmpSum1 += romMap[*it];
		}
		else
		{
			tmpSum2 += tmpSum1;
			tmpSum1 = romMap[*it];
		}
	}

	return tmpSum1 + tmpSum2;
}

std::unordered_map<char, std::size_t> InitRomMap(void)
{
	std::unordered_map<char, std::size_t> romMap;

	romMap['I'] = 1;
	romMap['i'] = 1;

	romMap['v'] = 5;
	romMap['V'] = 5;
	
	romMap['x'] = 10;
	romMap['X'] = 10;

	romMap['l'] = 50;
	romMap['L'] = 50;

	romMap['c'] = 100;
	romMap['C'] = 100;

	romMap['d'] = 500;
	romMap['D'] = 500;

	romMap['m'] = 1000;
	romMap['M'] = 1000;

	return romMap;
}



