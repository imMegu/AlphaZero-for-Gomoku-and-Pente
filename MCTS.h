#pragma once
#include "Bitboard.h"
#include "Node.h"
#include <memory>
#include <chrono>
#include <algorithm>

using namespace std;

template <typename BoardType> struct MCTS {
private:
  int it;
  uint moves_made = 0;
  unique_ptr<Node> root;
  BoardType board; // board is current base iteration
                   // state is one being simulated

  bool progressive_widening(Node *node) {
    // If it's the root node, allow all to be expanded quickly
    // (This allows us to avoid missing a win in next move, but iterations need
    // to be at least 4x bigger than the ammount of moves possible (for a 50x50
    // board theres 2500 moves))
    if (node->parent == nullptr) {
      return node->children.size() <
             BoardType::BOARDSIZE - BoardType::SIDE - moves_made;
    }

    // Progressive Widening formula: b < k * N^a
    // b = node->children.size()
    // N = node->v
    // k, a (configurable)
    constexpr float k = 10.0;
    constexpr float a = 0.5;

    // Check if current child count is less than the calculated threshold
    return node->children.size() < (k * pow(max(1u, node->v), a));
  }

  Node *select(BoardType *state) {
    Node *node = root.get();

    while (true) {

      // If node is terminal, return it
      if (state->is_over()) {
        return node;
      }

      // If it's a leaf node, select it for expansion
      // (should only happen if it hasn't been visited)
      if (node->children.empty()) {
        return node;
      }

      // PROGRESSIVE WIDENING
      if (progressive_widening(node)) {
        return node;
      }

      // Select best child using UCB
      Node *bestChild = nullptr;
      float bestScore = -FLT_MAX;

      for (const auto &child : node->children) {
        float score = child->ucb();
        if (score > bestScore) {
          bestScore = score;
          bestChild = child.get();
        }
      }

      if (!bestChild)
        break;

      // Update state for the next selection iteration
      state->make_move(bestChild->m);
      node = bestChild;
    }
    return node;
  }

  int simulate(BoardType *state) {
    int randomMove = rand() % BoardType::BOARDSIZE;
    while (!state->is_over()) {
      while (state->occupied(randomMove) ||
             (randomMove % (BoardType::SIDE + 1) == BoardType::SIDE))
        randomMove = rand() % BoardType::BOARDSIZE;
      state->make_move(randomMove);
    }
    if (state->win(0))
      return 0;
    if (state->win(1))
      return 1;
    return 2;
  }

  inline void backpropagate(Node *node, int winner) {
    while (node != nullptr) {
      node->v++;
      if (winner == 2) {
        // No change in wins
      } else if (winner == !node->player)
        node->w += 1;
      else {
        node->w -= 1;
      }
      node = node->parent;
    }
  }

public:
  MCTS(uint it)
      : it(it), root(make_unique<Node>(nullptr, 0, -1)), board(BoardType()) {};

  // Prevent copying
  MCTS(const MCTS &) = delete;
  MCTS &operator=(const MCTS &) = delete;

  // Allow moving
  MCTS(MCTS &&) = default;
  MCTS &operator=(MCTS &&) = default;

  BoardType &get_board() { return board; }
  const BoardType &get_board() const { return board; }

  uint get_moves() { return moves_made; }
  void set_moves(uint m) { moves_made = m; }

  void make_move(int pos) {
    bool valid = board.make_move(pos);
    if (valid)
      moves_made++;
  }

  int search(bool verbose = false) {

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < it; i++) {
      BoardType state = board.copy();
      Node *leaf = select(&state);

      // If not terminal, expand it
      if (!state.is_over()) {
        Node *child = leaf->expand(state, moves_made);

        if (child != leaf) { // Successfully created a new child
          state.make_move(child->m);
          leaf = child;
        }
        // If child == leaf, we've hit the expansion limit, simulate from
        // current node
      }

      int winner = simulate(&state);
      backpropagate(leaf, winner);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << it << " iterations in " << duration.count()
         << " milliseconds. (move " << moves_made + 1 << ')' << endl;

    if (root->children.empty()) {
      // Handle the case where no child nodes were created.
      cout << "Error: No children created for the root node after " << it
           << " iterations. Returning -1 (Invalid Move)." << endl
           << "Maybe preset board was already finished." << endl;
      return -1; // Return an invalid move
    }

    using ChildIterator = std::vector<std::unique_ptr<Node>>::iterator;

    ChildIterator best_child_it = root->children.end();
    uint max_visits = 0;
    int best_move = -1;

    // O(n), where n is children size (in 50x50 its 2500, negligible)
    for (ChildIterator it = root->children.begin(); it != root->children.end();
         ++it) {
      if ((*it)->v > max_visits) {
        max_visits = (*it)->v;
        best_child_it = it;
        best_move = (*it)->m;
      }
    }

    // Ensure a valid child was found
    // Should be, since root->children is not empty
    if (best_child_it == root->children.end()) {
      return -1;
    }

    /*
     *
     * This is code for printing ordered by most wins/visits.
     *
     *
    vector<Node *> sorted_children;
    for (const auto &child_ptr : root->children) {
      sorted_children.push_back(child_ptr.get());
    }

    sort(sorted_children.begin(), sorted_children.end(),
         [](Node *a, Node *b) { return a->w < b->w; });

    int total = 0, visits = 0;
    for (auto child : sorted_children) { // 'child' is now a raw pointer
      if (verbose)
        cout << "Move: " << child->m << "; Visits: " << child->v
             << "; Wins: " << child->w << endl;
      total += child->w;
      visits++;
    }

    if (verbose) {
      cout << "Root Visits: " << root->v << endl
           << "Root Score: " << root->w << "; Children Score: " << total << endl
           << "Root Children: " << root->children.size()
           << "; Counted Children: " << visits << endl;
    }

    // / */

    if (verbose)
      cout << "Old Visits: " << root->v << endl;

    // Transfer Ownership to 'root'
    // Old root (and all other children) is destroyed
    // New root keeps its subtree and statistics
    root = std::move(*best_child_it);
    root->parent = nullptr;

    if (verbose) {
      cout << "New Visits: " << root->v << endl
           << "New Score [-1, 1]: " << root->w / root->v << endl
           << "Play made: " << root->m << endl;
    }

    return best_move;
  }
};
