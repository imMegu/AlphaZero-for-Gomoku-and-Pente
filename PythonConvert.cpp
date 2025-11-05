#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include "MCTS.h"
#include "GomokuBoard.h"
#include "PenteBoard.h"

namespace py = pybind11;

// Wrapper function that takes board_size as a runtime parameter
int run_mcts_search(int board_size, const std::string &player1,
                    const std::string &player2, int turn_number) {
  // Since board_size is a template parameter, we need to handle different sizes

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
    bot.get_board().printNumbered();
    bot.set bot.set_moves(turn_number);
    return bot.search();
  } else if (board_size == 7) {
    MCTS<GomokuBoard<7>> bot(100000u);
    bot.get_board().setPlayer1(player1);
    bot.get_board().setPlayer2(player2);
    bot.get_board().printNumbered();
    bot.set_moves(turn_number);
    return bot.search();
  }
  // Add more sizes as needed
  else {
    throw std::runtime_error("Unsupported board size: " +
                             std::to_string(board_size));
  }
}

PYBIND11_MODULE(bot, m) {
  m.doc() = "Gomoku MCTS bot interface";

  m.def("run_mcts_search", &run_mcts_search,
        "Run MCTS search and return the best move", py::arg("board_size"),
        py::arg("player1"), py::arg("player2"), py::arg("turn_number"));
}
