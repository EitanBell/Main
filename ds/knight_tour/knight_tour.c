/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer:  Shauli	              *
*	Date: 14/02/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Knight Tour  			  *
*				                	  *
**************************************/

#include <assert.h> /* assert */
#include <time.h>	/* time */
#include <stdlib.h> /* qsort */
#include <stdio.h> /* printf */

#include "knight_tour.h" /* knight tour API */
#include "bit_array.h"	 /* bit array API */

#define BASE_SIZE (8)
#define BOARD_SIZE (64)
#define TIME_LIMIT (300)
#define TIME_UP (-1)
#define MAX_MOVES (8)
#define X_AXIS (0)
#define Y_AXIS (1)
#define NOT_VALID (64)

typedef enum validity
{
	VALID = 0,
	INVALID
} validity_t;

typedef enum status
{
	SUCCESS = 0,
	FAIL
} status_t;

enum bool_status
{
	FALSE = 0,
	TRUE
};

/*----------------------------------------------------------------------------*/
/*                              STATIC FUNCTIONS                              */
/*----------------------------------------------------------------------------*/

/*--------------- BOOLEAN functions ---------------*/							
static int IsValidPlace(bit_array_t board, int place);
static int IsTimeUp(time_t start, time_t end);
static int IsAllBoardVisited(bit_array_t board);

/*--------------- CONVERT functions ---------------*/
static size_t CordinatesToNum(size_t x, size_t y);
static void NumToCordinates(size_t num, size_t *Cordinates);

/*--------------- INITIALIZE functions ---------------*/
static void InitAllMovesLUT(int avail_moves[][BASE_SIZE]);
static bit_array_t SetPlaceToInValid(bit_array_t board, size_t place);
static void InitNextPos(int avail_moves[][BASE_SIZE], 
			    int next_positions[BASE_SIZE], int position, bit_array_t board);
			    
/*--------------- OTHER functions ---------------*/
static int FindPathRec(int avail_moves[][BASE_SIZE], size_t *path,  
	 					    bit_array_t board, int position, time_t start_time);
/* Uncomment to print all moves available */												    
/*static void PrintLUT(int all_moves[][BASE_SIZE]);*/
static size_t CountAvailMove(int avail_moves[][BASE_SIZE], int position, 
															 bit_array_t board);
static size_t GetMinMoves(int avail_moves[][BASE_SIZE], int position, 
															 bit_array_t board);	
static int CmpFunc(const void *data1, const void *data2);	
			    														 														 
/*----------------------------------------------------------------------------*/
/*                              MAIN FUNCTIONS                                */
/*----------------------------------------------------------------------------*/

/*------------------------------ KnightTour ----------------------------------*/

int KnightTour(size_t *path, size_t start_position)
{
	int all_moves[BOARD_SIZE][BASE_SIZE] = {0};
	bit_array_t board = 0;
	time_t start_time = 0;
	
	assert(NULL != path);
	assert(BOARD_SIZE > start_position);
	
	start_time = clock();
	InitAllMovesLUT(all_moves);
	
	*path = start_position;
	
/*	PrintLUT(all_moves);*/        /* Uncomment to print all moves available */
	
	return FindPathRec(all_moves, path + 1, board, start_position, start_time);
} 

/*----------------------------------------------------------------------------*/
/*                               HELPER FUNCTIONS                             */
/*----------------------------------------------------------------------------*/

/*------------------------------- IsValidPlace -------------------------------*/

static int IsValidPlace(bit_array_t board, int place)
{
	int was_visited = BitArrGetVal(board, place);
	
	return ((0 <= place) && (BOARD_SIZE > place) && !was_visited);
}

/*---------------------------- CordinatesToNum -------------------------------*/

static size_t CordinatesToNum(size_t x, size_t y)
{
	return (BASE_SIZE * y + x);
}

/*---------------------------- NumToCordinates -------------------------------*/

static void NumToCordinates(size_t num, size_t *Cordinates)
{
	assert(NULL != Cordinates);
	
	Cordinates[X_AXIS] = num & 7;
	Cordinates[Y_AXIS] = num >> 3;
}

/*---------------------------- InitAllMovesLUT -------------------------------*/

static void InitAllMovesLUT(int avail_moves[][BASE_SIZE])
{
	size_t i = 0;
	size_t j = 0;
	size_t x_step = 0;
	size_t y_step = 0;
	size_t cordinates[2] = {0};
	int curr_moves[BASE_SIZE][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2,-1},
											           {-1,-2}, {1,-2}, {2,-1}};
	assert(NULL != avail_moves);
	
	for (i = 0; BOARD_SIZE > i; ++i)
	{
		NumToCordinates(i, cordinates);
		
		for (j = 0; BASE_SIZE > j; ++j)
		{
			x_step = cordinates[X_AXIS] + curr_moves[j][X_AXIS];
			y_step = cordinates[Y_AXIS] + curr_moves[j][Y_AXIS];
			
			if ((BASE_SIZE > x_step) && (BASE_SIZE > y_step))
			{
				avail_moves[i][j] = CordinatesToNum(x_step, y_step);
			}
			else
			{
				avail_moves[i][j] = NOT_VALID;
			}
		}
	}
}

/*--------------------------- SetPlaceToInValid ------------------------------*/

static bit_array_t SetPlaceToInValid(bit_array_t board, size_t place)
{
	return (BitArrSetOn(board, place));
}

/*------------------------------- FindPathRec --------------------------------*/

static int FindPathRec(int avail_moves[][BASE_SIZE], size_t *path,  
						    bit_array_t board, int position, time_t start_time)
{
	int knight_runner = 0;
	size_t i = 0;
	int next_positions[BASE_SIZE] = {0};
	
	assert(NULL != avail_moves);
	assert(NULL != path);
	
	if (IsTimeUp(start_time, time(NULL)))
	{
		return TIME_UP;
	}

	if (!IsValidPlace(board, position))
	{
		return FAIL;
	} 
	
	board = SetPlaceToInValid(board, position);
	
	if (IsAllBoardVisited(board))
	{
		return SUCCESS;
	}
	
	InitNextPos(avail_moves, next_positions, position, board);
	
	for (i = 0; MAX_MOVES > i; ++i)
	{
		knight_runner = next_positions[i];
		
		if (IsValidPlace(board, knight_runner))
		{

			if (SUCCESS == FindPathRec(avail_moves, path + 1, board, 
													 knight_runner, start_time))
			{
				*path = (size_t)knight_runner;
				return SUCCESS;
			}
		}
	}
	return FAIL;
}

/*-------------------------------- IsTimeUp ----------------------------------*/

static int IsTimeUp(time_t start, time_t end)
{
	return (TIME_LIMIT > (end - start));
}

/*---------------------------- IsAllBoardVisited -----------------------------*/

static int IsAllBoardVisited(bit_array_t board)
{
	return (BOARD_SIZE == BitArrCountSetBitsLut(board));
}

/*------------------------------- PrintLUT -----------------------------------*/

/* Uncomment to print all moves available */

/*static void PrintLUT(int all_moves[][BASE_SIZE])*/
/*{*/
/*    size_t i = 0;*/
/*    size_t j = 0;*/
/*    */
/*    assert(NULL != all_moves);*/
/*    */
/*    for (i = 0; i < BASE_SIZE * BASE_SIZE; ++i)*/
/*    {*/
/*        printf("%lu: [", i);*/
/*        for (j = 0; j < BASE_SIZE - 1; ++j)*/
/*        {*/
/*            printf("%d, ", all_moves[i][j]);*/
/*        }*/
/*        printf("%d", all_moves[i][j]);*/
/*        */
/*        printf("]\n");*/
/*    }*/
/*    printf("\n");*/
/*}*/

/*---------------------------- CountAvailMove -----------------------------*/

static size_t CountAvailMove(int avail_moves[][BASE_SIZE], int position, 
															  bit_array_t board)
{
	size_t counter = 0;
	size_t i = 0;
	int next_pos = 0;
	
	assert(NULL != avail_moves);
	
	for (i = 0; BASE_SIZE > i; ++i)
	{
		next_pos = avail_moves[position][i];
		
		if (IsValidPlace(board, next_pos))
		{
			++counter;
		}
	}	
	
	return counter;
}

/*---------------------------- GetMinMoves -----------------------------*/

static size_t GetMinMoves(int avail_moves[][BASE_SIZE], int position, 
															  bit_array_t board)
{
	size_t count_moves = 0;
	size_t i = 0;
	size_t min = BASE_SIZE;
	int next_pos = 0;
	
	assert(NULL != avail_moves);
	
	for (i = 0; BASE_SIZE > i; ++i)
	{
		next_pos = avail_moves[position][i];
		
		if (IsValidPlace(board, next_pos))
		{
			count_moves = CountAvailMove(avail_moves, next_pos, board);
			
			if (min > count_moves)
			{
				min = count_moves;
			}
		}
	}	
	
	return min;
}

/*------------------------------- InitNextPos --------------------------------*/

static void InitNextPos(int avail_moves[][BASE_SIZE], 
			     int next_positions[BASE_SIZE], int position, bit_array_t board)
{
	size_t i = 0;
	size_t min_moves = 0;
	int next_pos = 0;
	
	assert(NULL != avail_moves);
	assert(NULL != next_positions);
	
	min_moves = GetMinMoves(avail_moves, position, board);

	for (i = 0; BASE_SIZE > i; ++i)
	{
		next_pos = avail_moves[position][i];
		
		if (IsValidPlace(board, next_pos) && 
		   (min_moves == CountAvailMove(avail_moves, next_pos, board)))
		{
			next_positions[i] = next_pos;
		}
		else
		{
			next_positions[i] = NOT_VALID;
		}
	}	
	
	qsort(next_positions, BASE_SIZE, sizeof(int), &CmpFunc);
}

/*-------------------------------- CmpFunc -----------------------------------*/

static int CmpFunc(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}


