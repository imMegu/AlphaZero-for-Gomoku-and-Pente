#pragma once
#include "Bitboard.h"

template <uint S> struct GomokuBoard : public Bitboard<S> {
public:
  // Constructor
  GomokuBoard() : Bitboard<S>() {}

  bool win(bool p) override {
    auto check_five_in_a_row = [&](const auto &b, int shift) {
      auto y = b & (b >> shift);
      auto z = y & (y >> shift);
      auto w = z & (z >> shift);
      return (w & (w >> shift)).any();
    };

    const auto &board = p ? this->player2 : this->player1;

    return check_five_in_a_row(board, 1) ||                // Horizontal
           check_five_in_a_row(board, (this->SIDE + 1)) || // Vertical
           check_five_in_a_row(board, this->SIDE) ||       // Diagonal /
           check_five_in_a_row(board, (this->SIDE + 2));   // Diagonal \ /
  }

  // bool make_move(int pos) is inherited from Bitboard

  GomokuBoard copy() const {
    GomokuBoard b;
    b.player1 = this->player1;
    b.player2 = this->player2;
    b.player = this->player;
    return b;
  }
};
