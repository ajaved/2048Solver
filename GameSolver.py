#! /usr/bin/python
from __future__ import division
import sys
import re
from math import log
from time import sleep
from random import randint
from copy import deepcopy
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC

UP_MOVE = 0
DOWN_MOVE = 1
RIGHT_MOVE = 2
LEFT_MOVE = 3

class GameFrame:

    def __init__(self):
        pass

    def start(self):
        print '2048 Begins.'
        self.browser = webdriver.Firefox()
        self.browser.implicitly_wait(10)
        self.browser.get('http://gabrielecirulli.github.io/2048/')
        self.elem = self.browser.find_element_by_xpath('/html')
        print('Page loaded.')

    def get_current_board_state(self):
        html = self.browser.page_source
        regexp = re.compile('tile-(\d+) tile-position-(\d+)-(\d+)')
        board_vals = regexp.findall(html)
        board = [[0]*4, [0]*4, [0]*4, [0]*4]
        for square in board_vals:
           # print('%d %d %d', int(square[2]), int(square[1]), int(square[0]))
            board[int(square[2])-1][int(square[1])-1] = int(square[0])
        return board

    def key_press(self, key):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        if key == UP_MOVE:
            self.elem.send_keys(Keys.ARROW_UP)
        if key == DOWN_MOVE:
            self.elem.send_keys(Keys.ARROW_DOWN)
        if key == RIGHT_MOVE:
            self.elem.send_keys(Keys.ARROW_RIGHT)
        if key == LEFT_MOVE:
            self.elem.send_keys(Keys.ARROW_LEFT)

    def end(self):
        self.browser.quit()

class GameBoard:
    
    # Length/width of board, in number of blocks
    size = 4;

    def __init__(self, initial_board=[[0]*4, [0]*4, [0]*4, [0]*4]):
        self.current_state = initial_board

    def update_board(self, current_board):
        self.current_state = current_board

    def make_move(self, move):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE
        
        # print('Move is: ' + str(move));
        # print('Board before move:')
        # print(self.current_state)
        new_board = GameBoard(deepcopy(self.current_state))

        # Transform board matrix to simplify fill and join
        if move == UP_MOVE:
            pass
            
        if move == DOWN_MOVE: 
            for j in range(GameBoard.size): 
                for i in range(GameBoard.size): 
                    new_board.current_state[GameBoard.size - 1 - j][i] = self.current_state[j][i]

        if move == RIGHT_MOVE: 
            for j in range(GameBoard.size): 
                for i in range(GameBoard.size): 
                    new_board.current_state[GameBoard.size - 1 - i][j] = self.current_state[j][i]

        if move == LEFT_MOVE:
            for j in range(GameBoard.size):
                for i in range(GameBoard.size):
                    new_board.current_state[i][j] =  self.current_state[j][i]

        # First fill up empty spaces
        for i in range(GameBoard.size):
            stable = False
            while not stable:
                for j in range(GameBoard.size):
                    if j == 3:
                        stable = True
                        #print('Achieved stability')
                        break
                    if new_board.current_state[j][i] == 0 and new_board.current_state[j + 1][i] != 0:
                        new_board.current_state[j][i] = new_board.current_state[j + 1][i]
                        new_board.current_state[j + 1][i] = 0
                        break
        # print('Board after filling spaces: ')
        # print (new_board.current_state)

        # Now join like numbers
        for j in range(GameBoard.size - 1):
            for i in range(GameBoard.size):
                if new_board.current_state[j][i] != 0 and new_board.current_state[j][i] == new_board.current_state[j + 1][i]:
                    # Double block value
                    new_board.current_state[j][i] = 2*new_board.current_state[j][i]
                    new_board.current_state[j + 1][i] = 0
                    # Push up numbers below
                    for h in range(j + 1, GameBoard.size):
                        if h == 3:
                            new_board.current_state[h][i] = 0
                        else:
                            new_board.current_state[h][i] = new_board.current_state[h + 1][i]
        
        # Undo matrix transform to correct board orientation
        if move == UP_MOVE:
            self.current_state = new_board.current_state
            
        if move == DOWN_MOVE: 
            for j in range(GameBoard.size): 
                for i in range(GameBoard.size): 
                    self.current_state[GameBoard.size - 1 - j][i] = new_board.current_state[j][i]

        if move == RIGHT_MOVE: 
            for j in range(GameBoard.size): 
                for i in range(GameBoard.size): 
                    self.current_state[i][j] = new_board.current_state[GameBoard.size - 1 - j][i]

        if move == LEFT_MOVE:
            for j in range(GameBoard.size):
                for i in range(GameBoard.size):
                    self.current_state[i][j] =  new_board.current_state[j][i]
        # print('Board after move:')
        # print(self.current_state)
        # print('Done')

class GameAiController:

    def __init__(self):
        self.frame = GameFrame()
        self.board = GameBoard()
        self.score_function = self.lookahead_scoring
        
    def start(self):
        self.frame.start()
        self.play_game()

    def play_game(self):
        while self.max_tile(self.board) < 2048: 
            sleep(0.8)
            #print('Board before move:')
            #print(self.board.current_state)

            self.update_board()
            
            #print('Board after move:')
            #print(self.board.current_state)

            next_move = self.calculate_next_move()
            if next_move == -sys.maxint:
                # User lost
                print('YOU LOST!')
                self.frame.end()
            # sleep(1)
            print('Move: ' + str(next_move))
            self.send_move_to_game(next_move)
        
        # User won
        print('YOU WON!')
        self.frame.end()

    def update_board(self):
        current_board = deepcopy(self.board)
        while current_board.current_state == self.board.current_state:
            current_board.update_board(self.frame.get_current_board_state())
        self.board = current_board

    def calculate_next_move(self):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        next_boards = [GameBoard(deepcopy(self.board.current_state)), GameBoard(deepcopy(self.board.current_state)), GameBoard(deepcopy(self.board.current_state)), GameBoard(deepcopy(self.board.current_state))]

        next_boards[0].make_move(UP_MOVE)
        next_boards[1].make_move(DOWN_MOVE)
        next_boards[2].make_move(RIGHT_MOVE)
        next_boards[3].make_move(LEFT_MOVE)
        next_scores = []
        for n in range(len(next_boards)):
            # print  (next_boards[n].current_state)
            if next_boards[n].current_state != self.board.current_state:
#                next_scores.append(self.score_function(next_boards[n], [self.board.current_state, next_boards[n].current_state]))
#                next_scores.append(self.short_look_scoring(next_boards[n], self.board))
                next_scores.append(self.minimax_scoring(next_boards[n], 1))
            else:
                next_scores.append(-sys.maxint)
        
        # print('Board before move:')
        # print(self.current_state)

        #print('Next scores: ')
        #print(next_scores)
        if max(next_scores) == -sys.maxint:
            return -sys.maxint
        return next_scores.index(max(next_scores))
        
    def minimax_scoring(self, board, depth):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        max_depth = 3

        if depth == max_depth:
            return self.greedy_scoring(board)

        empty_blocks = self.get_empty_blocks(board)
        next_scores = []
        alpha = 1
        new_depth = depth + 1
        for box in empty_blocks:
            new_board = deepcopy(board)
            new_board.current_state[box[0]][box[1]] = 2

            next_boards = [GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state))]

            next_boards[0].make_move(UP_MOVE)
            next_boards[1].make_move(DOWN_MOVE)
            next_boards[2].make_move(RIGHT_MOVE)
            next_boards[3].make_move(LEFT_MOVE)
            
            for n in range(len(next_boards)):
                if next_boards[n].current_state != board.current_state:
                    next_scores.append(self.greedy_scoring(board) + alpha*self.minimax_scoring(next_boards[n], new_depth))
                else:
                    next_scores.append(-1)
        for box in empty_blocks:
            new_board = deepcopy(board)
            new_board.current_state[box[0]][box[1]] = 4

            next_boards = [GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state)), 
                           GameBoard(deepcopy(new_board.current_state))]

            next_boards[0].make_move(UP_MOVE)
            next_boards[1].make_move(DOWN_MOVE)
            next_boards[2].make_move(RIGHT_MOVE)
            next_boards[3].make_move(LEFT_MOVE)
            
            for n in range(len(next_boards)):
                if next_boards[n].current_state != board.current_state:
                    next_scores.append(self.greedy_scoring(board) + alpha*self.minimax_scoring(next_boards[n], new_depth))
                else:
                    next_scores.append(-1)
        
        return sum(next_scores)/len(next_scores)

    def get_empty_blocks(self, board):
        empty = []
        for j in range(GameBoard.size):
            for i in range(GameBoard.size):
                if board.current_state[j][i] == 0:
                    empty.append((j, i))
        return empty
        
    def lookahead_scoring(self, board, previous_boards):
        # TODO: figure out what to do about last move and
        # correctly account for infinite loops
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        #print('BOARD')
        #print(board.current_state)

        alpha = 0.9

        # Function to determine end of possible recursion
        def check_end():
            tiles = []
            if len(previous_boards) > 20:
                return True
            for j in range(GameBoard.size):
                for i in range(GameBoard.size):
                    if board.current_state[j][i] != 0:
                        if board.current_state[j][i] in tiles:
                            return False
                        tiles.append(board.current_state[j][i])
            return True

        if check_end():
            return self.greedy_scoring(board)
        
        next_boards = [GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state))]

        next_boards[0].make_move(UP_MOVE)
        next_boards[1].make_move(DOWN_MOVE)
        next_boards[2].make_move(RIGHT_MOVE)
        next_boards[3].make_move(LEFT_MOVE)
        next_scores = []
        # opposite_dir = {UP_MOVE:DOWN_MOVE, DOWN_MOVE:UP_MOVE, RIGHT_MOVE:LEFT_MOVE, LEFT_MOVE:RIGHT_MOVE}
        for n in range(len(next_boards)):
            # print  (next_boards[n].current_state)
            if next_boards[n].current_state != board.current_state and next_boards[n].current_state not in previous_boards:
                previous_boards.append(next_boards[n].current_state)
                next_scores.append(self.greedy_scoring(board) + alpha*self.lookahead_scoring(next_boards[n], deepcopy(previous_boards)))
            else:
                next_scores.append(-1)
        
        return sum(next_scores)/len(next_scores)

    def get_number_tiles(self, board):
        number_tiles = 0
        for j in range(GameBoard.size):
            for i in range(GameBoard.size):
                if board.current_state[j][i] != 0:
                    number_tiles += 1
        return number_tiles

    def short_look_scoring(self, board, previous_board):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        if self.get_number_tiles(board) == self.get_number_tiles(previous_board):
            return self.greedy_scoring(board)
        
        next_boards = [GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state)), 
                       GameBoard(deepcopy(board.current_state))]

        next_boards[0].make_move(UP_MOVE)
        next_boards[1].make_move(DOWN_MOVE)
        next_boards[2].make_move(RIGHT_MOVE)
        next_boards[3].make_move(LEFT_MOVE)
        next_scores = []
        alpha = 1
        # opposite_dir = {UP_MOVE:DOWN_MOVE, DOWN_MOVE:UP_MOVE, RIGHT_MOVE:LEFT_MOVE, LEFT_MOVE:RIGHT_MOVE}
        for n in range(len(next_boards)):
            next_scores.append(self.greedy_scoring(board) + alpha*self.short_look_scoring(next_boards[n], board))

        return sum(next_scores)/len(next_scores)

        
    def max_tile(self, board):
        max = 0
        for j in range(GameBoard.size):
            for i in range (GameBoard.size):
                if board.current_state[j][i] > max:
                    max = board.current_state[j][i]
        return max

    def send_move_to_game(self, move):
        self.frame.key_press(move)

    # Scoring algorithms
    def max_scoring(self, board):
        return self.max_tile(board);

    def greedy_scoring(self, board):
        score = 0
        number_tiles = 0
        for j in range(GameBoard.size):
            for i in range(GameBoard.size):
                if board.current_state[j][i] != 0:
                    number_tiles += 1
                    score += board.current_state[j][i]**2 # log(board.current_state[j][i], 2)
        assert number_tiles > 0
        # print(score/number_tiles)
        return score/number_tiles


if __name__ == '__main__':

    # Initialize game model
    game_controller = GameAiController()
    game_controller.start()
        
    # browser.quit()
