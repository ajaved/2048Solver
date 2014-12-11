//#include "Python.h"
#include <unistd.h>
#include "scoring_board.h"
#include <stdlib.h> 
#include <math.h>
#include <omp.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

/* Define mapping from move direction to integer */
#define UP_MOVE 0
#define DOWN_MOVE 1
#define RIGHT_MOVE 2
#define LEFT_MOVE 3

#define BOARD_WIDTH 4
#define BOARD_LENGTH 4
#define NUM_BOARD_BLOCKS BOARD_LENGTH*BOARD_WIDTH
#define NUM_MOVES 4
#define BETA 0.8

//#define TEST

int max_depth = 1;
int moves[] = {UP_MOVE, DOWN_MOVE, RIGHT_MOVE, LEFT_MOVE};

#ifdef TEST

int
main()
{
  //int board[] = {0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,             //16384, 0, 0};
/* int board[] = {0, 0, 8, 0}; */
//int board[] = {2, 2, 2, 2, 4, 4, 4, 4, 8, 8, 8, 8, 16, 512, 1024, 0};
int board[] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0};
float max_f = FLT_MAX;
float min_f = FLT_MIN;
int depth = 0;
max_depth = 4;
float score = alpha_beta_scoring(board, &min_f, &max_f, depth);
  
printf("Score is: %0.2f\n", score);
return 0;
}
#endif

/* Scores board */
float
score_board(int *board, int depth)
{
  int i, j, m;
  float score = get_current_score(board);
 
  if (depth > max_depth - 1){
    return score;
  } /* else { */
  /*   score = 0; */
  /* } */

  int orig_board[NUM_BOARD_BLOCKS];
  memcpy((void *)orig_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));

  float scores[4] = {0};
  /* for (m = 0; m < NUM_MOVES; m++){ */
  /*   move((int *)board, moves[m]); */
  /*   if (same_board((int *)orig_board, (int *)board)){ */
  /*     scores[m] = FLT_MIN; */
  /*   } */
  /*   memcpy((void *)board, (void *)orig_board, NUM_BOARD_BLOCKS*sizeof(int)); */
  /* } */

  /* int move_flag = 0; */
  int board_after_move[NUM_BOARD_BLOCKS];
  int possible_new_numbers[] = {2, 4};
  int n;
  int n_length = sizeof(possible_new_numbers) / sizeof(possible_new_numbers[0]);


  for (m = 0; m < NUM_MOVES; m++){
    move((int *)board, moves[m]);
    if (!same_board((int *)orig_board, (int *)board)){
      memcpy((void *)board_after_move, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));
      /* Now add opponent's number */
      for (j = 0; j < BOARD_LENGTH; j++){
	for (i = 0; i < BOARD_WIDTH; i++){
	  if (*(board + i + j*BOARD_WIDTH) == 0){
	    for (n = 0; n < n_length; n++){
	      /* Lookahead assuming 2/4 got placed */
	      *(board + i + j*BOARD_WIDTH) = possible_new_numbers[n];
	      scores[m] += BETA * (score_board((int *)board, depth + 1));
	      memcpy((void *)board, (void *)board_after_move, NUM_BOARD_BLOCKS*sizeof(int));
	    }
	  }
	}	
      }
      
      memcpy((void *)board, (void *)orig_board, NUM_BOARD_BLOCKS*sizeof(int));
      /* move_flag = 1; */
    }
  }

  
  /* for (j = 0; j < BOARD_LENGTH; j++){ */
  /*   for (i = 0; i < BOARD_WIDTH; i++){ */
  /*     if (*(board + i + j*BOARD_WIDTH) == 0){ */
  /* 	for (n = 0; n < n_length; n++){ */
  /* 	  /\* Lookahead assuming 2/4 got placed *\/ */
  /* 	  *(board + i + j*BOARD_WIDTH) = possible_new_numbers[n]; */
  /* 	  memcpy((void *)board_before_move, (void *)board, NUM_BOARD_BLOCKS*sizeof(int)); */

  /* 	  for (m = 0; m < NUM_MOVES; m++){ */
  /* 	    /\* if (scores[m] == FLT_MIN) *\/ */
  /* 	    /\*   continue; *\/ */
  /* 	    move((int *)board, moves[m]); */
  /* 	    if (!same_board((int *)board_before_move, (int *)board)){ */
  /* 	      scores[m] += BETA * (score_board((int *)board, depth + 1)); */
  /* 	      memcpy((void *)board, (void *)board_before_move, NUM_BOARD_BLOCKS*sizeof(int)); */
  /* 	      /\* move_flag = 1; *\/ */
  /* 	    } else  */
  /* 	      scores[m] = FLT_MIN; */
  /* 	  } */
  /* 	  memcpy((void *)board, (void *)orig_board, NUM_BOARD_BLOCKS*sizeof(int)); */
  /* 	} */
  /*     } */
  /*   } */
  /* } */

  int max_move = 0;
  int max_score = FLT_MIN;
  int score_ind;
 
  for (score_ind = 0; score_ind < NUM_MOVES; score_ind++){
    if (scores[score_ind] > max_score){
      max_score = scores[score_ind];
      max_move = score_ind;
    }
    if (score != FLT_MIN)
      score += scores[score_ind];
  }
 
  if (!depth){
    //printf("Next move: %d\n", max_move);
    if (max_score == 0)
      return (float) 5;
    return (float)max_move;
  }

  //score = scores[0] + scores[1] + scores[2] + scores[3];
  return score;
}

/*
 * Recursively looks ahead and scores board 
 * according to future merge possibilities.
 * Should initally be called with depth 
 * set to 0.
 */

float
alpha_beta_scoring(int *board, float *alpha, float *beta, int depth)
{
  int m;
  float score = get_current_score(board);

  printf("Received board:\n");
  /* printb(board); */
  /* printf("\n"); */

  /* int board_max = get_max(board); */
  //int num_empty = get_num_empty(board);
  /* printf("Current max is: %d\n", board_max); */
  if (depth > max_depth){
    /* if (*beta > score) */
    /*   *beta = score; */
    return score;
  }
  
  float alpha_current = FLT_MIN;
  float current_score;
  float max_score = FLT_MIN;
  int max_move = 0;
  int orig_board[NUM_BOARD_BLOCKS];
  memcpy((void *)orig_board, (void *)board, NUM_BOARD_BLOCKS*sizeof(int));

  int move_possible = 0;
  for (m = 0; m < NUM_MOVES; m++){

    move((int *)board, moves[m]);

    /* printf("Made move: %d\n", m); */
    /* printb(board); */


    if (!same_board((int *)orig_board, (int *)board)){
      move_possible = 1;
      current_score = 0.99*opponent_move(board, &alpha_current, beta, depth);
      if (current_score > max_score){
    	max_score = current_score;
	max_move = m;
      }
      if (alpha_current > *beta)
      	m = NUM_MOVES;
    }
    memcpy((void *)board, (void *)orig_board, NUM_BOARD_BLOCKS*sizeof(int)); 
  }

  if (!move_possible)
    return score;

  if (*beta > alpha_current)
    *beta = alpha_current;

  if (depth == 0)
    return((float)max_move);

  return max_score;
}

float
opponent_move(int *board, float *alpha, float *beta, int depth)
{
  int i, j;
  float current_score;
  float beta_current = FLT_MAX;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + j*BOARD_WIDTH) == 0){
	*(board + i + j*BOARD_WIDTH) = 2;
	current_score = BETA * (alpha_beta_scoring(board, alpha, &beta_current, 						   depth+1));
	if (current_score < beta_current)
	  beta_current = current_score;
	if (current_score < *alpha){
	  i = BOARD_WIDTH;
	  j = BOARD_LENGTH;
	  break;
	}

	*(board + i + j*BOARD_WIDTH) = 4;
	current_score = BETA * (alpha_beta_scoring(board, alpha, &beta_current, 						   depth+1));
	if (current_score < beta_current)
	  beta_current = current_score;
	if (current_score < *alpha){
	  i = BOARD_WIDTH;
	  j = BOARD_LENGTH;
	  break;
	}

      }
    }
  }

  if (beta_current == FLT_MAX)
    beta_current = FLT_MIN;

  if (*alpha < beta_current)
    *alpha = beta_current;

  return beta_current;
}

int
get_max(int *board)
{
  int board_max = -1;
  int i, j;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (board_max < *(board + i + j*BOARD_WIDTH))
	board_max = *(board + i + j*BOARD_WIDTH);
    }
  }
  return board_max;
}

void
printb(int *board)
{
  int i, j;
  //printf("Current Board:\n");
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_LENGTH; i++){
      printf("%d\t\t", *(board + j*BOARD_WIDTH + i));
    }
    printf("\n");
  }
}


float
min_score(float *scores, int length)
{
  int i;
  float min = FLT_MAX;
  for (i = 0; i < length; i++){
    if (scores[i] < min)
      min = scores[i];
  }
  return(min);
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

/* Returns 1 if board and check_board are the same, otherwise 0 */
int
same_board(int *board, int *check_board)
{
  int i, j;
  int same = 1;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + BOARD_WIDTH*j + i) != *(check_board + BOARD_WIDTH*j + i)){
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
  int i, j, m, l;
  int score = 0;
  int number_tiles = 0;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + BOARD_WIDTH*j) != 0){
	number_tiles++;
	score += pow(*(board + i + BOARD_WIDTH*j), 2);
      }
    }
  }

  float distance_score = 0;
  for (j = 0; j < BOARD_LENGTH; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(board + i + BOARD_WIDTH*j) != 0){
  	for (m = j; m < BOARD_LENGTH; m++){
  	  for (l = i+1; l < BOARD_WIDTH; l++){
  	    /* if (*(board + l + BOARD_WIDTH*m) != 0){ */
	    if (*(board + l + BOARD_WIDTH*m) == *(board + i + BOARD_WIDTH*j)){
	      distance_score += ((float)(*(board + l + BOARD_WIDTH*m) + *(board + i + BOARD_WIDTH*j)))/((float)((abs(j-m) + abs(i-l))));
  	    }
  	  }
  	}
      }
    }
  }

  /* int empty = get_num_empty(board); */
  return(((float)score + distance_score)/((float)number_tiles));
  //  return(((float)score)/((float)pow(number_tiles, 2)));
}

/* Changes state of current board by moving all blocks in direction of move*/
void
move(int *current_board, int move)
{

  int i, j, stable, h, temp;  

  /* Transform board matrix to simplify fill and join */
  if (move == UP_MOVE){
    ;
  }

  if (move == LEFT_MOVE || move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = j; i < BOARD_WIDTH; i++){
	temp = *(current_board + BOARD_LENGTH*j + i);
        *(current_board + BOARD_LENGTH*j + i) = *(current_board + BOARD_LENGTH*i + j);
	*(current_board + BOARD_LENGTH*i + j) = temp;
      }
    }
  }

  if (move == DOWN_MOVE || move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH/2; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	temp = *(current_board + BOARD_LENGTH*j + i);
	*(current_board + BOARD_LENGTH*j + i) = *(current_board + BOARD_LENGTH*(BOARD_LENGTH - 1 - j) + i);
	*(current_board + BOARD_LENGTH*(BOARD_LENGTH - 1 - j) + i) = temp;

      }
    }
  }


  /* First fill up empty spaces */
  for (i = 0; i < BOARD_WIDTH; i++){
    stable = 0;
    while (!stable){
      for (j = 0; j < BOARD_LENGTH; j++){
	if (j == BOARD_LENGTH-1){
	  stable = 1;
	  break;
	}	
	if (*(current_board + BOARD_LENGTH*j + i) == 0 && 
	    *(current_board + BOARD_LENGTH*(j + 1) + i) != 0){
	  *(current_board + BOARD_LENGTH*j + i) = *(current_board + BOARD_LENGTH*(j + 1) + i);
	  *(current_board + BOARD_LENGTH*(j + 1) + i) = 0;
	  break;
	}
      }
    }
  }

  /* Now join like numbers */
  for (j = 0; j < BOARD_LENGTH - 1; j++){
    for (i = 0; i < BOARD_WIDTH; i++){
      if (*(current_board + BOARD_LENGTH*j + i) != 0 && 
	  *(current_board + BOARD_LENGTH*j + i) == *(current_board + BOARD_LENGTH*(j + 1) + i)){

	/* Double block value */
	*(current_board + BOARD_LENGTH*j + i) = 2*(*(current_board + BOARD_LENGTH*j + i));
	*(current_board + BOARD_LENGTH*(j + 1) + i) = 0;

	/* Push up numbers below */
	for (h = j + 1; h < BOARD_LENGTH; h++){
	  if (h == 3)
	    *(current_board + BOARD_LENGTH*h + i) = 0;
	  else
	    *(current_board + BOARD_LENGTH*h + i) = *(current_board + BOARD_LENGTH*(h + 1) + i);
	}
      }
    }
  }

  /* Transform board matrix to simplify fill and join */
  if (move == UP_MOVE){
    ;
  }

  if (move == DOWN_MOVE || move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH/2; j++){
      for (i = 0; i < BOARD_WIDTH; i++){
	temp = *(current_board + BOARD_LENGTH*j + i);
	*(current_board + BOARD_LENGTH*j + i) = *(current_board + BOARD_LENGTH*(BOARD_LENGTH - 1 - j) + i);
	*(current_board + BOARD_LENGTH*(BOARD_LENGTH - 1 - j) + i) = temp;

      }
    }
  }

  if (move == LEFT_MOVE || move == RIGHT_MOVE){
    for (j = 0; j < BOARD_LENGTH; j++){
      for (i = j; i < BOARD_WIDTH; i++){
	temp = *(current_board + BOARD_LENGTH*j + i);
	*(current_board + BOARD_LENGTH*j + i) = *(current_board + BOARD_LENGTH*i + j);
	*(current_board + BOARD_LENGTH*i + j) = temp;
      }
    }
  }
}
