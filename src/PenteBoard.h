#pragma once
#include "Bitboard.h"

template <uint S> struct PenteBoard : public Bitboard<S> {
public:
  int captures1 = 0; // Captures for player 1
  int captures2 = 0; // Captures for player 2

  // Constructor
  PenteBoard() : Bitboard<S>() {}

  bool win(bool p) override {
    auto check_five_in_a_row = [&](const auto &b, int shift) {
      auto y = b & (b >> shift);
      auto z = y & (y >> shift);
      auto w = z & (z >> shift);
      return (w & (w >> shift)).any();
    };

    const auto &board = p ? this->player2 : this->player1;

    bool five_in_a_row =
        check_five_in_a_row(board, 1) ||                // Horizontal
        check_five_in_a_row(board, (this->SIDE + 1)) || // Vertical
        check_five_in_a_row(board, this->SIDE) ||       // Diagonal /
        check_five_in_a_row(board, (this->SIDE + 2));   // Diagonal \ /

    // TODO: Implement check capture

    bool captured_enough = p ? (captures2 >= 5) : (captures1 >= 5);

    return five_in_a_row || captured_enough;
  }

  bool make_move(int pos) override {
    // Place piece using Bitboard logic
    bool valid = Bitboard<S>::make_move(pos);

    if (valid) {
      // The 'player' flag in Bitboard has already been flipped after
      // Bitboard::make_move. The current piece placed belongs to player 'p'
      // where p = !player.

      // TODO: Implement capture logic here.
      // - Check for P-E-E-P pattern around 'pos'
      // - If capture, clear opponent bits and increment current player's
      // capture count. Example: if (current player is 0 and made capture) {
      // captures1++; }

      // Note: Bitboard::make_move flips 'player'. If we want the *current*
      // player, we need to use '!player' right after the call to
      // Bitboard::make_move(pos).
    }
    return valid;
  }

  PenteBoard copy() const {
    PenteBoard b;
    b.player1 = this->player1;
    b.player2 = this->player2;
    b.player = this->player;
    b.captures1 = this->captures1;
    b.captures2 = this->captures2;
    return b;
  }
};
