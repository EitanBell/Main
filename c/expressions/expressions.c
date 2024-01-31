#include <stdio.h>

int PositivePower(int base, int power);
void Swap(int *a, int *b);
float PowerOfTen(int power);

int main(void)
{
	//-----------------------------QUESTION 4-----------------------------
	int n;
	float result;
	int num, size = 0, num_copy, reverse_num = 0, i;
	int a = 3, b = 5;

	printf("Please enter a numer for power: \n ");
	scanf("%d", &n);
	
	result = PowerOfTen(n);
	
	printf("The result power of %d in base 10 is: %f\n", n, result);
}

float PowerOfTen(int power)
{
	int i;
	float result = 1, base = 10;
	
	if(power < 0)
	{
		base = 0.1;
		power *= -1;
	}
	
	for(i = 0; i < power; i++)
	{
		result *= base;
	}
	return result;
}
	//------------------------------QUESTION 5---------------------------
	
	
	printf("Please enter a number to reverse: \n");
	scanf("%d", &num);
	
	num_copy = num;
	
	while(num_copy != 0)
	{
		size++;
		num_copy /= 10;
	}
	
	num_copy = num;

	for(i = size-1; i > 0; i--)
	{
		reverse_num += (num_copy % 10) * PositivePower(10,i);
		num_copy /= 10;
	}
	reverse_num += num_copy;   		//Adds the unity digit
	
	printf("%d --> %d\n", num, reverse_num);
}

int PositivePower(int base, int power)
{
	int result = 1;
	for(int i = 0; i < power; i++)
	{
		result *= base;
	}
	return result;
}
	
	//-------------------------QUESTION 6--------------------------
	
	printf("Before swap: a = %d, b = %d\n", a, b);
	Swap(&a, &b);
	printf("After swap: a = %d, b = %d\n", a, b);

}

void Swap(int *a, int *b)
{
	int temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
}
