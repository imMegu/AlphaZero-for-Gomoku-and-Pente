#pragma once
#include "Bitboard.h"

template <uint S> struct PenteBoard : public Bitboard<S> {
public:
  int captures1 = 0; // Captures for player 1
  int captures2 = 0; // Captures for player 2

  // Constructor
  PenteBoard() : Bitboard<S>() {}

  bool win(bool p) override {

    if (p == 0 && captures1 >= 5)
      return 1;
    if (p == 1 && captures2 >= 5)
      return 1;

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

    return five_in_a_row;
  }

  bool make_move(int pos) override {
    // Place piece using Bitboard logic
    bool valid = Bitboard<S>::make_move(pos);

    if (valid) {
      // Note: Bitboard::make_move flips 'player'. If we want the *current*
      // player, we need to use '!player' right after the call to
      // Bitboard::make_move(pos).

      auto &B1 = !this->player ? this->player2 : this->player1;
      auto &B2 = !this->player ? this->player1 : this->player2;

      int directions[4] = {1, S + 1, S, S + 2};

      for (int dir : directions) {
        // Both directions along same axis
        for (int sign : {-1, 1}) {
          int offset = sign * dir;

          // Check if positions are valid
          int pos1 = pos + offset;
          int pos2 = pos + 2 * offset;
          int pos3 = pos + 3 * offset;

          // Check if any are in padding (invalid)
          if ((pos1 % (S + 1) == S) || (pos2 % (S + 1) == S) ||
              (pos3 % (S + 1) == S))
            continue;

          // Check if its within limits
          if ((pos1 < 0) || (pos2 < 0) || (pos3 < 0))
            continue;

          // + 2 because max playable position is S * (S+1) - 2
          const int max_board_size = (int)(S * (S + 1));
          if ((pos1 + 2 > max_board_size) || (pos2 + 2 > max_board_size) ||
              (pos3 + 2 > max_board_size))
            continue;

          // Check A-B-B-A pattern
          if (B2.test(pos1) && B2.test(pos2) && B1.test(pos3)) {
            // Capture! Remove the two opponent pieces
            B2.reset(pos1);
            B2.reset(pos2);
            if (!this->player == 0)
              captures1++;
            else
              captures2++;
          }
        }
      }
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
