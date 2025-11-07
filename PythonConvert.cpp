#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include "src/MCTS.h"
#include "src/GomokuBoard.h"
#include "src/PenteBoard.h"

namespace py = pybind11;

// Wrapper function that takes board_size as a runtime parameter
int run_mcts_search(int rules, int board_size, const std::string &player1,
                    const std::string &player2, int turn_number) {
  // Since board_size is a template parameter, we need to handle different sizes

  if (rules == 0) {
    if (board_size == 15) {
      MCTS<GomokuBoard<15>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      // bot.get_board().printNumbered();
      bot.set_moves(turn_number);
      return bot.search();
    } else if (board_size == 50) {
      MCTS<GomokuBoard<50>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      // bot.get_board().printNumbered();
      bot.set_moves(turn_number);
      return bot.search();
    } else if (board_size == 5) {
      MCTS<GomokuBoard<5>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      bot.get_board().printNumbered();
      // bot.set_moves(turn_number);
      return bot.search();
    } else {
      throw std::runtime_error("Unsupported board size: " +
                               std::to_string(board_size));
    }
  } else if (rules == 1) {
    if (board_size == 15) {
      MCTS<PenteBoard<15>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      // bot.get_board().printNumbered();
      bot.set_moves(turn_number);
      return bot.search();
    } else if (board_size == 50) {
      MCTS<PenteBoard<50>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      // bot.get_board().printNumbered();
      bot.set_moves(turn_number);
      return bot.search();
    } else if (board_size == 5) {
      MCTS<PenteBoard<5>> bot(100000u);
      bot.get_board().setPlayer1(player1);
      bot.get_board().setPlayer2(player2);
      // bot.get_board().printNumbered();
      bot.set_moves(turn_number);
      return bot.search();
    } else {
      throw std::runtime_error("Unsupported board size: " +
                               std::to_string(board_size));
    }
  } else {
    throw std::runtime_error("Unsupported rule (should be 0 or 1): " +
                             std::to_string(board_size));
  }
}

PYBIND11_MODULE(bot, m) {
  m.doc() = "Gomoku MCTS bot interface";

  // clang-format off
  m.def("run_mcts_search", &run_mcts_search,
        "Run MCTS search and return the best move", 
        py::arg("rules"), 
        py::arg("board_size"),
        py::arg("player1"), 
        py::arg("player2"), 
        py::arg("turn_number"));
  // clang-format on
}
