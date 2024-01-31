

#include <stdio.h>

double Sqrt(double number);

int main(void)
{
	
	printf("sqrt of 100 \tis: %.3f \n", Sqrt(100));
	printf("sqrt of 7 \tis: %.3f \n", Sqrt(7));
	printf("sqrt of 64 \tis: %.3f \n", Sqrt(64));
	printf("sqrt of 10\tis: %.3f \n", Sqrt(10));
	printf("sqrt of 1 \tis: %.3f \n", Sqrt(1));
	printf("sqrt of 10000 \tis: %.3f \n", Sqrt(10000));
	printf("sqrt of 9 \tis: %.3f \n", Sqrt(9));
	
	
	return 0;
}

double Sqrt(double number)
{
	double res = 0;
	double tmp = 0;
	
	if (0 > number)
	{
		return -1;
	}
	
	res = number / 2;
	
	while (res != tmp)
	{
		tmp = res;
		res = (number / tmp + tmp) / 2;
	}

	return res;
}

