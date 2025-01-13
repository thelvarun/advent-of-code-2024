#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr char WALL = '#';
constexpr char OPEN_SPOT = '.';
constexpr char EXIT = 'E';
constexpr char START = 'S';

typedef long long ll;
typedef pair<int, int> pii;

pii dl = {0, -1};
pii dr = {0, 1};
pii du = {-1, 0};
pii dd = {1, 0};
vector<pii> deltas = {dl, dr, du, dd};

enum class dir { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, UNKNOWN = 4 };
string dirts(dir d) {
  switch (d) {
    case dir::LEFT:
      return "LEFT";
    case dir::RIGHT:
      return "RIGHT";
    case dir::UP:
      return "UP";
    case dir::DOWN:
      return "DOWN";
    case dir::UNKNOWN:
      return "UNKNOWN";
    default:
      assert(false);
  }
}

string operator~(pii p) {
  return "{" + to_string(p.first) + "," + to_string(p.second) + "}";
}

ostream& operator<<(ostream& o, pii rhs) {
  o << ~rhs;
  return o;
}

pii operator+(pii lhs, pii rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

pii operator-(pii lhs, pii rhs) {
  return {lhs.first - rhs.first, lhs.second - rhs.second};
}

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    // Hash the first element
    size_t hash1 = hash<T1>{}(p.first);
    // Hash the second element
    size_t hash2 = hash<T2>{}(p.second);
    // Combine the two hash values
    return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
  }
};

void pg(vector<vector<char>> grid) {
  for (const vector<char>& row : grid) {
    for (char c : row) {
      cout << c;
    }
    cout << endl;
  }
}

string pv(vector<pii> v) {
  string s;
  string separator = "";
  for (pii p : v) {
    s += separator + ~p;
    separator = ", ";
  }
  return s;
}

bool inb(pii pos, size_t max_i, size_t max_j) {
  return 0 <= pos.first && pos.first < max_i && 0 <= pos.second &&
         pos.second < max_j;
}

struct elem {
  bool visited = false;
  bool is_wall = true;
  ll dist = LLONG_MAX;
  dir facing_dir = dir::UNKNOWN;
  pii pos = {-1, -1};
  vector<pii> parents;
  bool backwards_visited = false;

  string to_string() {
    return "{visited=" + (visited ? string("true") : string("false")) +
           ", is_wall=" + (is_wall ? "true" : "false") +
           ", dist=" + std::to_string(dist) + ", " + dirts(facing_dir) +
           ", pos=" + ~pos + ", " + "parents={" + pv(parents) + "}}";
  }
};

struct comp {
  bool operator()(const elem& a, const elem& b) {
    // Sorting on the basis of height(Just for example)
    return (a.dist > b.dist);
  }
};

ll find_num_tiles_on_best_paths(vector<vector<elem>>& elems, pii start_pos,
                                pii exit_pos) {
  // now we just trace backward using a bfs
  queue<pii> q;
  q.push(exit_pos);
  elems[exit_pos.first][exit_pos.second].backwards_visited = true;
  ll count = 0;
  while (!q.empty()) {
    // cout << "------" << endl;
    pii cur_pos = q.front();
    count++;
    q.pop();
    elem& cur_elem = elems[cur_pos.first][cur_pos.second];
    // cout << cur_elem.to_string() << endl;
    for (pii parent : cur_elem.parents) {
      if (!elems[parent.first][parent.second].backwards_visited) {
        q.push(parent);
        elems[parent.first][parent.second].backwards_visited = true;
      }
    }
  }
  return count;
}

ll find_minimum_score(vector<vector<elem>>& elems, ll total_nodes,
                      pii start_pos, pii exit_pos) {
  elems[start_pos.first][start_pos.second].dist = 0;
  elems[start_pos.first][start_pos.second].facing_dir = dir::RIGHT;

  size_t max_i = elems.size();
  size_t max_j = elems[0].size();
  ll nodes_visited = 0;

  while (nodes_visited < total_nodes) {
    // cout << "---" << endl;
    // cout << "nodes_visited: " << nodes_visited
    //      << ", total_nodes: " << total_nodes << endl;
    // Find unvisited node with minimum distance to start
    ll min_dist = LLONG_MAX;
    pii node_pos = {-1, -1};
    for (int i = 0; i < elems.size(); ++i) {
      for (int j = 0; j < elems[i].size(); ++j) {
        if (!elems[i][j].is_wall && !elems[i][j].visited &&
            elems[i][j].dist < min_dist) {
          node_pos = {i, j};
          min_dist = elems[i][j].dist;
        }
      }
    }

    elem& cur = elems[node_pos.first][node_pos.second];
    // cout << cur.to_string() << endl;
    cur.visited = true;
    nodes_visited++;

    pii left = cur.pos + dl;
    pii right = cur.pos + dr;
    pii up = cur.pos + du;
    pii down = cur.pos + dd;

    if (inb(left, max_i, max_j) && !elems[left.first][left.second].is_wall) {
      elem& e = elems[left.first][left.second];
      ll cost = (cur.facing_dir == dir::LEFT) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::LEFT;
    }

    if (inb(right, max_i, max_j) && !elems[right.first][right.second].is_wall) {
      elem& e = elems[right.first][right.second];
      ll cost = (cur.facing_dir == dir::RIGHT) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::RIGHT;
    }

    if (inb(up, max_i, max_j) && !elems[up.first][up.second].is_wall) {
      elem& e = elems[up.first][up.second];
      ll cost = (cur.facing_dir == dir::UP) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::UP;
    }

    if (inb(down, max_i, max_j) && !elems[down.first][down.second].is_wall) {
      elem& e = elems[down.first][down.second];
      ll cost = (cur.facing_dir == dir::DOWN) ? 1 : 1001;
      if ((cur.dist + cost) < e.dist) {
        e.dist = cur.dist + cost;
        e.parents = {cur.pos};
      } else if ((cur.dist + cost) == e.dist) {
        e.parents.push_back(cur.pos);
      }
      e.facing_dir = dir::DOWN;
    }
  }
  ll tiles_along_best_paths =
      find_num_tiles_on_best_paths(elems, start_pos, exit_pos);
  cout << "tiles_along_best_paths: " << tiles_along_best_paths << endl;
  return elems[exit_pos.first][exit_pos.second].dist;
}

enum class node_type { UNKNOWN = 0, TOP = 1, RIGHT = 2, BOTTOM = 3, LEFT = 4 };
string operator~(node_type node_type) {
  switch (node_type) {
    case node_type::UNKNOWN:
      return "UNKNOWN";
    case node_type::TOP:
      return "TOP";
    case node_type::RIGHT:
      return "RIGHT";
    case node_type::BOTTOM:
      return "BOTTOM";
    case node_type::LEFT:
      return "LEFT";
    default:
      cout << "wtf!" << endl;
      assert(false);
  }
}
struct out_node;

struct in_node {
  pii id = {-1, -1};
  node_type type = node_type::UNKNOWN;

  out_node* left_out_node = nullptr;
  ll cost_to_left_out_node = -1;

  out_node* right_out_node = nullptr;
  ll cost_to_right_out_node = -1;

  out_node* top_out_node = nullptr;
  ll cost_to_top_out_node = -1;

  out_node* bottom_out_node = nullptr;
  ll cost_to_bottom_out_node = -1;

  in_node(pii id, node_type type) {
    this->id = id;
    this->type = type;
  }
};

struct out_node {
  pii id = {-1, -1};
  node_type type = node_type::UNKNOWN;
  in_node* dest = nullptr;
  ll cost = -1;
  out_node(pii id, node_type type) {
    this->id = id;
    this->type = type;
  }
};

ostream& operator<<(ostream& o, const out_node& out_node) {
  o << "id=" << out_node.id << ", type=" << ~out_node.type << ", dest="
    << (out_node.dest == nullptr
            ? "null"
            : ~out_node.dest->id + " - " + to_string(out_node.cost));
  return o;
}

ostream& operator<<(ostream& o, const in_node& in_node) {
  o << "id=" << in_node.id << ", type=" << ~in_node.type << ", left_out_node="
    << (in_node.left_out_node == nullptr
            ? "null"
            : ~in_node.left_out_node->id + " - " +
                  to_string(in_node.cost_to_left_out_node))
    << ", right_out_node="
    << (in_node.right_out_node == nullptr
            ? "null"
            : ~in_node.right_out_node->id + " - " +
                  to_string(in_node.cost_to_right_out_node))
    << ", top_out_node="
    << (in_node.top_out_node == nullptr
            ? "null"
            : ~in_node.top_out_node->id + " - " +
                  to_string(in_node.cost_to_top_out_node))
    << ", bottom_out_node="
    << (in_node.bottom_out_node == nullptr
            ? "null"
            : ~in_node.bottom_out_node->id + " - " +
                  to_string(in_node.cost_to_bottom_out_node));
  return o;
}

struct big_node {
  pii id = {-1, -1};
  bool visited = false;
  ll dist = LLONG_MAX;
  out_node top_out_node;
  out_node bot_out_node;
  out_node left_out_node;
  out_node right_out_node;

  in_node top_in_node;
  in_node bot_in_node;
  in_node left_in_node;
  in_node right_in_node;

  node_type which_input_node = node_type::UNKNOWN;

  big_node(pii id)
      : top_out_node(id, node_type::TOP),
        bot_out_node(id, node_type::BOTTOM),
        left_out_node(id, node_type::LEFT),
        right_out_node(id, node_type::RIGHT),
        top_in_node(id, node_type::TOP),
        bot_in_node(id, node_type::BOTTOM),
        left_in_node(id, node_type::LEFT),
        right_in_node(id, node_type::RIGHT) {
    this->id = id;
  }

  bool connected_to_node_on_right() { return right_out_node.dest != nullptr; }
  bool connected_to_node_on_left() { return left_out_node.dest != nullptr; }
  bool connected_to_node_above() { return top_out_node.dest != nullptr; }
  bool connected_to_node_below() { return bot_out_node.dest != nullptr; }

  void connect_internal_nodes() {
    // Connect the internal nodes such that only if output nodes have a dest,
    // internal nodes should connect. input and output noodes of the same type
    // (r,l,u,d) will never be connected.
    if (top_out_node.dest != nullptr) {
      // TODO: should the internal nodes be connected even if there's no input
      // coming into the internal node?

      // The top input node can't connect to the top output node.
      top_in_node.top_out_node = nullptr;
      top_in_node.cost_to_top_out_node = -1;

      // The right input node has a cost of 1000 to go to the top output node
      right_in_node.top_out_node = &top_out_node;
      right_in_node.cost_to_top_out_node = 1000;

      // The left input node has a cost of 1000 to go to the top output node
      left_in_node.top_out_node = &top_out_node;
      left_in_node.cost_to_top_out_node = 1000;

      // The bottom node costs nothing to go to the top node, since input from
      // the bottom implies we're going in the north direction
      bot_in_node.top_out_node = &top_out_node;
      bot_in_node.cost_to_top_out_node = 0;
    }

    if (right_out_node.dest != nullptr) {
      // TODO: should the internal nodes be connected even if there's no input
      // coming into the internal node?

      // The top input node has a cost of 1000 to go to the right output node
      top_in_node.right_out_node = &right_out_node;
      top_in_node.cost_to_right_out_node = 1000;

      // The right input node can't connect to the right output node.
      right_in_node.right_out_node = nullptr;
      right_in_node.cost_to_right_out_node = LLONG_MAX;

      // The left input node costs nothing to go to the right output node, since
      // input from the left implies we're going in the east direction
      left_in_node.right_out_node = &right_out_node;
      left_in_node.cost_to_right_out_node = 0;

      // The bottom input node has a cost of 1000 to go to the right output node
      bot_in_node.right_out_node = &right_out_node;
      bot_in_node.cost_to_right_out_node = 1000;
    }

    if (bot_out_node.dest != nullptr) {
      // TODO: should the internal nodes be connected even if there's no input
      // coming into the internal node?

      // The top input node costs nothing to go to the bottom output node, since
      // input from the top implies we're going in the south direction
      top_in_node.bottom_out_node = &bot_out_node;
      top_in_node.cost_to_bottom_out_node = 0;

      // The right input node has a cost of 1000 to go to the bottom output node
      right_in_node.bottom_out_node = &bot_out_node;
      right_in_node.cost_to_bottom_out_node = 1000;

      // The left input node has a cost of 1000 to go to the bottom output node
      left_in_node.bottom_out_node = &bot_out_node;
      left_in_node.cost_to_bottom_out_node = 1000;

      // The bottom input node can't connect to the bottom output node.
      bot_in_node.bottom_out_node = nullptr;
      bot_in_node.cost_to_bottom_out_node = LLONG_MAX;
    }

    if (left_out_node.dest != nullptr) {
      // TODO: should the internal nodes be connected even if there's no input
      // coming into the internal node?

      // The top input node has a cost of 1000 to go to the left output node
      top_in_node.left_out_node = &left_out_node;
      top_in_node.cost_to_left_out_node = 1000;

      // The right input node costs nothing to go to the left output node, since
      // input from the right implies we're going in the west direction
      right_in_node.left_out_node = &left_out_node;
      right_in_node.cost_to_left_out_node = 0;

      // The left input node can't connect to the left output node.
      left_in_node.left_out_node = nullptr;
      left_in_node.cost_to_left_out_node = LLONG_MAX;

      // The bottom input node has a cost of 1000 to go to the left output node
      bot_in_node.left_out_node = &left_out_node;
      bot_in_node.cost_to_left_out_node = 1000;
    }
  }

  void connect_to_big_node_to_the_left(big_node& on_the_left) {
    assert((on_the_left.id - this->id == pii{0, -1}));

    this->left_out_node.dest = &on_the_left.right_in_node;
    this->left_out_node.cost = 1;

    on_the_left.right_out_node.dest = &this->left_in_node;
    on_the_left.right_out_node.cost = 1;
  }

  void connect_to_big_node_to_the_right(big_node& on_the_right) {
    assert((on_the_right.id - this->id == pii{0, 1}));

    this->right_out_node.dest = &on_the_right.left_in_node;
    this->right_out_node.cost = 1;

    on_the_right.left_out_node.dest = &this->right_in_node;
    on_the_right.left_out_node.cost = 1;
  }

  void connect_to_big_node_above(big_node& above) {
    assert((above.id - this->id == pii{-1, 0}));

    this->top_out_node.dest = &above.bot_in_node;
    this->top_out_node.cost = 1;

    above.bot_out_node.dest = &this->top_in_node;
    above.bot_out_node.cost = 1;
  }

  void connect_to_big_node_below(big_node& below) {
    assert((below.id - this->id == pii{1, 0}));

    this->bot_out_node.dest = &below.top_in_node;
    this->bot_out_node.cost = 1;

    below.top_out_node.dest = &this->bot_in_node;
    below.top_out_node.cost = 1;
  }
};
ostream& operator<<(ostream& o, const big_node& big_node) {
  o << "{id=" << big_node.id << "\ndist=" << big_node.dist;
  // << "\ntop_out_node=" << big_node.top_out_node
  // << "\nbot_out_node=" << big_node.bot_out_node
  // << "\nleft_out_node=" << big_node.left_out_node
  // << "\nright_out_node=" << big_node.right_out_node << "\ntop_in_node={"
  // << big_node.top_in_node << "}"
  // << "\nbot_in_node={" << big_node.bot_in_node << "}"
  // << "\nleft_in_node={" << big_node.left_in_node << "}"
  // << "\nright_in_node={" << big_node.right_in_node << "}}";
  return o;
}

// ll find_num_tiles_on_best_paths(vector<vector<elem>>& elems, pii start_pos,
//                                 pii exit_pos) {
//   // now we just trace backward using a bfs
//   queue<pii> q;
//   q.push(exit_pos);
//   elems[exit_pos.first][exit_pos.second].backwards_visited = true;
//   ll count = 0;
//   while (!q.empty()) {
//     // cout << "------" << endl;
//     pii cur_pos = q.front();
//     count++;
//     q.pop();
//     elem& cur_elem = elems[cur_pos.first][cur_pos.second];
//     // cout << cur_elem.to_string() << endl;
//     for (pii parent : cur_elem.parents) {
//       if (!elems[parent.first][parent.second].backwards_visited) {
//         q.push(parent);
//         elems[parent.first][parent.second].backwards_visited = true;
//       }
//     }
//   }
//   return count;
// }

ll find_minimum_score(unordered_map<pii, big_node*, hash_pair>& big_nodes,
                      pii start_pos, pii exit_pos) {
  big_nodes.at(start_pos)->dist = 0;
  big_nodes.at(start_pos)->which_input_node = node_type::LEFT;
  // elems[start_pos.first][start_pos.second].dist = 0;
  // elems[start_pos.first][start_pos.second].facing_dir = dir::RIGHT;

  ll total_nodes = big_nodes.size();
  ll nodes_visited = 0;

  while (nodes_visited < total_nodes) {
    cout << "----------" << endl;
    // Find unvisited node with minimum distance to start
    ll min_dist = LLONG_MAX;
    pii node_pos = {-1, -1};
    for (const auto& kv : big_nodes) {
      if (!kv.second->visited && kv.second->dist < min_dist) {
        node_pos = kv.first;
        min_dist = kv.second->dist;
      }
    }

    big_node* cur = big_nodes.at(node_pos);
    cout << "cur: " << (*cur) << endl;
    // cout << cur.to_string() << endl;
    cur->visited = true;
    nodes_visited++;
    in_node* cur_input_node = nullptr;
    switch (cur->which_input_node) {
      case node_type::LEFT:
        cur_input_node = &cur->left_in_node;
        break;
      case node_type::RIGHT:
        cur_input_node = &cur->right_in_node;
        break;
      case node_type::TOP:
        cur_input_node = &cur->top_in_node;
        break;
      case node_type::BOTTOM:
        cur_input_node = &cur->bot_in_node;
        break;
      default:
        cout << "this shouldn't happen" << endl;
        assert(false);
        break;
    }

    // cout << "cur input node: " << *cur_input_node << endl;

    pii left = cur->id + dl;
    pii right = cur->id + dr;
    pii up = cur->id + du;
    pii down = cur->id + dd;

    // Look for the next input node we need to get to.
    if (cur->connected_to_node_on_left()) {
      assert(cur->left_out_node.dest != nullptr &&
             cur->left_out_node.dest->id == left);
      if (cur->dist + cur_input_node->cost_to_left_out_node +
              cur->left_out_node.cost <
          big_nodes.at(left)->dist) {
        big_nodes.at(left)->dist = cur->dist +
                                   cur_input_node->cost_to_left_out_node +
                                   cur->left_out_node.cost;
        big_nodes.at(left)->which_input_node = node_type::RIGHT;
      }
    }

    if (cur->connected_to_node_on_right()) {
      assert(cur->right_out_node.dest != nullptr &&
             cur->right_out_node.dest->id == right);
      // cout << "in here..." << endl;
      // cout << "cost to right out node: "
      //  << cur_input_node->cost_to_right_out_node << endl;
      // cout << "right out node cost: " << cur->right_out_node.cost << endl;
      // cout << big_nodes.at(right)->dist << endl;
      if (cur->dist + cur_input_node->cost_to_right_out_node +
              cur->right_out_node.cost <
          big_nodes.at(right)->dist) {
        // cout << "we got in here ? " << endl;
        big_nodes.at(right)->dist = cur->dist +
                                    cur_input_node->cost_to_right_out_node +
                                    cur->right_out_node.cost;
        // cout << "right node new distance: " << big_nodes.at(right)->dist
        //      << endl;
        big_nodes.at(right)->which_input_node = node_type::LEFT;
      }
    }

    if (cur->connected_to_node_above()) {
      assert(cur->top_out_node.dest != nullptr &&
             cur->top_out_node.dest->id == up);
      if (cur->dist + cur_input_node->cost_to_top_out_node +
              cur->top_out_node.cost <
          big_nodes.at(up)->dist) {
        big_nodes.at(up)->dist = cur->dist +
                                 cur_input_node->cost_to_top_out_node +
                                 cur->top_out_node.cost;
        big_nodes.at(up)->which_input_node = node_type::BOTTOM;
      }
    }

    if (cur->connected_to_node_below()) {
      assert(cur->bot_out_node.dest != nullptr &&
             cur->bot_out_node.dest->id == down);
      if (cur->dist + cur_input_node->cost_to_bottom_out_node +
              cur->bot_out_node.cost <
          big_nodes.at(down)->dist) {
        big_nodes.at(down)->dist = cur->dist +
                                   cur_input_node->cost_to_bottom_out_node +
                                   cur->bot_out_node.cost;
        big_nodes.at(down)->which_input_node = node_type::TOP;
      }
    }
  }
  // ll tiles_along_best_paths =
  //     find_num_tiles_on_best_paths(elems, start_pos, exit_pos);
  // cout << "tiles_along_best_paths: " << tiles_along_best_paths << endl;
  return big_nodes.at(exit_pos)->dist;
}

void solve(string file_name) {
  ifstream fin(file_name);
  pii exit_pos;
  pii start_pos;
  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    vector<char> row;
    for (int j = 0; j < line.length(); ++j) {
      row.push_back(line[j]);
      if (line[j] == EXIT) {
        exit_pos = {grid.size(), j};
      } else if (line[j] == START) {
        start_pos = {grid.size(), j};
      }
    }
    grid.push_back(row);
  }

  vector<big_node> big_nodes;
  unordered_map<pii, big_node*, hash_pair> map;
  // First create the big nodes.
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] != WALL) {
        big_node bn({i, j});
        big_nodes.push_back(bn);
        // map.insert({{i, j}, &bn});
      }
    }
  }

  for (int i = 0; i < big_nodes.size(); ++i) {
    map.insert({big_nodes[i].id, &big_nodes[i]});
  }

  // Print out the big nodes
  // cout << "+++++++" << endl;
  // for (int i = 0; i < big_nodes.size(); ++i) {
  //   cout << "-------" << endl;
  //   cout << big_nodes[i] << endl;
  // }

  // Print out the big nodes by iterating through the map
  // cout << "#######" << endl;
  // cout << "map.size before: " << map.size() << endl;
  // for (const auto& kv : map) {
  //   cout << kv.first << " : " << *kv.second << endl;
  // }

  size_t maxi = grid.size();
  size_t maxj = grid[0].size();
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      char c = grid[i][j];
      pii cur_pos = {i, j};
      if (c != WALL) {
        // Now look up down left and right
        pii left = cur_pos + dl;
        pii right = cur_pos + dr;
        pii up = cur_pos + du;
        pii down = cur_pos + dd;

        // cout << "curpos=" << cur_pos << ", left=" << left << ", right=" <<
        // right
        //      << " up=" << up << ", down=" << down << endl;

        if (inb(left, maxi, maxj) && grid[left.first][left.second] != WALL) {
          // cout << "in conn to left" << endl;
          map.at(cur_pos)->connect_to_big_node_to_the_left(*map.at(left));
        }

        if (inb(right, maxi, maxj) && grid[right.first][right.second] != WALL) {
          // cout << "in conn to right" << endl;
          // cout << *map.at(cur_pos) << endl;
          // cout << *map.at(right) << endl;
          map.at(cur_pos)->connect_to_big_node_to_the_right(*map.at(right));
        }

        if (inb(up, maxi, maxj) && grid[up.first][up.second] != WALL) {
          // cout << "in conn to up" << endl;
          map.at(cur_pos)->connect_to_big_node_above(*map.at(up));
        }

        if (inb(down, maxi, maxj) && grid[down.first][down.second] != WALL) {
          // cout << "in conn to down" << endl;
          map.at(cur_pos)->connect_to_big_node_below(*map.at(down));
        }
      }
    }
  }

  // Now connect the internal nodes once we have established all the
  // connections.
  for (auto& kv : map) {
    kv.second->connect_internal_nodes();
  }
  ll output = find_minimum_score(map, start_pos, exit_pos);
  cout << output << endl;

  // Print out the nodes again. Also, connect the internal nodes.
  // for (int i = 0; i < big_nodes.size(); ++i) {
  //   cout << "-----" << endl;
  //   big_nodes[i].connect_internal_nodes();
  //   cout << big_nodes[i] << endl;
  // }

  // vector<vector<elem>> elems;
  // ll total_nodes = 0;
  // for (int i = 0; i < grid.size(); ++i) {
  //   vector<elem> row;
  //   for (int j = 0; j < grid[i].size(); ++j) {
  //     char c = grid[i][j];
  //     row.push_back(elem{.visited = false,
  //                        .is_wall = c == WALL,
  //                        .dist = INT_MAX,
  //                        .facing_dir = dir::UNKNOWN,
  //                        .pos = {i, j},
  //                        .parents = {},
  //                        .backwards_visited = false});
  //     total_nodes += (c != WALL);
  //   }
  //   elems.push_back(row);
  // }
  // ll output = find_minimum_score(elems, total_nodes, start_pos,
  // exit_pos); for (int i = 0; i < elems.size(); ++i) {
  //   cout << "---" << endl;
  //   for (int j = 0; j < elems[i].size(); ++j) {
  //     if (!elems[i][j].is_wall) {
  //       cout << elems[i][j].to_string() << endl;
  //     }
  //   }
  // }
  // cout << output << endl;
  return;
}

int main(int argc, char* argv[]) {
  // Default to sample.in if no arguments were provided.
  string file_name = "sample.in";
  if (argc == 2) {
    file_name = argv[1];
  }
  solve(file_name);
  return 0;
}
