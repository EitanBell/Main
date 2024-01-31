/*************************************
*				     *
*	Developer: Eitan Bellaiche   *	 
*	Reviewer:                    *
*	Date: 23/11/22	             *
*	Valgrind & GDB passed        *
*	Subject: Serialize Struct    *
*				     *
**************************************/


#include "serialize_structs.h" /* my struct */


void TestStudentGrades(void)
{
	student_t Eitan = {0};
	student_t Eitan2 = {0};
	
	const char *ser_file_name = "myfile";
	int write_check = 0;
	
	InitGrades(&Eitan);
	TestPrintGrades(&Eitan);
	
	write_check = WriteStudentStructToFile(ser_file_name, &Eitan);
	if (FAIL == write_check)
	{
		printf("\n*** The file was failed to write ***\n\n");
	}
	write_check = ReadStudentStructFromFile(ser_file_name, &Eitan2);
	if (FAIL == write_check)
	{
		printf("\n*** The file was failed to read ***\n\n");
	}
	TestPrintGrades(&Eitan2);
		
}

void InitGrades(student_t *p_name)
{
	p_name -> first_name = "Eitan";
	p_name -> last_name = "Bellaiche";
	p_name -> grade.humanistic.sociology = 70;
	p_name -> grade.humanistic.psycology = 90;
	p_name -> grade.humanistic.literature = 88;
	p_name -> grade.humanistic.poetry = 80;
	
	p_name -> grade.real.math = 95;
	p_name -> grade.real.physics = 96;
	p_name -> grade.real.chemistry = 97;
	p_name -> grade.real.computer_science = 100;
	
	p_name -> grade.sports = 92;
}

void TestPrintGrades(student_t *p_name)
{
	printf("\n---------- Student Grades ----------\n\n");
	printf("Grades in realistics:\n\n\tmath:\t\t\t%lu\n\tphysics:\t\t%lu\n"
		"\tchemistry:\t\t%lu\n\tcomputer_science:\t%lu\n\n",
		p_name -> grade.real.math,
		p_name -> grade.real.physics, 
		p_name -> grade.real.chemistry,
		p_name -> grade.real.computer_science);
		
	printf("Grades in humanistics:\n\n\tsociology:\t\t%lu\n\tpsycology:\t\t%lu\n"
		"\tliterature:\t\t%lu\n\tpoetry:\t\t\t%lu\n\n",
		p_name -> grade.humanistic.sociology,
		p_name -> grade.humanistic.psycology,
		p_name -> grade.humanistic.literature,
		p_name -> grade.humanistic.poetry);	
		
	printf("\tGrade in sports:\t%.2f\n", p_name -> grade.sports);	
}


int WriteStudentStructToFile(const char *file_name, student_t *student)
{
	size_t f_check = 0;
	FILE * p_file = NULL;
	
	p_file = fopen(file_name, "wb");
	if (NULL == p_file)
	{	
		return FAIL;
	}
	
	f_check = fwrite(student, sizeof(student_t), 1, p_file);			
	if (1 != f_check)
	{
		return FAIL;
	}
	
	fclose(p_file);
	
	return SUCCESS;
		
	
}
int ReadStudentStructFromFile(const char *file_name, student_t *student)
{
	size_t f_check = 0;
	FILE * p_file = NULL;
	
	p_file = fopen(file_name, "rb");
	if (NULL == p_file)
	{	
		return FAIL;
	}
	
	f_check = fread(student, sizeof(student_t), 1, p_file);
	if (1 != f_check)
	{
		return FAIL;
	}
	
	fclose(p_file);
	
	return SUCCESS;	
}








