#include <bits/stdc++.h>
#include "MCTS.h"
#include "GomokuBoard.h"
#include "PenteBoard.h"

using namespace std;

void test1() {
  MCTS<GomokuBoard<15>> bot(100000u);

  string p1 = "0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000100000000000000000000000000100000000000100"
              "000000000000000000000001000011110000000001010110";

  string p2 = "0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000000000000000000000000000000"
              "0000000000000000000000000000000000000001110000000000000000000000"
              "000000000001000100000000001000001000000010101001";

  bot.get_board().setPlayer1(p1);
  bot.get_board().setPlayer2(p2);
  bot.set_moves(23);
  /*
  bot.get_board().print();
  cout << (bot.get_board().win(0) || bot.get_board().win(1)) << endl;
  bot.make_move(20);
  bot.get_board().print();
  cout << (bot.get_board().win(0) || bot.get_board().win(1)) << endl;
  */
  int play = bot.search(true);
  cout << "BOARD BEFORE MAKING MOVE:" << endl;
  bot.get_board().printNumbered();

  bot.make_move(play);

  cout << "DID THE GAME END AFTER MAKING THE MOVE " << play << "? "
       << ((bot.get_board().is_over()) ? "yes" : "no") << endl;
}

void test2() {
  MCTS<GomokuBoard<32>> bot(10000u);

  constexpr uint BOARDSIZE = GomokuBoard<32>::BOARDSIZE;
  constexpr uint SIDE = GomokuBoard<32>::SIDE;

  while (bot.get_moves() < 8 * SIDE) {
    int randomNumber = rand() % (BOARDSIZE);
    if (!bot.get_board().occupied(randomNumber) &&
        randomNumber % (SIDE + 1) != SIDE) {
      bot.make_move(randomNumber);
    }
  }

  int play = bot.search(true);
  if (play == -1)
    return;
  cout << "BOARD BEFORE MAKING MOVE:" << endl;
  bot.get_board().printNumbered();

  bot.make_move(play);
  cout << "DID THE GAME END AFTER MAKING THE MOVE " << play << " ?"
       << ((bot.get_board().is_over()) ? "yes" : "no") << endl;
}

void test3() { // test if board is correctly evaluating wins

  while (true) {
    MCTS<PenteBoard<25>> bot(10000u);

    constexpr uint BOARDSIZE = PenteBoard<25>::BOARDSIZE;
    constexpr uint SIDE = PenteBoard<25>::SIDE;

    while (bot.get_moves() < BOARDSIZE - SIDE) {
      int randomNumber = rand() % (BOARDSIZE);
      if (!bot.get_board().occupied(randomNumber) &&
          randomNumber % (SIDE + 1) != SIDE) {
        bot.make_move(randomNumber);
      }
    }
    // Find finished game
    if (bot.get_board().win(0)) {
      bot.get_board().printPlayer(0);
      cout << bot.get_board().getBit1() << endl;
      break;
    } else if (bot.get_board().win(1)) {
      bot.get_board().printPlayer(1);
      cout << bot.get_board().getBit2() << endl;
      break;
    }
  }
}

void test4() {
  int i = 0;
  while (true) {
    i++;
    MCTS<GomokuBoard<25>> bot(10000u);

    constexpr uint BOARDSIZE = GomokuBoard<25>::BOARDSIZE;
    constexpr uint SIDE = GomokuBoard<25>::SIDE;

    while (bot.get_moves() < 500) {
      int randomNumber = rand() % (BOARDSIZE);
      if (!bot.get_board().occupied(randomNumber) &&
          randomNumber % (SIDE + 1) != SIDE) {
        bot.make_move(randomNumber);
      }
    }

    int play = bot.search(false);
    if (play == 0)
      return;
    cout << i << endl;
  }
}

void test5() { // Filled out board should still have padding zeroes.
  MCTS<GomokuBoard<25>> bot(10000u);

  constexpr uint B = GomokuBoard<25>::BOARDSIZE;
  // constexpr uint S = GomokuBoard<25>::SIDE;

  for (uint i = 0; i < B; i++)
    bot.make_move(i);

  bot.get_board().print();
  cout << bot.get_board().getBit1() << endl << endl;
  cout << bot.get_board().getBit2() << endl << endl;
  cout << (bot.get_board().getBit1() | bot.get_board().getBit2()) << endl;

  cout << "Win? " << (bot.get_board().is_over() ? "yes" : "no") << endl;
}

void test6() {
  MCTS<PenteBoard<6>> bot(100000u);

  while (!bot.get_board().is_over()) {
    int move = bot.search(true);
    bot.make_move(move);
    bot.get_board().printNumbered();
  }

  bot.get_board().printNumbered();
  if (bot.get_board().win(0))
    cout << "Player1 won." << endl;
  else if (bot.get_board().win(1))
    cout << "Player2 won." << endl;
  else
    cout << "Tie." << endl;
}

void test7() {
  MCTS<GomokuBoard<7>> bot(100000u);

  int hmove;

  while (true) {
    int move = bot.search(true);
    bot.make_move(move);
    bot.get_board().printNumbered();

    if (bot.get_board().is_over())
      break;

    cout << "Enter your move: ";
    cin >> hmove;
    bool valid = bot.make_human_move(hmove);
    while (!valid) {
      cout << "Invalid Move, try again: ";
      cin >> hmove;
      valid = bot.make_human_move(hmove);
    }

    if (bot.get_board().is_over())
      break;
  }

  bot.get_board().printNumbered();
  if (bot.get_board().win(0))
    cout << "Player1 won." << endl;
  else if (bot.get_board().win(1))
    cout << "Player2 won." << endl;
  else
    cout << "Tie." << endl;
}

void play() {

  MCTS<GomokuBoard<3>> bot(100000u);

  string p1 = "011100000001";

  string p2 = "000000000100";

  bot.get_board().setPlayer1(p1);
  bot.get_board().setPlayer2(p2);
  bot.set_moves(5);
  bot.get_board().printNumbered();
}

void hvh() {
  MCTS<PenteBoard<5>> bot(100000u);

  int hmove;

  while (true) {
    cout << "Enter your move: ";
    cin >> hmove;
    bool valid = bot.make_human_move(hmove);
    while (!valid) {
      cout << "Invalid Move, try again: ";
      cin >> hmove;
      valid = bot.make_human_move(hmove);
    }

    bot.get_board().printNumbered();
    cout << "P1 captures: " << bot.get_board().captures1 << endl;
    cout << "P2 captures: " << bot.get_board().captures2 << endl;

    if (bot.get_board().is_over())
      break;
  }
}

int main() {

  srand(time({})); // Seed the RNG with current time

  test6();

  return 0;
}
