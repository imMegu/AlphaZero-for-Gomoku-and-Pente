import bot
import random

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
                self.board_size,
                player1,
                player2,
                turn_number
            )
            # Convert index back to (x, y) coordinates
            print(move)
            row = (self.board_size - 1 ) - (move // (self.board_size + 1))
            col = move % (self.board_size + 1)
            return col, row
        except Exception as e:
            print(f"Error calling C++ MCTS search: {e}")
            return -1, -1

bsize = 7
board = [[0 for _ in range(bsize)] for _ in range(bsize)]

# 1110 0000 1000
# 0000 0000 0010

turn = 0

p = Player(0, bsize)
for _ in range(int(bsize * bsize / 2)):

    #bot play
    while True:
        col, row = p.play(board, turn, None)
        print(f"Best move: ({col}, {row})")
        if board[row][col] == 0: # bot is always player1
            board[row][col] = 1
            turn += 1
            break;

    # random play 
    while True:
        row = random.randrange(bsize)
        col = random.randrange(bsize)
        if board[row][col] == 0:
            board[row][col] = 2
            turn += 1
            break
