#pragma once
#include <bitset>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

template <uint S> struct Bitboard {
public:
  static constexpr uint SIDE = S;
  static constexpr uint BOARDSIZE = (SIDE * (SIDE + 1));

  bitset<BOARDSIZE> player1;
  bitset<BOARDSIZE> player2;
  bool player;

  Bitboard() : player(false) {}
  virtual ~Bitboard() = default;

  // Prevent accidental copying (use copy() method instead)
  Bitboard(const Bitboard &) = default;
  Bitboard &operator=(const Bitboard &) = default;

  static constexpr int index(int r, int c) { return r * (SIDE + 1) + c; }

  void setPlayer1(string b) {
    bitset<BOARDSIZE> b1(b);
    player1 = b1;
  }
  void setPlayer2(string b) {
    bitset<BOARDSIZE> b2(b);
    player2 = b2;
  }

  bitset<BOARDSIZE> getBit1() { return player1; }
  bitset<BOARDSIZE> getBit2() { return player2; }

  virtual bool win(bool p) = 0;

  bool is_over() {
    return (count(0) + count(1) >= SIDE * SIDE) || win(0) || win(1);
  }

  virtual bool make_move(int pos) {
    bool valid = !occupied(pos) & (pos % (SIDE + 1) != SIDE);
    if (valid) {
      if (player) {
        player2.set(pos);
      } else {
        player1.set(pos);
      }
      player = !player;
    }
    return valid;
  }

  // Return true if pos has player1 or 2 piece in there
  bool occupied(int pos) const {
    return player2.test(pos) || player1.test(pos);
  }

  size_t count(bool p) const {
    if (p)
      return player2.count();
    return player1.count();
  }

  void printPlayer(bool p) {
    for (int r = SIDE - 1; r >= 0; --r) {
      for (uint c = 0; c < SIDE; ++c) {
        int idx = r * (SIDE + 1) + c;
        if ((p == 0) && player1.test(idx))
          cout << "O ";
        else if ((p == 1) && player2.test(idx))
          cout << "X ";
        else
          cout << ". ";
      }
      cout << "\n";
    }
  }

  void print() {
    for (int r = SIDE - 1; r >= 0; --r) {
      for (uint c = 0; c < SIDE; ++c) {
        int idx = r * (SIDE + 1) + c;
        if (player1.test(idx))
          cout << "O ";
        else if (player2.test(idx))
          cout << "X ";
        else
          cout << ". ";
      }
      cout << "\n";
    }
  }

  void printNumbered() {

    // To print according to board size
    int magnitude = floor(log10(SIDE * SIDE));

    for (int r = SIDE - 1; r >= 0; --r) {

      for (uint i = 0; i < SIDE; i++) {
        cout << '+';
        for (int j = 0; j < magnitude + 1; j++)
          cout << '-';
      }
      cout << '+' << endl;

      for (uint c = 0; c < SIDE; ++c) {

        int idx = r * (SIDE + 1) + c;
        cout << '|';
        if (player1.test(idx)) {
          for (int i = 0; i < magnitude + 1; i++) {
            cout << "\033[41m \033[0m";
          }
        } else if (player2.test(idx)) {
          for (int i = 0; i < magnitude + 1; i++) {
            cout << "\033[43m \033[0m";
          }
        } else {
          int idx = index(r, c);
          for (int i = magnitude; i > 0; i--) {
            if (idx < pow(10, i))
              cout << '0';
          }
          cout << idx;
        }
      }
      cout << "|\n";
    }
    for (uint i = 0; i < SIDE; i++) {
      cout << '+';
      for (int j = 0; j < magnitude + 1; j++)
        cout << '-';
    }
    cout << '+' << endl;
  }
};
