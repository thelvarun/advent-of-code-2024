#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

enum class dir { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

string s(dir d) {
  switch (d) {
    case dir::UP:
      return "UP";
    case dir::DOWN:
      return "DOWN";
    case dir::LEFT:
      return "LEFT";
    case dir::RIGHT:
      return "RIGHT";
    default:
      break;
  }
}

int diri(dir d) {
  switch (d) {
    case dir::UP:
      return 0;
    case dir::DOWN:
      return 1;
    case dir::LEFT:
      return 2;
    case dir::RIGHT:
      return 3;
    default:
      break;
  }
}

dir rotate(dir d) {
  switch (d) {
    case dir::UP:
      return dir::RIGHT;
    case dir::DOWN:
      return dir::LEFT;
    case dir::LEFT:
      return dir::UP;
    case dir::RIGHT:
      return dir::DOWN;
    default:
      break;
  }
}

struct delta {
  int di = 0;
  int dj = 0;
};

delta get_delta(dir d) {
  switch (d) {
    case dir::UP:
      return {-1, 0};
    case dir::DOWN:
      return {1, 0};
    case dir::LEFT:
      return {0, -1};
    case dir::RIGHT:
      return {0, 1};
    default:
      break;
  }
  return {0, 0};
}

struct ij {
  int i = -1;
  int j = -1;

  string s() const { return "(" + to_string(i) + "," + to_string(j) + ")"; }
};

ostream& operator<<(ostream& out, const ij& rhs) {
  out << rhs.s();
  return out;
}

bool oob(int max_i, int max_j, ij ij) {
  return !(0 <= ij.i && ij.i < max_i && 0 <= ij.j && ij.j < max_j);
}

string s(vector<vector<char>>& grid) {
  string s;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      s += grid[i][j];
    }
    s += '\n';
  }
  return s;
}

bool does_loop(vector<vector<char>>& grid, ij guard_pos) {
  // cout << "does loop..." << endl;
  ij start = guard_pos;
  vector<vector<vector<bool>>> seen(
      grid.size(),
      vector<vector<bool>>(grid[0].size(), vector<bool>(4, false)));
  dir cur_dir = dir::UP;
  delta cur_delta = get_delta(cur_dir);
  while (!oob(int(grid.size()), int(grid[0].size()), guard_pos)) {
    if (seen[guard_pos.i][guard_pos.j][diri(cur_dir)]) {
      return true;
    }
    seen[guard_pos.i][guard_pos.j][diri(cur_dir)] = true;

    ij next_pos{.i = (guard_pos.i + cur_delta.di),
                .j = (guard_pos.j + cur_delta.dj)};
    if (!oob(int(grid.size()), int(grid[0].size()), next_pos) &&
        (grid[next_pos.i][next_pos.j] == '#' ||
         grid[next_pos.i][next_pos.j] == 'O')) {
      // cout << "in the if clause..." << endl;
      cur_dir = rotate(cur_dir);
      cur_delta = get_delta(cur_dir);
      // while (!oob(int(grid.size()), int(grid[0].size()), next_pos) &&
      //        (grid[next_pos.i][next_pos.j] == '#' ||
      //         grid[next_pos.i][next_pos.j] == 'O')) {
      //   cur_dir = rotate(cur_dir);
      //   cur_delta = get_delta(cur_dir);
      //   next_pos = {.i = (guard_pos.i + cur_delta.di),
      //               .j = (guard_pos.j + cur_delta.dj)};
      // }
    } else {
      guard_pos = next_pos;
    }
  }
  // cout << "got here..." << endl;
  // cin.ignore();
  return false;
}

void solve2() {
  ifstream fin("day6.in");
  vector<vector<char>> grid;
  string line;
  ij guard_start;
  ij guard_pos;
  while (getline(fin, line)) {
    vector<char> row;
    for (int i = 0; i < line.length(); ++i) {
      char c = line[i];
      row.push_back(c);
      if (c == '^') {
        guard_pos = {int(grid.size()), i};
        guard_start = guard_pos;
      }
    }
    grid.push_back(row);
  }
  int loop_pos_count = 0;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if ((i != guard_start.i || j != guard_start.j) && grid[i][j] == '.') {
        // cout << "grid[i][j]: " << grid[i][j] << endl;
        // cin.ignore();
        // mark the grid
        // string temp = s(grid);
        grid[i][j] = 'O';
        if (does_loop(grid, guard_pos)) {
          loop_pos_count++;
        }
        grid[i][j] = '.';
        string t2 = s(grid);
        // if (temp == t2) {
        //   cout << "success" << endl;
        // } else {
        //   cout << "failure" << endl;
        // }
        // cout << "temp == t2" << (temp == t2) << endl;
      }
    }
  }
  cout << loop_pos_count << endl;
}

// void solve() {
//   ifstream fin("sample.in");
//   vector<vector<char>> grid;
//   string line;
//   ij guard_start;
//   ij guard_pos;
//   while (getline(fin, line)) {
//     vector<char> row;
//     for (int i = 0; i < line.length(); ++i) {
//       char c = line[i];
//       row.push_back(c);
//       if (c == '^') {
//         guard_pos = {int(grid.size()), i};
//         guard_start = guard_pos;
//       }
//     }
//     grid.push_back(row);
//   }
//   vector<vector<vector<bool>>> seen(
//       grid.size(),
//       vector<vector<bool>>(grid[0].size(), vector<bool>(4, false)));
//   vector<vector<bool>> locations(grid.size(),
//                                  vector<bool>(grid[0].size(), false));
//   dir cur_dir = dir::UP;
//   delta cur_delta = get_delta(cur_dir);
//   int count = 0;

//   // cout << seen.size() << endl;
//   // cout << seen[0].size() << endl;
//   // cout << seen[0][0].size() << endl;
//   // cout << guard_pos << endl;
//   // cout << (!seen[guard_pos.i][guard_pos.j][diri(cur_dir)]) << endl;
//   // cout << "here..." << endl;
//   // cout << "!oob(int(grid.size()), int(grid[0].size()), guard_pos): "
//   //      << !oob(int(grid.size()), int(grid[0].size()), guard_pos) << endl;
//   // cout << "!seen[guard_pos.i][guard_pos.j][diri(cur_dir)]: "
//   //      << !seen[guard_pos.i][guard_pos.j][diri(cur_dir)] << endl;
//   int loop_pos_count = 0;
//   for (int i = 0; i < grid.size(); ++i) {
//     for (int j = 0; j < grid[0].size(); ++j) {
//       if (i != guard_start.i || j != guard_start.j && grid[i][j] == '.') {
//         // mark the grid
//         grid[i][j] = 'O';
//         if (does_loop(grid, guard_pos)) {
//           loop_pos_count++;
//         }
//         grid[i][j] = '.';
//       }
//     }
//   }
//   while (!oob(int(grid.size()), int(grid[0].size()), guard_pos) &&
//          !seen[guard_pos.i][guard_pos.j][diri(cur_dir)]) {
//     // cout << "-----" << endl;
//     // cout << "guard position: " << guard_pos << endl;
//     seen[guard_pos.i][guard_pos.j][diri(cur_dir)] = true;
//     if (!locations[guard_pos.i][guard_pos.j]) {
//       locations[guard_pos.i][guard_pos.j] = true;
//       count++;
//     }

//     ij next_pos{.i = (guard_pos.i + cur_delta.di),
//                 .j = (guard_pos.j + cur_delta.dj)};
//     // switch directions clause
//     while (!oob(int(grid.size()), int(grid[0].size()), next_pos) &&
//            grid[next_pos.i][next_pos.j] == '#') {
//       cur_dir = rotate(cur_dir);
//       cur_delta = get_delta(cur_dir);
//       next_pos = {.i = (guard_pos.i + cur_delta.di),
//                   .j = (guard_pos.j + cur_delta.dj)};
//     }
//     guard_pos = next_pos;
//   }

//   // for (int i = 0; i < grid.size(); ++i) {
//   //   for (int j = 0; j < grid[0].size(); ++j) {
//   //     if (locations[i][j]) {
//   //       cout << "X";
//   //     } else {
//   //       cout << grid[i][j];
//   //     }
//   //   }
//   //   cout << endl;
//   // }
//   cout << count << endl;
// }

int main() {
  solve2();
  return 0;
}