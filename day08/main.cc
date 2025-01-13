#include <fstream>
#include <iostream>
#include <tuple>
#include <unordered_map>
#include <vector>

using namespace std;

struct ij {
  int i = -1;
  int j = -1;

  string s() const { return "{" + to_string(i) + "," + to_string(j) + "}"; }
};

ostream& operator<<(ostream& o, const ij& rhs) {
  o << rhs.s();
  return o;
}

string sv(vector<vector<char>>& v) {
  string s;
  for (int i = 0; i < v.size(); ++i) {
    for (int j = 0; j < v[i].size(); ++j) {
      s += v[i][j];
    }
    s += '\n';
  }
  return s;
}

string sm(unordered_map<char, vector<ij>>& m) {
  string s;
  for (auto& kv : m) {
    s += kv.first;
    s += ": ";
    string separator;
    for (ij e : kv.second) {
      s += separator + e.s();
      separator = ",";
    }
    s += '\n';
  }
  return s;
}

void pm(unordered_map<char, vector<ij>>& m) { cout << sm(m); }

void pv(vector<vector<char>>& v) { cout << sv(v); }

bool oob(size_t maxi, size_t maxj, int i, int j) {
  return i < 0 || i >= int(maxi) || j < 0 || j >= int(maxj);
}

pair<ij, ij> calc_antinodes(ij a, ij b) {
  // First calculate the "slope"
  int di = b.i - a.i;
  int dj = b.j - a.j;

  // Apply the negative slope to point a and the regular slope to b.
  return {ij{.i = a.i + (-1 * di), .j = a.j + (-1 * dj)},
          ij{b.i + di, b.j + dj}};
}

vector<ij> calc_multiple_antinodes(ij a, ij b, size_t maxi, size_t maxj) {
  // First calculate the "slope"
  int di = b.i - a.i;
  int dj = b.j - a.j;

  // Apply the negative slope to point a and the regular slope to b. Keep
  // calculating antinodes for each until they go out of bounds.
  vector<ij> output;
  ij a_antinode = ij{.i = a.i + (-1 * di), .j = a.j + (-1 * dj)};
  while (!oob(maxi, maxj, a_antinode.i, a_antinode.j)) {
    output.push_back(a_antinode);
    a_antinode.i += (-1 * di);
    a_antinode.j += (-1 * dj);
  }

  ij b_antinode = ij{b.i + di, b.j + dj};
  while (!oob(maxi, maxj, b_antinode.i, b_antinode.j)) {
    output.push_back(b_antinode);
    b_antinode.i += di;
    b_antinode.j += dj;
  }
  return output;
}

void solve() {
  ifstream fin("sample.in");
  unordered_map<char, vector<ij>> antennas;
  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    vector<char> row;
    for (int i = 0; i < line.size(); ++i) {
      // if we come across an antenna, save it's location in our map.
      char c = line[i];
      row.push_back(c);
      if (c != '.') {
        ij loc{int(grid.size()), i};
        if (antennas.find(c) != antennas.end()) {
          antennas[c].push_back(loc);
        } else {
          vector<ij> locations = {loc};
          antennas.insert({c, locations});
        }
      }
    }
    grid.push_back(row);
  }

  // pv(grid);
  // pm(antennas);

  // Go through each pair of antennas and calculate the antinodes. Mark each
  // antinode on a boolean grid as our cache, increment a count every time we
  // come across a new location of an antinode.
  vector<vector<char>> debug(grid);
  vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));
  int count = 0;
  for (const auto& kv : antennas) {
    // cout << "------" << endl;
    // char antenna = kv.first;
    // cout << "finding antindoes for antenna " << antenna << "..." << endl;
    const vector<ij>& locations = kv.second;
    for (int i = 0; i < locations.size(); ++i) {
      ij antenna1 = locations[i];
      // cout << "antenna1: " << antenna1 << endl;
      for (int j = i + 1; j < locations.size(); ++j) {
        ij antenna2 = locations[j];
        // cout << "antenna2: " << antenna2 << endl;
        pair<ij, ij> antinodes = calc_antinodes(antenna1, antenna2);
        ij antinode1 = antinodes.first;
        ij antinode2 = antinodes.second;
        // cout << "antinodes: " << antinode1 << ", " << antinode2 << endl;
        // cout << "checking if " << antinode1 << " will work..." << endl;
        if (!oob(grid.size(), grid[0].size(), antinode1.i, antinode1.j)) {
          // cout << antinode1 << " is in bounds..." << endl;
          if (!seen[antinode1.i][antinode1.j]) {
            // cout << antinode1 << " works. Marking and incrementing
            // count..."
            //      << endl;
            seen[antinode1.i][antinode1.j] = true;
            debug[antinode1.i][antinode1.j] = '#';
            count++;
          }
        }
        // cout << "checking if " << antinode2 << " will work..." << endl;
        if (!oob(grid.size(), grid[0].size(), antinode2.i, antinode2.j)) {
          // cout << antinode2 << " is in bounds..." << endl;
          if (!seen[antinode2.i][antinode2.j]) {
            // cout << antinode2 << " works. Marking and incrementing
            // count..."
            //      << endl;
            seen[antinode2.i][antinode2.j] = true;
            debug[antinode2.i][antinode2.j] = '#';
            count++;
          }
        }
      }
    }
  }
  // pv(debug);
  cout << count << endl;
  return;
}

void solve2() {
  ifstream fin("day8.in");
  unordered_map<char, vector<ij>> antennas;
  vector<vector<char>> grid;
  string line;
  while (getline(fin, line)) {
    vector<char> row;
    for (int i = 0; i < line.size(); ++i) {
      // if we come across an antenna, save it's location in our map.
      char c = line[i];
      row.push_back(c);
      if (c != '.') {
        ij loc{int(grid.size()), i};
        if (antennas.find(c) != antennas.end()) {
          antennas[c].push_back(loc);
        } else {
          vector<ij> locations = {loc};
          antennas.insert({c, locations});
        }
      }
    }
    grid.push_back(row);
  }

  // pv(grid);
  // pm(antennas);

  // Go through each pair of antennas and calculate the antinodes. Mark each
  // antinode on a boolean grid as our cache, increment a count every time we
  // come across a new location of an antinode.
  vector<vector<char>> debug(grid);
  vector<vector<bool>> seen(grid.size(), vector<bool>(grid[0].size(), false));
  int count = 0;
  for (const auto& kv : antennas) {
    // cout << "------" << endl;
    // char antenna = kv.first;
    // cout << "finding antindoes for antenna " << antenna << "..." << endl;
    const vector<ij>& locations = kv.second;
    for (int i = 0; i < locations.size(); ++i) {
      ij antenna1 = locations[i];
      // cout << "antenna1: " << antenna1 << endl;
      for (int j = i + 1; j < locations.size(); ++j) {
        ij antenna2 = locations[j];
        // cout << "antenna2: " << antenna2 << endl;
        for (ij antinode : calc_multiple_antinodes(
                 antenna1, antenna2, grid.size(), grid[0].size())) {
          // cout << antinode1 << " is in bounds..." << endl;
          if (!seen[antinode.i][antinode.j]) {
            // cout << antinode << endl;
            // cout << antinode1 << " works. Marking and incrementing
            // count..."
            //      << endl;
            seen[antinode.i][antinode.j] = true;
            debug[antinode.i][antinode.j] = '#';
            count++;
          }
        }
      }
    }
  }

  // now go through the antinodes themselves and mark them if they haven't been
  // seen already.
  for (const auto& kv : antennas) {
    for (ij location : kv.second) {
      if (!seen[location.i][location.j]) {
        count++;
      }
    }
  }
  // pv(debug);
  cout << count << endl;
  return;
}

int main() {
  solve2();
  return 0;
}