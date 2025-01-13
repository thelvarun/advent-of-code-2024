#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

typedef long long ll;
typedef pair<long, long> pll;

struct robot {
  pll pos = {-1, -1};
  pll velocity = {0, 0};
};

ostream& operator<<(ostream& o, const pll& rhs) {
  o << "{" << rhs.first << "," << rhs.second << "}";
  return o;
}

pll operator+(const pll& lhs, const pll& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

pll operator*(const pll& lhs, ll factor) {
  return {lhs.first * factor, lhs.second * factor};
}

robot parse_input(string& s) {
  stringstream ss(s);
  string starting_pos_str;
  string velocity_str;
  ss >> starting_pos_str;
  ll pos_x = stoll(starting_pos_str.substr(starting_pos_str.find('=') + 1,
                                           starting_pos_str.find(',')));
  ll pos_y = stoll(starting_pos_str.substr(starting_pos_str.find(',') + 1));
  pll robot_pos(pos_x, pos_y);
  ss >> velocity_str;
  ll vel_x = stoll(
      velocity_str.substr(velocity_str.find('=') + 1, velocity_str.find(',')));
  ll vel_y = stoll(velocity_str.substr(velocity_str.find(',') + 1));
  pll velocity{vel_x, vel_y};
  return robot{.pos = robot_pos, .velocity = velocity};
}

void simulate(robot& r, ll num_simulations, ll max_x, ll max_y) {
  // simulate the robot movement after 1 second from the current starting
  // position.
  pll adj_vel = r.velocity * num_simulations;
  if (adj_vel.first < 0) {
    adj_vel.first = ((adj_vel.first * -1) % max_x) * -1;
  } else if (adj_vel.first >= max_x) {
    adj_vel.first %= max_x;
  }

  if (adj_vel.second < 0) {
    adj_vel.second = ((adj_vel.second * -1) % max_y) * -1;
  } else if (adj_vel.second >= max_y) {
    adj_vel.second %= max_y;
  }

  pll new_pos = r.pos + adj_vel;
  if (new_pos.first < 0) {
    new_pos.first += max_x;
  } else if (new_pos.first >= max_x) {
    new_pos.first %= max_x;
  }

  if (new_pos.second < 0) {
    new_pos.second += max_y;
  } else if (new_pos.second >= max_y) {
    new_pos.second %= max_y;
  }

  r.pos = new_pos;
}

ll calc_safety_factor(vector<robot>& robots, ll max_x, ll max_y) {
  ll top_left_quadrant = 0;
  ll top_right_quadrant = 0;
  ll bottom_left_quadrant = 0;
  ll bottom_right_quadrant = 0;
  for (robot robot : robots) {
    if (robot.pos.first < (max_x / 2)) {
      // in a left quadrant
      if (robot.pos.second < (max_y / 2)) {
        // in a top left quadrant
        top_left_quadrant++;
      } else if (robot.pos.second > (max_y / 2)) {
        // in a bottom left quadrant
        bottom_left_quadrant++;
      }

    } else if (robot.pos.first > (max_x / 2)) {
      // in a right quadrant
      if (robot.pos.second < (max_y / 2)) {
        // in a top right quadrant
        top_right_quadrant++;
      } else if (robot.pos.second > (max_y / 2)) {
        // in a bottom right quadrant
        bottom_right_quadrant++;
      }
    }
  }
  vector<ll> temp{top_left_quadrant, top_right_quadrant, bottom_left_quadrant,
                  bottom_right_quadrant};
  ll product = 1;
  for (ll x : temp) {
    if (x != 0) {
      product *= x;
    }
  }
  return product;
}

void output(vector<robot>& robots, ll max_x, ll max_y) {
  vector<vector<char>> grid(max_x, vector<char>(max_y, ' '));
  for (auto robot : robots) {
    grid[robot.pos.first][robot.pos.second] = '#';
  }

  for (auto& row : grid) {
    for (char c : row) {
      cout << c;
    }
    cout << endl;
  }
}

bool inb(ll i, ll j, ll max_i, ll max_j) {
  return 0 <= i && i < max_i && 0 <= j && j < max_j;
}

bool has_neighbor(ll i, ll j, ll max_i, ll max_j, vector<vector<int>>& grid) {
  vector<ll> di{-1, 0, 1};
  vector<ll> dj{-1, 0, 1};
  for (ll ddi : di) {
    for (ll ddj : dj) {
      if (ddi != 0 || ddj != 0) {
        if (inb(i + ddi, j + ddj, max_i, max_j) &&
            grid[i + ddi][j + ddj] != 0) {
          return true;
        }
      }
    }
  }
  return false;
}

bool is_maybe_christmas_tree(vector<robot>& robots, ll max_x, ll max_y) {
  // put the robots on the grid and see if there's a lot of neighbors
  vector<vector<int>> grid(max_y, vector<int>(max_x, 0));
  for (auto robot : robots) {
    grid[robot.pos.second][robot.pos.first]++;
  }

  ll robot_with_neighbors = 0;
  for (ll i = 0; i < max_y; ++i) {
    for (ll j = 0; j < max_x; ++j) {
      if (grid[i][j] != 0) {
        if (has_neighbor(i, j, grid.size(), grid[0].size(), grid)) {
          // cout << "in here..." << endl;
          robot_with_neighbors += grid[i][j];
        }
      }
    }
  }
  cout << "robots with neighbors: " << robot_with_neighbors
       << ", robots size: " << robots.size() << endl;
  double percentage = (double)robot_with_neighbors / (double)robots.size();
  cout << "percentage: " << percentage << endl;
  return percentage > 0.2;
}

void solve(string file_name) {
  ifstream fin(file_name);
  string line;
  vector<robot> robots;
  while (getline(fin, line)) {
    robot r = parse_input(line);
    robots.push_back(r);
  }

  ll max_x = 101;
  ll max_y = 103;
  ll num_simulations = 100;
  // for (auto& robot : robots) {
  //   simulate(robot, num_simulations, max_x, max_y);
  // }

  for (int i = 0; i < (max_x * max_y); ++i) {
    for (auto& robot : robots) {
      simulate(robot, 1, max_x, max_y);
    }
    cout << "----- " << (i + 1) << " seconds passed -----" << endl;
    if (is_maybe_christmas_tree(robots, max_x, max_y)) {
      output(robots, max_x, max_y);
    }
  }

  ll safety_factor = calc_safety_factor(robots, max_x, max_y);
  cout << safety_factor << endl;
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
