#! /usr/bin/python

from __future__ import division
import sys
import re
import _scoring_board as c_score
import numpy
#from math import log
from time import sleep
from time import clock
from random import randint
from copy import deepcopy
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support import expected_conditions as EC

# Global mapping from move direction to integers
UP_MOVE = 0
DOWN_MOVE = 1
RIGHT_MOVE = 2
LEFT_MOVE = 3

# Frame class that manages opening of 2048 web page, then communicating
# with and extracting game information from page.
class GameFrame:

    def __init__(self):
        pass

    # Opens Chrome browser and then takes user to 2048 webpage.
    def start(self):
        print '2048 Begins.'
        self.browser = webdriver.Chrome()
        self.browser.implicitly_wait(10)
        self.browser.get('http://gabrielecirulli.github.io/2048/')
        self.elem = self.browser.find_element(By.XPATH, '//body')
        print('Page loaded.')
    
    # Parses board state from webpage and returns matrix (list of lists)
    # representing current board state.
    def get_current_board_state(self):
        html = self.browser.page_source
        regexp = re.compile('tile-(\d+) tile-position-(\d+)-(\d+)')
        board_vals = regexp.findall(html)
        board = [[0]*4, [0]*4, [0]*4, [0]*4]
        for square in board_vals:
            board[int(square[2])-1][int(square[1])-1] = int(square[0])
        return board

    # Sends key press to game webpage
    def key_press(self, key):
        global UP_MOVE
        global DOWN_MOVE
        global RIGHT_MOVE
        global LEFT_MOVE

        try:
            # self.elem.click()
            if key == UP_MOVE:
                self.elem.send_keys(Keys.ARROW_UP)
            if key == DOWN_MOVE:
                self.elem.send_keys(Keys.ARROW_DOWN)
            if key == RIGHT_MOVE:
                self.elem.send_keys(Keys.ARROW_RIGHT)
            if key == LEFT_MOVE:
                self.elem.send_keys(Keys.ARROW_LEFT)
        except:
            print "Unexpected error:", sys.exc_info()[0]
            raise

    # Ends game by exiting out of browser session.
    def end(self):
        self.browser.quit()

# Board class represents the 2048 game board.
class GameBoard:
    
    # Length/width of board, in number of blocks
    size = 4;

    def __init__(self, initial_board=[[0]*4, [0]*4, [0]*4, [0]*4]):
        self.current_state = initial_board

    # Updates the board game with new state
    def update_board(self, current_board):
        self.current_state = current_board

# AiController class manages entire program, from 
# beginning a browser session to reading board states and
# calculating best next moves.
class GameAiController:

    def __init__(self):
        self.frame = GameFrame()
        self.board = GameBoard()
        self.num_attempts = 0
        self.num_wins = 0

    # Starts the browser session and begins playing game.
    def start(self):
        self.frame.start()
        self.play_game()

    # Executes move after move in hope of attaining 2048
    def play_game(self):
        while (1):
            while self.max_tile(self.board) < 2048: 
                sleep(0.2)
                #print('Board before move:')
                #print(self.board.current_state)

                self.update_board()
            
                #print('Board after move:')
                #print(self.board.current_state)
            
                next_move = self.calculate_next_move(3)
                if next_move == 5:
                    # Game over. Restart?
                    self.num_attempts += 1
                    print('YOU LOST =(')
                    print('Number of attempts so far: ' + str(self.num_attempts))
                    print('Number of wins so far: ' + str(self.num_wins))
                    self.frame.elem.click()
                    continue
                self.send_move_to_game(next_move)
                
                # if next_move == -sys.maxint:
                #     # User lost
                #     print('YOU LOST!')
                #     self.frame.end()
                # sleep(1)
                # print('Move: ' + str(next_move))
                # self.send_move_to_game(next_move)
            
            # User won
            self.num_attempts += 1
            self.num_wins += 1
            print('Number of attempts so far: ' + str(self.num_attempts))
            print('Number of wins so far: ' + str(self.num_wins))
                    
            self.frame.elem.click()
            print('YOU WON!')
            # self.frame.end()

    # Updates board state with the new one in the browser
    def update_board(self):
        current_board = deepcopy(self.board)
        while current_board.current_state == self.board.current_state:
            current_board.update_board(self.frame.get_current_board_state())
        self.board = current_board

    # Calls C function to calculate next best move
    def calculate_next_move(self, depth):
        numpy_board = numpy.array(self.board.current_state)
        numpy_board = numpy_board.astype(numpy.int32, copy=False)
        return c_score.score_board(int(depth), numpy_board)

    # Returns maximum value on board, to be used to check whether
    # user has won yet or not.
    def max_tile(self, board):
        max = 0
        for j in range(GameBoard.size):
            for i in range (GameBoard.size):
                if board.current_state[j][i] > max:
                    max = board.current_state[j][i]
        return max

    def send_move_to_game(self, move):
        self.frame.key_press(move)

def check_full(board):
    check = 1;
    for i in range(4):
        for j in range(4):
            if board[i][j] == 0:
                check = 0
                break
        if check == 0:
            break
    return check

if __name__ == '__main__':

    # Initialize game model
    game_controller = GameAiController()

    # Begin playing
    game_controller.start()
