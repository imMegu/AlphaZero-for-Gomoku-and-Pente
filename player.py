import subprocess
import os
import sys
import random
import time
# The 'bot' module import is moved to after the compilation check.

# --- C++ Module Compilation Check ---

MODULE_NAME = "bot"
BUILD_DIR = "build"

try:
    # 1. Ensure build directory exists
    os.makedirs(BUILD_DIR, exist_ok=True)
    
    start_time = time.time()
    
    print(f"Running: cmake -S . -B {BUILD_DIR}")
    subprocess.run(
        ["cmake", "-S", ".", "-B", BUILD_DIR],
        check=True, # Raise exception on error
        capture_output=False, 
        text=True
    )

    print(f"Running: cmake --build {BUILD_DIR}")
    subprocess.run(
        ["cmake", "--build", BUILD_DIR],
        check=True, # Raise exception on error
        capture_output=False,
        text=True
    )
    
    end_time = time.time()
    print(f"Compilation process finished in {end_time - start_time:.2f} seconds.")

except subprocess.CalledProcessError as e:
    print("\n--- COMPILATION FAILED ---", file=sys.stderr)
    print("Could not compile the C++ extension. Please check C++ code and CMakeLists.txt.", file=sys.stderr)
    sys.exit(1)
except Exception as e:
    print(f"An unexpected error occurred during build setup: {e}", file=sys.stderr)
    sys.exit(1)

import bot

class Player:
    def __init__(self, rules, board_size):
        self.rules = rules
        self.board_size = board_size
    
    def play(self, board, turn_number, last_opponent_move):
        player1 = ""
        player2 = ""

        for y in range(self.board_size):
            # Add padding before every line
            player1 += "0"
            player2 += "0" 
            for x in range(self.board_size - 1, -1, -1):
                if (board[y][x] == 0):
                    player1 += "0"
                    player2 += "0"
                elif (board[y][x] == 1): 
                    player1 += "1"
                    player2 += "0"
                elif (board[y][x] == 2): 
                    player1 += "0"
                    player2 += "1"

        # Call the C++ function
        # The C++ function returns the chosen move index
        try:
            move = bot.run_mcts_search(
                self.rules,
                self.board_size,
                player1,
                player2,
                turn_number
            )
            # Convert index back to (x, y) coordinates
            if move == -1:
                return -1, -1

            row = (self.board_size - 1 ) - (move // (self.board_size + 1))
            col = move % (self.board_size + 1)
            return col, row

        except Exception as e:
            print(f"Error calling C++ MCTS search: {e}")
            return -1, -1


'''
bsize = 5
board = [[0 for _ in range(bsize)] for _ in range(bsize)]

# 1110 0000 1000
# 0000 0000 0010

turn = 0
gameover = False

p = Player(1, bsize)
for _ in range(int(bsize * bsize)):

    # bot play
    while True:
        col, row = p.play(board, turn, None)
        print(f"Best move: ({col}, {row})")

        if col == row == -1:
            gameover = True
            break

        if board[row][col] == 0: # bot is always player1
            board[row][col] = 1
            turn += 1
            break

    if gameover:
        break

    # random play 
    while True:
        if turn == 1: 
            board[2][2] = 2
            turn += 1
            break
        elif turn == 3:
            board[0][2] = 2
            turn += 1
            break
        elif turn == 5:
            board[4][2] = 2 
            turn += 1
            break
        elif turn == 7:
            board[2][0] = 2
            turn += 1
            break
        elif turn == 9:
            board[2][4] = 2
            turn += 1
            break
        else:
            row = random.randrange(bsize)
            col = random.randrange(bsize)
            if board[row][col] == 0:
                board[row][col] = 2
                turn += 1
                break
'''
