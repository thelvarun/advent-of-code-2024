#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

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

pii move_robot(pii robot_pos, vector<vector<char>>& grid, pii dir) {
  pii last_pos_of_movable_objs = robot_pos;
  while (
      inb(last_pos_of_movable_objs, grid.size(), grid[0].size()) &&
      (grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
           '@' ||
       grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
           'O')) {
    last_pos_of_movable_objs = last_pos_of_movable_objs + dir;
  }

  if (grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] ==
      '#') {
    return robot_pos;
  }

  // We know this has to be a dot, so go start from the opposite direction and
  // move the objects in the direction we're going in.
  while (last_pos_of_movable_objs != robot_pos) {
    grid[last_pos_of_movable_objs.first][last_pos_of_movable_objs.second] =
        grid[last_pos_of_movable_objs.first - dir.first]
            [last_pos_of_movable_objs.second - dir.second];
    grid[last_pos_of_movable_objs.first - dir.first]
        [last_pos_of_movable_objs.second - dir.second] = '.';
    last_pos_of_movable_objs = last_pos_of_movable_objs - dir;
  }

  // Do one more move for the robot
  return robot_pos + dir;
}

ll sum_gps_coordinates(vector<vector<char>>& grid) {
  ll output = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      if (grid[i][j] == 'O') {
        output += (100 * i) + j;
      }
    }
  }
  return output;
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
        robot_pos = {grid.size(), i};
      }
    }
    grid.push_back(row);
  }

  vector<char> moves;
  while (getline(fin, line)) {
    for (char c : line) {
      moves.push_back(c);
    }
  }

  for (char move : moves) {
    pii dir = {0, 0};
    switch (move) {
      case '<':
        dir = {0, -1};
        break;
      case '>':
        dir = {0, 1};
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
    robot_pos = move_robot(robot_pos, grid, dir);
    // pg(grid);
    // cin.ignore();
  }

  // for (int i = 0; i < grid.size(); ++i) {
  //   for (int j = 0; j < grid[i].size(); ++j) {
  //     cout << grid[i][j];
  //   }
  //   cout << endl;
  // }
  ll output = sum_gps_coordinates(grid);
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
