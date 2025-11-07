#pragma once
#include "Bitboard.h"
#include <optional>

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

  optional<GomokuBoard> genRandomBoard(int threshold) {
    // Threshold should be 0-100, representing a percentage
    if (threshold < 0 || threshold > 100)
      throw runtime_error("Threshold should be between 0-100%.");

    int possible_moves = S * S;
    int moves_made = 0;

    GomokuBoard newBoard;

    while (threshold > (int)((double(moves_made) / possible_moves) * 100)) {
      int move = rand() % (S * S);
      bool valid = newBoard.make_move(move);
      if (valid)
        moves_made++;
    }

    if (newBoard.is_over())
      return nullopt;

    return newBoard;
  }
};
