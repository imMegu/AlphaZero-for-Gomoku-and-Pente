#include <vector>
#include <set>
#include <cfloat>
#include <memory>
#include <math.h>
#include <random>
#include <algorithm>

using namespace std;

struct Node {
public:
  Node *parent; // Parent Node
  bool player;  // Used for backpropagation
  int m;
  float w = 0;                            // wins
  uint v = 0;                             // visits
  vector<std::unique_ptr<Node>> children; // Use vector of raw pointers
  set<int> tried_moves;                   // Track which moves have been tried

  Node(Node *parent, bool p, int m) : parent(parent), player(p), m(m) {}

  // Prevent copying
  Node(const Node &) = delete;
  Node &operator=(const Node &) = delete;

  // Allow moving
  Node(Node &&) = default;
  Node &operator=(Node &&) = default;

  inline float ucb(float exploration_weight = 1.414f) {

    if (v == 0)
      return FLT_MAX; // If not visited, give max priority
    if (!parent)      // root node
      return 0;

    float exploitation = w / v;
    float exploration = sqrtf(logf(parent->v) / v);
    return exploitation + (exploration_weight * exploration);
  }
  /*
   * We have some ways of going about this:  * Expand all children at once,
   * -> this creates a wide and balanced tree, but is very memory intensive and
   * has shallow search. Expand one random untried move,
   * -> creates a deep tree and is memor5 effient, but might miss important
   * moves if unlucky
   *
   * Maybe we should try a progressively widening?
   * This could mean expanding earlier in the selection phase,
   * if the number of visits hits a certain threshold.
   * ( b < k * N ^ a )
   * b - child nodes
   * k, a - tuning parameters
   * N - visits
   * Pros: creates a more balanced tree and is not as memory intensive.
   * Cons: still using random moves.
   *
   * example:
   * with 1 visit we have 1 child,
   * 50 visits, 2 children,
   * etc, etc
   *
   * Another way is expanding only the moves that seem relevant, aka,
   * they are close to the current places pieces, this might be computationally
   * expensive tho
   * Pros: Explores the relevant space, does not focus on unimportant moves
   * Cons: Inneficient, naive approach is O(N^2).
   */
  template <typename BoardType> Node *expand(const BoardType &state, int mm) {

    // Build list of valid positions first
    vector<int> positions;
    for (uint i = 0; i < BoardType::BOARDSIZE; i++)
      // Free space, not in padding and not already tried
      if (!state.occupied(i) &&
          (i % (BoardType::SIDE + 1) != BoardType::SIDE) &&
          (tried_moves.find(i) == tried_moves.end()))
        positions.push_back(i);

    // If no valid positions remain, return this node (terminal or fully
    // expanded)
    if (positions.empty()) {
      return this;
    }

    // Randomly select one position
    random_device rd;
    mt19937 g(rd());
    shuffle(positions.begin(), positions.end(), g);

    tried_moves.insert(positions[0]);

    auto child = std::make_unique<Node>(this, !player, positions[0]);
    Node *child_ptr = child.get();
    children.push_back(std::move(child));
    return child_ptr;
  }
};
