

#include <stdio.h> /* printf */
 
void TowerOfHanoi(int n, char from_rod, char to_rod, char aux_rod)
{
	if (n == 0)
	{
		return;
	}
	if (n == 1)
	{
		printf("Disk 1 move from %c to %c\n", from_rod, to_rod);
		return;
	}

	TowerOfHanoi(n - 1, from_rod, aux_rod, to_rod);
	printf("Disk %d move from %c to %c\n", n, from_rod, to_rod);
	TowerOfHanoi(n - 1, aux_rod, to_rod, from_rod);
}
 
int main(void)
{
	TowerOfHanoi(3, 'A', 'C', 'B');
	return 0;
}

