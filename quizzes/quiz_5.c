#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE (8)

int BursaBenefit(int *arr, int n, int *buy, int *sell);

int main()
{

	return 0;
}

int BursaBefit(int *arr, int n, int *buy, int *sell)
{
	int best_benefit = 0;
	int last_benefit_buy_ptr = 0;
	int last_benefit_sell_ptr = 0;
	
	buy = arr;
	sell = arr;
	
	while (sell < arr + n)
	{
		if (*sell < *buy)
		{
			buy = sell;
		}
		if (*sell - *buy > best_benefit)
		{
			best_benefit = *sell - *buy;
			last_benefit_buy_ptr = *buy;
			last_benefit_sell_ptr = *sell;
		}
		++sell;
	}
	*buy = last_benefit_buy_ptr;
	*sell = last_benefit_sell_ptr;
	
	return best_benefit;
	
}

