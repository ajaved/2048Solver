#include "Python.h"
#include "scoring_board.h"

#define UP_MOVE 0
#define DOWN_MOVE 1
#define RIGHT_MOVE 2
#define LEFT_MOVE 3


int
score_board(int **board, int x_dim, int y_dim){
  int i, j;
  printf("Received Board:\n");
  for (i = 0; i < x_dim; i++){
    for (j = 0; j < y_dim; j++){
      printf("%d\t", board[i][j]);
    }
    printf("\n");
  }

  return 1;
}

void
move(int **board, int move){
        
/*   int **new_board = malloc( */
/* new_board = GameBoard(deepcopy(self.current_state)) */

/* # Transform board matrix to simplify fill and join */
/*     if move == UP_MOVE: */
/*   pass */
            
/*     if move == DOWN_MOVE:  */
/*   for j in range(GameBoard.size):  */
/*   for i in range(GameBoard.size):  */
/*   new_board.current_state[GameBoard.size - 1 - j][i] = self.current_state[j][i] */

/*     if move == RIGHT_MOVE:  */
/*   for j in range(GameBoard.size):  */
/*   for i in range(GameBoard.size):  */
/*   new_board.current_state[GameBoard.size - 1 - i][j] = self.current_state[j][i] */

/*     if move == LEFT_MOVE: */
/*   for j in range(GameBoard.size): */
/*   for i in range(GameBoard.size): */
/*   new_board.current_state[i][j] =  self.current_state[j][i] */

/* # First fill up empty spaces */
/*     for i in range(GameBoard.size): */
/*   stable = False */
/*     while not stable: */
/*   for j in range(GameBoard.size): */
/*   if j == 3: */
/*   stable = True */
/* #print('Achieved stability') */
/*     break */
/*     if new_board.current_state[j][i] == 0 and new_board.current_state[j + 1][i] != 0: */
/*   new_board.current_state[j][i] = new_board.current_state[j + 1][i] */
/*     new_board.current_state[j + 1][i] = 0 */
/*     break */
/* # print('Board after filling spaces: ') */
/* # print (new_board.current_state) */

/* # Now join like numbers */
/*     for j in range(GameBoard.size - 1): */
/*   for i in range(GameBoard.size): */
/*   if new_board.current_state[j][i] != 0 and new_board.current_state[j][i] == new_board.current_state[j + 1][i]: */
/* # Double block value */
/*   new_board.current_state[j][i] = 2*new_board.current_state[j][i] */
/*     new_board.current_state[j + 1][i] = 0 */
/* # Push up numbers below */
/*     for h in range(j + 1, GameBoard.size): */
/*   if h == 3: */
/*   new_board.current_state[h][i] = 0 */
/*   else: */
/*     new_board.current_state[h][i] = new_board.current_state[h + 1][i] */
        
/* # Undo matrix transform to correct board orientation */
/*       if move == UP_MOVE: */
/*   self.current_state = new_board.current_state */
            
/*     if move == DOWN_MOVE:  */
/*   for j in range(GameBoard.size):  */
/*   for i in range(GameBoard.size):  */
/*   self.current_state[GameBoard.size - 1 - j][i] = new_board.current_state[j][i] */

/*     if move == RIGHT_MOVE:  */
/*   for j in range(GameBoard.size):  */
/*   for i in range(GameBoard.size):  */
/*   self.current_state[i][j] = new_board.current_state[GameBoard.size - 1 - j][i] */

/*     if move == LEFT_MOVE: */
/*   for j in range(GameBoard.size): */
/*   for i in range(GameBoard.size): */
/*   self.current_state[i][j] =  new_board.current_state[j][i] */

}
