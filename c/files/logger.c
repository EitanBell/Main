/*--------------------------------------------------*/
/* DEVELOPER: Eitan Bellaiche                       */
/* DATE MODIFIED: 15.11.2022                        */
/* WORKSHEET NAME: files                            */
/*                                                  */
/* Reviewer:                                        */
/*--------------------------------------------------*/

#include <stdio.h> /*fopen*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*strlen, strcmp*/

#define FILE_INDEX 1
#define TRUE 1
#define ERROR -1
#define BUFFER_SIZE 1000
#define ARR_SIZE 5

enum operate_func {FAIL = -1, SUCCESS = 0};
typedef int (*cmp_func)(const char* str1, const char* str2);
typedef enum operate_func (*operation_func)();

typedef struct compare_and_operate
{
	const char *str;
	cmp_func Compare;
	operation_func Operation;
}compare_and_operate;

/***********************FUNCTIONS**************************/

int WriteToFile(int argc, char **argv);
void InitArrayAndInput(const char *filename);
void GetInputFromUser(const char *filename, compare_and_operate *array, size_t arr_size);
int CompareInput(const char* command, const char* str);
int CompareTrue();
enum operate_func AppendToFile(const char* filename, const char* str);
enum operate_func RemoveFile(const char* filename);
enum operate_func CountLines(const char* filename);
enum operate_func Exit();
enum operate_func AppendToStart(const char* filename, const char* str);
enum operate_func CopyFileContent(const char* src_name, const char* dest_name);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "error: user didn't enter file name\n");
		return ERROR;
	}
	
	InitArrayAndInput(argv[FILE_INDEX]);
	
	return 0;
}

void InitArrayAndInput(const char *filename)
{
	compare_and_operate array[ARR_SIZE] =
	{{"-remove", &CompareInput, &RemoveFile},
	{"-count", &CompareInput, &CountLines},
	{"-exit", &CompareInput, &Exit},
	{"<", &CompareInput, &AppendToStart},
	{"", &CompareTrue, &AppendToFile}};
	
	GetInputFromUser(filename, array, ARR_SIZE);
}

void GetInputFromUser(const char *filename, compare_and_operate *array, size_t arr_size)
{
	size_t i = 0;
	char buffer[BUFFER_SIZE] = {0};
	int exit_value = 0;
	
	printf("Hello user !\nEnter a string for chaining it to your file "
		 "\nEnter '-remove' to remove your file."
		 "\nEnter '-count' to see how many rows are in the file."
		 "\nEnter '-exit' to exit the program."
		 "\nEnter '<' and your string to chain it to the top of your file.\n");

	while (TRUE)
	{
		while (NULL != fgets(buffer, BUFFER_SIZE, stdin))
		{
			for (i = 0; i < arr_size; ++i)
			{
				if (0 == (array + i) -> Compare((array + i) -> str, buffer))
				{
					exit_value = (array+ i) -> Operation(filename, buffer);
					
					if (FAIL == exit_value)
					{
						return;
					}
					i = arr_size;
				}
			}
		}
	}
}

int CompareInput(const char* command, const char* str)
{
	int res = strncmp(command, str, strlen(command));
	
	return res;
}

int CompareTrue()
{
	return 0;
}

enum operate_func AppendToFile(const char* filename, const char* str)
{
	FILE* file = fopen(filename, "a");
	if (NULL == file)
	{
		return FAIL;
	}
	
	fputs(str, file);
	fclose(file);
	
	return SUCCESS;
}

enum operate_func RemoveFile(const char* filename)
{
	remove(filename);
	
	return FAIL;
}

enum operate_func CountLines(const char* filename)
{
	int count = 0;
	char c = '\0';
	
	FILE *file = fopen(filename, "r");
	if (NULL == file)
	{
		return FAIL;
	}
	
	c = fgetc(file);
	
	while (c != EOF)
	{
		c = fgetc(file);
		if ('\n' == c)
		{
			++count;
		}
	}
	
	fclose(file);
	printf("Number of lines in file is: %d\n", count);
	return SUCCESS;
}

enum operate_func Exit()
{
	return FAIL;
}

enum operate_func AppendToStart(const char* filename, const char* str)
{
	FILE *tmp = NULL;
	FILE *original = NULL;
	char buffer[BUFFER_SIZE] = {0};
	
	tmp = fopen("tmp", "a");
	if (NULL == tmp)
	{
		return FAIL;
	}
	
	original = fopen(filename, "r");
	if (NULL == original)
	{
		fclose(tmp);
		remove("tmp");
		return FAIL;
	}
	
	fputs(str + 1, tmp);

	while (NULL != fgets(buffer, BUFFER_SIZE, original))
	{
		fputs(buffer, tmp);
	}
	
	fclose(original);
	fclose(tmp);
	
	return CopyFileContent("tmp", filename);
}

enum operate_func CopyFileContent(const char* src_name, const char* dest_name)
{
	FILE *src = NULL;
	FILE *dest = NULL;
	char buffer[BUFFER_SIZE] = {0};
	
	src = fopen(src_name, "r");
	if (NULL == src)
	{
		return FAIL;
	}
	
	dest = fopen(dest_name, "w");
	if (NULL == dest)
	{
		fclose(src);
		remove(src_name);
		return FAIL;
	}
	while (NULL != fgets(buffer, BUFFER_SIZE, src))
	{
		fputs(buffer, dest);
	}
	
	fclose(dest);
	fclose(src);
	remove(src_name);
	
	return SUCCESS;
}

