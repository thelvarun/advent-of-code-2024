#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

constexpr char WALL = '#';
constexpr char LEFT_BRACKET = '[';
constexpr char RIGHT_BRACKET = ']';
constexpr char LEFT_PAREN = '(';
constexpr char RIGHT_PAREN = ')';
constexpr char OPEN_SPOT = '.';
constexpr char ROBOT = '@';
constexpr char OBSTACLE = 'O';

ostream& operator<<(ostream& o, const pii& rhs) {
  o << "{" << rhs.first << "," << rhs.second << "}";
  return o;
}

bool inb(pii pos, size_t max_i, size_t max_j) {
  return 0 <= pos.first && pos.first < max_i && 0 <= pos.second &&
         pos.second < max_j;
}

pii operator+(const pii& lhs, const pii& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

pii operator-(const pii& lhs, const pii& rhs) {
  return {lhs.first - rhs.first, lhs.second - rhs.second};
}

void pg(vector<vector<char>>& grid) {
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      cout << grid[i][j];
    }
    cout << endl;
  }
}

pii move_robot_horizontally(pii robot_pos, vector<vector<char>>& grid,
                            pii dir) {
  // cout << "move robot horizontally!" << endl;
  pii last_pos_of_movable_objs = robot_pos;
  while (
      inb(last_pos_of_movable_objs, grid.size(), grid[0].size()) &&
      (grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
           ROBOT ||
       grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
           LEFT_BRACKET ||
       grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
           RIGHT_BRACKET)) {
    last_pos_of_movable_objs = last_pos_of_movable_objs + dir;
  }

  // cout << "last pos of movable objs after iterating: "
  //      << last_pos_of_movable_objs << endl;

  if (grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
      WALL) {
    return robot_pos;
  }

  // We know this has to be a dot, so go start from the opposite direction and
  // move the objects in the direction we're going in.
  while (last_pos_of_movable_objs != robot_pos) {
    // cout << "in change while loop..." << endl;
    grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] =
        grid[last_pos_of_movable_objs.first - dir.first]
            [last_pos_of_movable_objs.second - dir.second];
    grid[last_pos_of_movable_objs.first - dir.first]
        [last_pos_of_movable_objs.second - dir.second] = OPEN_SPOT;
    last_pos_of_movable_objs = last_pos_of_movable_objs - dir;
  }

  // cout << "we got here..." << endl;
  // Do one more move for the robot
  return robot_pos + dir;
}

// TODO: Change this name
char flip(char c) {
  switch (c) {
    case LEFT_PAREN:
      return LEFT_BRACKET;
    case RIGHT_PAREN:
      return RIGHT_BRACKET;
    case ROBOT:
      return ROBOT;
    default:
      cout << "wtf! unknown char:  " << c << endl;
      assert(false);
  }
}

pii move_robot_vertically(pii robot_pos, vector<vector<char>>& grid, pii dir) {
  stack<pii> s;
  queue<pii> q;
  q.push(robot_pos);
  s.push(robot_pos);
  size_t max_i = grid.size();
  size_t max_j = grid[0].size();
  vector<vector<bool>> seen(max_i, vector<bool>(max_j, false));
  while (!q.empty()) {
    pii cur = q.front();
    q.pop();

    // Check the character that's in the direction that you're going.
    pii next_pos = cur + dir;
    if (!inb(next_pos, max_i, max_j)) {
      return robot_pos;
    }

    char next_char = grid[next_pos.first][next_pos.second];

    if (next_char == WALL) {
      return robot_pos;
    }

    // Add these into the queue and the stack, and switch them to parens
    if (next_char == LEFT_BRACKET && !seen[next_pos.first][next_pos.second]) {
      q.push(next_pos);
      s.push(next_pos);
      seen[next_pos.first][next_pos.second] = true;
      // grid[next_pos.first][next_pos.second] = LEFT_PAREN;

      // We also have to add the closing bracket. If we saw a '[', a ']' should
      // be on the right.

      // Sanity checks
      assert(inb({next_pos.first, next_pos.second + 1}, max_i, max_j));
      assert(grid[next_pos.first][next_pos.second + 1] == RIGHT_BRACKET);

      // Actually add the other bracket into the queue and stack.
      q.push({next_pos.first, next_pos.second + 1});
      s.push({next_pos.first, next_pos.second + 1});
      seen[next_pos.first][next_pos.second + 1] = true;
      // grid[next_pos.first][next_pos.second + 1] = RIGHT_PAREN;
    }

    if (next_char == RIGHT_BRACKET && !seen[next_pos.first][next_pos.second]) {
      q.push(next_pos);
      s.push(next_pos);
      seen[next_pos.first][next_pos.second] = true;
      // grid[next_pos.first][next_pos.second] = RIGHT_PAREN;

      // We also have to add the closing bracket. If we saw a ']', a '[' should
      // be on the left.

      // Sanity checks
      assert(inb({next_pos.first, next_pos.second - 1}, max_i, max_j));
      assert(grid[next_pos.first][next_pos.second - 1] == LEFT_BRACKET);

      // Actually add the other bracket into the queue and stack.
      q.push({next_pos.first, next_pos.second - 1});
      s.push({next_pos.first, next_pos.second - 1});
      seen[next_pos.first][next_pos.second - 1] = true;
      // grid[next_pos.first][next_pos.second - 1] = LEFT_PAREN;
    }
  }

  while (!s.empty()) {
    pii cur = s.top();
    s.pop();
    pii next = cur + dir;
    grid[next.first][next.second] = grid[cur.first][cur.second];
    grid[cur.first][cur.second] = OPEN_SPOT;
  }
  return robot_pos + dir;
}

ll sum_gps_coordinates(vector<vector<char>>& grid) {
  ll output = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == LEFT_BRACKET) {
        output += (100 * i) + j;
      }
    }
  }
  return output;
}

vector<vector<char>> expand_grid(vector<vector<char>>& grid) {
  vector<vector<char>> expanded_grid(grid.size(),
                                     vector<char>(grid[0].size() * 2, '_'));
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      char c = grid[i][j];
      char first_char = 'x';
      char second_char = 'o';
      if (c == WALL) {
        first_char = WALL;
        second_char = WALL;
      } else if (c == OBSTACLE) {
        first_char = LEFT_BRACKET;
        second_char = RIGHT_BRACKET;
      } else if (c == OPEN_SPOT) {
        first_char = OPEN_SPOT;
        second_char = OPEN_SPOT;
      } else if (c == ROBOT) {
        first_char = ROBOT;
        second_char = OPEN_SPOT;
      } else {
        cout << "wtf expand grid!" << endl;
        assert(false);
      }
      expanded_grid[i][j * 2] = first_char;
      expanded_grid[i][j * 2 + 1] = second_char;
    }
  }
  return expanded_grid;
}

void solve(string file_name) {
  ifstream fin(file_name);
  vector<vector<char>> grid;
  pii robot_pos;
  string line;
  while (getline(fin, line) && line != "") {
    vector<char> row;
    for (int i = 0; i < line.length(); ++i) {
      row.push_back(line[i]);
      if (line[i] == '@') {
        robot_pos = {grid.size(), i * 2};
      }
    }
    grid.push_back(row);
  }

  vector<vector<char>> expanded_grid = expand_grid(grid);
  // cout << "-------- Initial state: -------" << endl;
  // pg(expanded_grid);

  vector<char> moves;
  while (getline(fin, line)) {
    for (char c : line) {
      moves.push_back(c);
    }
  }

  for (char move : moves) {
    pii dir = {0, 0};
    bool is_horizontal_move = false;
    switch (move) {
      case '<':
        dir = {0, -1};
        is_horizontal_move = true;
        break;
      case '>':
        dir = {0, 1};
        is_horizontal_move = true;
        break;
      case '^':
        dir = {-1, 0};
        break;
      case 'v':
        dir = {1, 0};
        break;
      default:
        break;
    }
    // cout << "------- Move " << move << " " << dir << ": -------" << endl;
    if (is_horizontal_move) {
      robot_pos = move_robot_horizontally(robot_pos, expanded_grid, dir);
    } else {
      robot_pos = move_robot_vertically(robot_pos, expanded_grid, dir);
    }
    // pg(expanded_grid);
    // cin.ignore();
  }
  ll output = sum_gps_coordinates(expanded_grid);
  cout << output << endl;
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
