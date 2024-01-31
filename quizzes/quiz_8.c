
#include <stdio.h>

typedef unsigned long size_t;

int MaxSubArr(int* arr, size_t size, int *start, int *end)
{
	int sum = 0;
	int subarr_sum = 0;
	size_t i = 0;
	*start = 0;
	*end = 0;
	for (i = 0; i < size; ++i)
	{
		if (0 < *(arr + i))
		{
			if (subarr_sum < sum + *(arr + i))
			{
				subarr_sum = sum + *(arr + i);
				*end = (int)i;
			}
			if (*(arr + i) > subarr_sum)
			{
				sum = 0;
				subarr_sum = *(arr + i);
				*start = (int)i;
				*end = (int)i;
			}
		}
		sum += *(arr + i);
	}
	return subarr_sum;
}

int main()
{
	int arr[] = {-2, 1, -3, 4, -1, 2, 3, -5, 4};
	int start = 0, end = 0;
	int sum = MaxSubArr(arr, 9, &start, &end);
	printf("\nSum = %d\nstart index = %d\nend index = %d\n\n", sum, start, end);
	return 0;
}

