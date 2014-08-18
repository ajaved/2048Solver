#include "Python.h"
#include "scoring_board.h"
#include <stdlib.h> 
#include <math.h>
#include <omp.h>
#define UP_MOVE 0
#define DOWN_MOVE 1
#define RIGHT_MOVE 2
#define LEFT_MOVE 3
#define BOARD_WIDTH 4
#define BOARD_LENGTH 4
#define NUM_BOARD_BLOCKS BOARD_LENGTH*BOARD_WIDTH

#define BETA 1

int max_depth = 1;

/*
 * Recursively looks ahead and scores board 
 * according to future merge possibilities.
 * Should initally be called with depth 
 * set to 0.
 */

float
score_board(int *board, int x_dim, int y_dim, int depth)
{
  int i, j;
 
  /* printf("Received Board:\n"); */
  /* for (i = 0; i < x_dim; i++){ */
  /*   for (j = 0; j < y_dim; j++){ */
  /*     printf("%d\t", *(board + i + BOARD_WIDTH*j)); */
  /*   } */
  /*   printf("\n"); */
  /* } */
  /* int num_empty_blocks = get_num_empty(board); */
  /* if (num_empty_blocks > 6){ */
  /*   depth++; */
  /* } */
  //printf("Depth is: %d\n", depth);

  float score = get_current_score(board);
  if (depth > max_depth - 1){
    return score;
  }

  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + j*BOARD_LENGTH) == 0){
	*(board + i + j*BOARD_LENGTH) = 2;

	int up_board[BOARD_LENGTH][BOARD_WIDTH];
	int down_board[BOARD_LENGTH][BOARD_WIDTH];
	int right_board[BOARD_LENGTH][BOARD_WIDTH];
	int left_board[BOARD_LENGTH][BOARD_WIDTH];
	memcpy((void *)up_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)down_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)left_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)right_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));

	*(board + i + j*BOARD_LENGTH) = 0;

	/* Move in all directions */
	/* move((int *)up_board, UP_MOVE); */
	/* move((int *)down_board, DOWN_MOVE); */
	/* move((int *)right_board, RIGHT_MOVE); */
	/* move((int *)left_board, LEFT_MOVE); */

#pragma omp parallel sections num_threads(4)
	{
	  
	  //printf ("section 1 id = %d, \n", omp_get_thread_num());
	  move((int *)up_board, UP_MOVE);
	  
	  //if (!same_board((int *)up_board, (int *)board)){
	  score += BETA * (score_board((int *)up_board, x_dim, y_dim, depth + 1));
	  //}

#pragma omp section
	  {
	    //printf ("section 2 id = %d, \n", omp_get_thread_num());
	    move((int *)down_board, DOWN_MOVE);
	    /* if (!same_board((int *)down_board, (int *)board)){ */
	    score += BETA * (score_board((int *)down_board, x_dim, y_dim, depth + 1));
	    /* } */
	  }
#pragma omp section
	  {
	    //printf ("section 3 id = %d, \n", omp_get_thread_num());
	    move((int *)right_board, RIGHT_MOVE);
	    /* if (!same_board((int *)right_board, (int *)board)){ */
	    score += BETA * (score_board((int *)right_board, x_dim, y_dim, depth + 1));
	    /* } */
	  }
#pragma omp section
	  {
	    //printf ("section 4 id = %d, \n", omp_get_thread_num());
	    move((int *)left_board, LEFT_MOVE);
	    /* if (!same_board((int *)left_board, (int *)board)){ */
	    score += BETA * (score_board((int *)left_board, x_dim, y_dim, depth + 1));
	    /* } */
	    
	  }
      }
	/* score += BETA * (score_board((int *)up_board, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)down_board, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)right_board, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)left_board, x_dim, y_dim, depth + 1)); */
	
	/* Lookahead assuming 4 got placed */
	*(board + i + j*BOARD_LENGTH) = 4;

	int up_board_four[BOARD_LENGTH][BOARD_WIDTH];
	int down_board_four[BOARD_LENGTH][BOARD_WIDTH];
	int right_board_four[BOARD_LENGTH][BOARD_WIDTH];
	int left_board_four[BOARD_LENGTH][BOARD_WIDTH];
	memcpy((void *)up_board_four, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)down_board_four, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)left_board_four, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
	memcpy((void *)right_board_four, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));

	*(board + i + j*BOARD_LENGTH) = 0;

	/* /\* Move in all directions *\/ */
	/* move((int *)up_board_four, UP_MOVE); */
	/* move((int *)down_board_four, DOWN_MOVE); */
	/* move((int *)right_board_four, RIGHT_MOVE); */
	/* move((int *)left_board_four, LEFT_MOVE); */

	
#pragma omp parallel sections num_threads(4)
	{
	  
	  //printf ("section 1 id = %d, \n", omp_get_thread_num());
	  move((int *)up_board_four, UP_MOVE);
	  
	  //if (!same_board((int *)up_board, (int *)board)){
	  score += BETA * (score_board((int *)up_board_four, x_dim, y_dim, depth + 1));
	  //}

#pragma omp section
	  {
	    //printf ("section 2 id = %d, \n", omp_get_thread_num());
	    move((int *)down_board_four, DOWN_MOVE);
	    /* if (!same_board((int *)down_board, (int *)board)){ */
	    score += BETA * (score_board((int *)down_board_four, x_dim, y_dim, depth + 1));
	    /* } */
	  }
#pragma omp section
	  {
	    //printf ("section 3 id = %d, \n", omp_get_thread_num());
	    move((int *)right_board_four, RIGHT_MOVE);
	    /* if (!same_board((int *)right_board, (int *)board)){ */
	    score += BETA * (score_board((int *)right_board_four, x_dim, y_dim, depth + 1));
	    /* } */
	  }
#pragma omp section
	  {
	    //printf ("section 4 id = %d, \n", omp_get_thread_num());
	    move((int *)left_board_four, LEFT_MOVE);
	    /* if (!same_board((int *)left_board, (int *)board)){ */
	    score += BETA * (score_board((int *)left_board_four, x_dim, y_dim, depth + 1));
	    /* } */
	    
	  }
      }

	/* if (!same_board((int *)up_board_four, (int *)board)){ */
	/*   score += BETA * (score_board((int *)up_board_four, x_dim, y_dim, depth + 1)); */
	/* } */
	/* if (!same_board((int *)down_board_four, (int *)board)){ */
	/*   score += BETA * (score_board((int *)down_board_four, x_dim, y_dim, depth + 1)); */
	/* } */
	/* if (!same_board((int *)right_board_four, (int *)board)){ */
	/*   score += BETA * (score_board((int *)right_board_four, x_dim, y_dim, depth + 1)); */
	/* } */
	/* if (!same_board((int *)left_board_four, (int *)board)){ */
	/*   score += BETA * (score_board((int *)left_board_four, x_dim, y_dim, depth + 1)); */
	/* } */


	/* score += BETA * (score_board((int *)up_board_four, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)down_board_four, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)right_board_four, x_dim, y_dim, depth + 1) + */
	/* 	score_board((int *)left_board_four, x_dim, y_dim, depth + 1)); */
	
      }
    }
  }
  
  /* float score = get_current_score(board) + */
  /*   BETA * (score_board((int *)up_board, x_dim, y_dim, depth + 1) + */
  /* 	    score_board((int *)down_board, x_dim, y_dim, depth + 1) + */
  /* 	    score_board((int *)right_board, x_dim, y_dim, depth + 1) + */
  /* 	    score_board((int *)left_board, x_dim, y_dim, depth + 1)); */
  
  return score;
}

int
get_num_empty(int *board)
{
  int i, j;
  int number_tiles = 0;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + BOARD_WIDTH*j) == 0){
	number_tiles++;
      }
    }
  }
  return(number_tiles);
}

int
same_board(int *board, int*check_board)
{
  int i, j;
  int same = 1;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + BOARD_WIDTH*j) != *(check_board + i + BOARD_WIDTH*j)){
	same = 0;
	break;
      }
    }
    if (!same)
      break;
  }
  return(same);
}

float
get_current_score(int *board)
{
  int i, j;
  float score = 0;
  int number_tiles = 0;
   for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + BOARD_WIDTH*j) != 0){
	number_tiles++;
	score += pow(*(board + i + BOARD_WIDTH*j), 2);
      }
    }
  }
  
  return(score/number_tiles);
}

void
move(int *current_board, int move)
{

  int i, j, stable, h;
  int new_board[BOARD_LENGTH][BOARD_WIDTH];
  memcpy((void *)new_board, (void *)current_board, NUM_BOARD_BLOCKS*sizeof(int));
  
  /* Transform board matrix to simplify fill and join */
  if (move == UP_MOVE){;}

  if (move == DOWN_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	new_board[BOARD_LENGTH - 1 - j][i] = *(current_board + BOARD_WIDTH*j + i);
      }
    }
  }

  if (move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	new_board[BOARD_LENGTH - 1 - i][j] = *(current_board + BOARD_WIDTH*j + i);
      }
    }
  }

  if (move == LEFT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	new_board[i][j] = *(current_board + BOARD_WIDTH*j + i);
      }
    }
  }

  /* First fill up empty spaces */
  for (i = 0; i < BOARD_WIDTH; i++){
    stable = 0;
    while (!stable){
      for (j = 0; j < BOARD_LENGTH; j++){
	if (j == 3){
	  stable = 1;
	  break;
	}
	if (new_board[j][i] == 0 && new_board[j + 1][i] != 0){
	  new_board[j][i] = new_board[j + 1][i];
	  new_board[j + 1][i] = 0;
	  break;
	}
      }
    }
  }

  /* printf("Joining numbers.\n"); */
  /* Now join like numbers */
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (new_board[j][i] != 0 && new_board[j][i] == new_board[j + 1][i]){

	/* # Double block value */
	new_board[j][i] = 2*new_board[j][i];
	new_board[j + 1][i] = 0;

	/* # Push up numbers below */
	for (h = j + 1; h < BOARD_LENGTH; h++){
	  if (h == 3)
	    new_board[h][i] = 0;
	  else
	    new_board[h][i] = new_board[h + 1][i];
	}
      }
    }
  }
  
  /* Undo matrix transform to correct board orientation */
  if (move == UP_MOVE){
    memcpy((void *)current_board, (void *)new_board, NUM_BOARD_BLOCKS*sizeof(int));
  }

  if (move == DOWN_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	*(current_board + BOARD_LENGTH*(BOARD_LENGTH - 1 - j) + i) = new_board[j][i];
      }
    }
  }

  if (move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
        *(current_board + BOARD_WIDTH*i + j) = new_board[BOARD_LENGTH - 1 - j][i];
      }
    }
  }

  if (move == LEFT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	*(current_board + BOARD_WIDTH*i + j) = new_board[j][i];
      }
    }
  }
}
