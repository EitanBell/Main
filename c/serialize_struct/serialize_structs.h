/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:                    *
*	Date: 23/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Serialize Struct    *
*				     *
**************************************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */ 

typedef struct realistic_grades 
{
	size_t math;
	size_t physics;
	size_t chemistry;
	size_t computer_science;
}realistic_grades_t;

typedef struct humanistic_grades 
{
	size_t sociology;
	size_t psycology;
	size_t literature;
	size_t poetry;
}humanistic_grades_t;

typedef struct grades 
{
	humanistic_grades_t humanistic;
	realistic_grades_t real;
	float sports;
}grades_t;

typedef struct student 
{
	char *first_name;
	char *last_name;
	grades_t grade;
}student_t;

enum operate_func {FAIL = 1, SUCCESS = 0};

void TestStudentGrades(void);
void InitGrades(student_t *name);
void TestPrintGrades(student_t *p_name);
int WriteStudentStructToFile(const char *file_name, student_t *student);
int ReadStudentStructFromFile(const char *file_name, student_t *student);




