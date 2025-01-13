#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

// Not good practice, but for convenience.
using namespace std;

string_view XMAS = "XMAS";
constexpr int XMAS_LEN = 4;

enum class diagonal {
  UP_LEFT = 0,
  UP_RIGHT = 1,
  DOWN_RIGHT = 2,
  DOWN_LEFT = 3
};

struct delta {
  int di = 0;
  int dj = 0;
};

delta get_delta(diagonal d) {
  switch (d) {
    case diagonal::UP_LEFT:
      return delta{.di = -1, .dj = -1};
    case diagonal::UP_RIGHT:
      return delta{.di = -1, .dj = 1};
    case diagonal::DOWN_RIGHT:
      return delta{.di = 1, .dj = 1};
    case diagonal::DOWN_LEFT:
      return delta{.di = 1, .dj = -1};
  }
}

// Checks if there is a forward horizontal XMAS string starting at indices si
// and sj.
bool horizontal_forwards_match(vector<vector<char>>& grid, int si, int sj) {
  if (sj + XMAS_LEN > grid[si].size()) {
    return false;
  }
  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si][sj + i] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a backwards horizontal XMAS string starting at indices si
// and sj.
bool horizontal_backwards_match(vector<vector<char>>& grid, int si, int sj) {
  if (si == 0 && sj == 7) {
    cout << "---" << endl;
    cout << "in horizontal_backwards_match... " << endl;
  }
  if (sj - XMAS_LEN + 1 < 0) {
    return false;
  }
  if (si == 0 && sj == 7) {
    cout << "made it past the first check" << endl;
  }
  for (int i = 0; i < XMAS_LEN; ++i) {
    if (si == 0 && sj == 7) {
      cout << grid[si][sj - i] << endl;
    }
    if (grid[si][sj - i] != XMAS[i]) {
      return false;
    }
  }
  if (si == 0 && sj == 7) {
    cout << "this works" << endl;
  }
  return true;
}

// Checks if there is a upwards vertical XMAS string starting at indices si
// and sj.
bool vertical_upwards_match(vector<vector<char>>& grid, int si, int sj) {
  if (si - XMAS_LEN + 1 < 0) {
    return false;
  }
  // cout << "here" << endl;
  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si - i][sj] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a upwards vertical XMAS string starting at indices si
// and sj.
bool vertical_downwards_match(vector<vector<char>>& grid, int si, int sj) {
  if (si + XMAS_LEN > grid.size()) {
    return false;
  }
  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si + i][sj] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a up left diagonal XMAS string starting at indices si
// and sj.
bool diagonal_up_left_match(vector<vector<char>>& grid, int si, int sj) {
  delta d = get_delta(diagonal::UP_LEFT);
  if (d.di * XMAS_LEN + si + 1 < 0) {
    return false;
  }

  if (d.dj * XMAS_LEN + sj + 1 < 0) {
    return false;
  }

  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si + d.di * i][sj + d.dj * i] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a up right diagonal XMAS string starting at indices si
// and sj.
bool diagonal_up_right_match(vector<vector<char>>& grid, int si, int sj) {
  delta d = get_delta(diagonal::UP_RIGHT);
  if (d.di * XMAS_LEN + si + 1 < 0) {
    return false;
  }

  if (d.dj * XMAS_LEN + sj > grid[si].size()) {
    return false;
  }

  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si + d.di * i][sj + d.dj * i] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a up right diagonal XMAS string starting at indices si
// and sj.
bool diagonal_down_right_match(vector<vector<char>>& grid, int si, int sj) {
  delta d = get_delta(diagonal::DOWN_RIGHT);
  if (d.di * XMAS_LEN + si > grid.size()) {
    return false;
  }

  if (d.dj * XMAS_LEN + sj > grid[si].size()) {
    return false;
  }

  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si + d.di * i][sj + d.dj * i] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

// Checks if there is a up right diagonal XMAS string starting at indices si
// and sj.
bool diagonal_down_left_match(vector<vector<char>>& grid, int si, int sj) {
  delta d = get_delta(diagonal::DOWN_LEFT);
  if (d.di * XMAS_LEN + si > grid.size()) {
    return false;
  }

  if (d.dj * XMAS_LEN + sj + 1 < 0) {
    return false;
  }

  for (int i = 0; i < XMAS_LEN; ++i) {
    if (grid[si + d.di * i][sj + d.dj * i] != XMAS[i]) {
      return false;
    }
  }
  return true;
}

int count_occurences(vector<vector<char>>& grid, int si, int sj) {
  int count = 0;

  int hf = horizontal_forwards_match(grid, si, sj);
  int hb = horizontal_backwards_match(grid, si, sj);

  int vu = vertical_upwards_match(grid, si, sj);
  int vd = vertical_downwards_match(grid, si, sj);

  int ddl = diagonal_down_left_match(grid, si, sj);
  int ddr = diagonal_down_right_match(grid, si, sj);

  int dul = diagonal_up_left_match(grid, si, sj);
  int dur = diagonal_up_right_match(grid, si, sj);

  // cout << "hf: " << hf << endl;
  // cout << "hb: " << hb << endl;
  // cout << "vu: " << vu << endl;
  // cout << "vd: " << vd << endl;
  // cout << "ddl: " << ddl << endl;
  // cout << "ddr: " << ddr << endl;
  // cout << "dul: " << dul << endl;
  // cout << "dur: " << dur << endl;

  count += (hf + hb + vu + vd + ddl + ddr + dul + dur);
  return count;
}

bool inb(int grid_row_size, int grid_col_size, int i, int j) {
  return 0 <= i && i < grid_row_size && 0 <= j && j < grid_col_size;
}

// Given a grid and si and sj pointing to an A, return a true if this is a "X"
// of MAS.
bool is_mas_x(vector<vector<char>>& grid, int si, int sj) {
  int top_left_i = si - 1;
  int top_left_j = sj - 1;
  int bottom_right_i = si + 1;
  int bottom_right_j = sj + 1;

  int top_right_i = si - 1;
  int top_right_j = sj + 1;
  int bottom_left_i = si + 1;
  int bottom_left_j = sj - 1;

  if (!inb(grid.size(), grid[si].size(), top_left_i, top_left_j)) {
    return false;
  }

  if (!inb(grid.size(), grid[si].size(), bottom_right_i, bottom_right_j)) {
    return false;
  }

  if (!inb(grid.size(), grid[si].size(), top_right_i, top_right_j)) {
    return false;
  }

  if (!inb(grid.size(), grid[si].size(), bottom_left_i, bottom_left_j)) {
    return false;
  }

  bool top_right_to_bottom_left_diagonal_good = false;
  bool bottom_left_to_top_right_diagonal_good = false;

  if (grid[top_left_i][top_left_j] == 'M') {
    if (grid[bottom_right_i][bottom_right_j] == 'S') {
      top_right_to_bottom_left_diagonal_good = true;
    }
  } else if (grid[top_left_i][top_left_j] == 'S') {
    if (grid[bottom_right_i][bottom_right_j] == 'M') {
      top_right_to_bottom_left_diagonal_good = true;
    }
  }

  if (grid[bottom_left_i][bottom_left_j] == 'M') {
    if (grid[top_right_i][top_right_j] == 'S') {
      bottom_left_to_top_right_diagonal_good = true;
    }
  } else if (grid[bottom_left_i][bottom_left_j] == 'S') {
    if (grid[top_right_i][top_right_j] == 'M') {
      bottom_left_to_top_right_diagonal_good = true;
    }
  }

  return top_right_to_bottom_left_diagonal_good &&
         bottom_left_to_top_right_diagonal_good;
}

void solve() {
  ifstream fin("day4.in");
  vector<vector<char>> grid;
  string s;
  while (getline(fin, s)) {
    vector<char> row;
    for (char c : s) {
      row.push_back(c);
    }
    grid.push_back(row);
  }

  int sum = 0;
  // cout << "grid size: " << grid.size() << endl;
  // cout << "grid[0].size(): " << grid[0].size() << endl;
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[i].size(); ++j) {
      // cout << "(" << i << "," << j << ")" << endl;
      if (grid[i][j] == 'A') {
        // cout << "-----" << endl;
        // cout << "found x at (" << i << "," << j << ")" << endl;
        // int occurences = count_occurences(grid, i, j);
        // cout << "num of occurences of XMAS at (" << i << "," << j
        //      << "): " << occurences << endl;
        if (is_mas_x(grid, i, j)) {
          sum++;
        }
        // sum += occurences;
      }
    }
  }
  cout << sum << endl;
}

int main() {
  solve();
  return 0;
}