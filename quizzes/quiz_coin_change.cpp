/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Date: 12/05/23	                  *
*	Subject: Coin change              *
**************************************/

#include <iostream>		/* cout */
#include <ostream>		/* std::endl */

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")

using namespace std;

size_t CoinChange(size_t *arr_coins, size_t size, size_t sum);
void Swap(size_t *a, size_t *b);
void insertionSort(size_t *arr, size_t len);
int Test(void);

int main(void)
{
	cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	cout << "\t\t     CoinChange Test:   " << TEST_RES(Test()) << endl;
							
	cout << "\n\t****************** TESTS FINISHED ******************\n\n";
	
	return 0;
}

int Test(void)
{
	size_t arr[] = {1, 2, 5, 10};
	size_t sum[] = {22, 2, 60, 99};
	size_t res[] = {3, 1, 6, 12};
	int status = 0;
	
	for (size_t i = 0; 4 > i; ++i)
	{
		status += (res[i] == CoinChange(arr, 4, sum[i]));
	} 
		
	return ((4 == status) ? 0 : 1);
	
}


size_t CoinChange(size_t *coins, size_t len, size_t sum)
{
	size_t tmp_sum = 0;
	size_t coin_counter = 0;
	size_t *curr_coin = 0;
	
	insertionSort(coins, len);
	
	curr_coin = coins + len - 1;
	
	while (tmp_sum != sum)
	{
		while (tmp_sum + *curr_coin > sum)
		{
			--curr_coin;
		}
		
		tmp_sum += *curr_coin;
		++coin_counter;
	}
	
	return coin_counter;
}

void insertionSort(size_t *arr, size_t len)
{
    size_t i = 0;
    size_t key = 0;
    int j = 0;
    
    for (i = 1; i < len; i++)
    {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void Swap(size_t *a, size_t *b)
{
	size_t tmp = 0;
	
	tmp = *b;
	*b = *a;
	*a = tmp;
}



