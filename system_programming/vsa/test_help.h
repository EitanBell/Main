#ifndef __TEST_HELP_H__
#define __TEST_HELP_H__

/*   includes & macros   */

#include <stddef.h>		/* size_t */

#define CHECK_MARK ("\t\033[0;32m\xE2\x9C\x93\033[0m\n")
#define FAIL_MARK ("\t\033[0;31m\xE2\x9C\x97\033[0m\n")

typedef enum
{
	SUCCESS = 0,
	FAIL

} status_t;

/*   Function Declarations   */

void PrintUnitTitle(char *func_name);
void PrintTestTitle(char *tests_name);
void PrintCheckStat(int stat);
void PrintCheckTitle(char *tests_name, size_t test_num);
void PrintUnitStat(int stat, char *func_name);
void PrintTestStat(int stat, char *tests_name);

#endif /* __TEST_HELP_H__ */
