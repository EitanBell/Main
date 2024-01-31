/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:  Gad     *
*	Date: 28/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: atoi & itoa         *
*				     *
**************************************/

#ifndef ATIT_H_
#define ATIT_H_

#define IS_LITTLE_ENDIAN (*(unsigned short *)"\0\xFF" == 0xFF00)

typedef enum
{
	SUCCESS = 0,
	FAIL

} status_t;

typedef enum
{
	FALSE = 0,
	TRUE
} boolian_t; 

char *Itoa (int value , char *str, int base);  
char *ItoaBase10 (int value , char *str); 
int Atoi(const char *nptr, int base);
int AtoiBase10(const char *nptr);

void PrintLetters(const char *str1, const char *str2, const char *str3,
					int len1, int len2, int len3);
					
boolian_t IsLittelEndian(void);

#endif

