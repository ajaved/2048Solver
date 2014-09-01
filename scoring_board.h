float alpha_beta_scoring(int *board, float *alpha, float *beta);
float score_board(int *board, int x_dim, int y_dim, int depth);
float opponent_move(int *board, float *alpha, float *beta);
float get_current_score(int *board);
void move(int *board, int move);
int get_num_empty(int *board);
int same_board(int *board, int*check_board);
