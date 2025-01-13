#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <unordered_map>

using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
typedef pair<long double, long double> pdd;

struct custom_hash {
  size_t operator()(const pair<ll, ll>& x) const {
    return hash<ll>()((x.first) ^ ((x.second) << 32));
  }
};

ostream& operator<<(ostream& o, const pll& rhs) {
  o << "(" << rhs.first << "," << rhs.second << ")";
  return o;
}

pll operator+(const pll& lhs, const pll& rhs) {
  return {lhs.first + rhs.first, lhs.second + rhs.second};
}

pll operator-(const pll& lhs, const pll& rhs) {
  return {lhs.first - rhs.first, lhs.second - rhs.second};
}

struct parse_input_struct {
  pll a;
  pll b;
  pll prize;
};

pll parse_button(string& line) {
  stringstream ss(line);
  string token;
  ss >> token;  // Button
  ss >> token;  // Letter
  ss >> token;  // X...
  ll x = stoll(token.substr(token.find('+') + 1, token.length() - 1));
  ss >> token;  // Y...
  ll y = stoll(token.substr(token.find('+') + 1));
  return {x, y};
}

pll parse_prize(string& line) {
  stringstream ss(line);
  string token;
  ss >> token;  // Prize
  ss >> token;  // X...
  ll x = stoll(token.substr(token.find('=') + 1, token.length() - 1));
  ss >> token;  // Y...
  ll y = stoll(token.substr(token.find('=') + 1));
  return {x, y};
}

parse_input_struct parse_input(string& line1, string& line2, string& line3) {
  return {parse_button(line1), parse_button(line2), parse_prize(line3)};
}

struct elem {
  pll xy = {-1, -1};
  ll tokens = -1;
};

unordered_map<pll, ll, custom_hash> cache;

ll foo(pll cur, ll cur_tokens, pll a, pll b) {
  // cout << "cur: " << cur << endl;
  if (cur.first < 0 || cur.second < 0) {
    return -1;
  }

  if (cache.find(cur) != cache.end()) {
    if (cache[cur] == -1) {
      return -1;
    }
    return cur_tokens + cache[cur];
  }

  // choose the minimum between a and b
  ll a_result = foo(cur - a, cur_tokens + 3, a, b);
  ll b_result = foo(cur - b, cur_tokens + 1, a, b);
  if (a_result == -1 && b_result == -1) {
    cache[cur] = -1;
  } else if (a_result == -1 && b_result != -1) {
    cache[cur] = b_result;
  } else if (a_result != -1 && b_result == -1) {
    cache[cur] = a_result;
  } else {
    cache[cur] = min(a_result, b_result);
  }
  return cache[cur];
}

ll lin_eq(pdd a, pdd b, pdd prize) {
  // cout << "---" << endl;
  // cout << "a: " << a << ", b: " << b << ", prize: " << prize << endl;
  // // Solve the set of linear equations for the solution.
  // long double a_button_press =
  //     (prize.second - (prize.first * b.second / b.first)) /
  //     ((a.second - (a.first * b.second / b.first)));

  // long double b_button_press =
  //     (prize.first - a_button_press * a.first) / b.first;

  // // Now we need to check if this "worked" - we can only push the buttons an
  // // integer number of times, so if we end up with a double result, then we
  // know
  // // this combination won't work.
  // long double intpart1;
  // long double intpart2;
  // if (modf(a_button_press, &intpart1) != 0.0 ||
  //     modf(b_button_press, &intpart2) != 0.0) {
  //   return -1;
  // }

  // if (a_button_press < 0 || b_button_press < 0) {
  //   return -1;
  // }
  // ll a_cost = (ll)a_button_press * 3;
  // ll b_cost = (ll)b_button_press * 1;
  // return a_cost + b_cost;
  long double a_button_press =
      (prize.first * b.second - prize.second * b.first) /
      (a.first * b.second - a.second * b.first);
  long double b_button_press =
      (prize.first - a_button_press * a.first) / b.first;

  long double intpart1;
  long double intpart2;
  if (modf(a_button_press, &intpart1) != 0.0 ||
      modf(b_button_press, &intpart2) != 0.0) {
    return -1;
  }
  return (a_button_press * 3 + b_button_press);
}

void solve(string file_name) {
  ifstream fin(file_name);
  string line1;
  string line2;
  string line3;
  ll output = 0;
  while (getline(fin, line1)) {
    getline(fin, line2);
    getline(fin, line3);
    parse_input_struct pis = parse_input(line1, line2, line3);
    pis.prize.first += 10000000000000;
    pis.prize.second += 10000000000000;
    pdd a = {(long double)pis.a.first, (long double)pis.a.second};
    pdd b = {(long double)pis.b.first, (long double)pis.b.second};
    pdd prize = {(long double)pis.prize.first, (long double)pis.prize.second};
    // cout << "---" << endl;
    ll min_token = lin_eq(a, b, prize);
    if (min_token != -1) {
      output += min_token;
    }
    getline(fin, line1);  // skip the blank line
  }
  cout << output << endl;
  return;
}

// void exploration() {
//   ll offset = 10000000000000;
//   ll ax = 94;
//   ll ay = 34;
//   ll zx = (offset + 8400);
//   ll zy = (offset + 5400);

//   // ll ay_rem = zy % ay;
//   ll ax_rem = zx % ax;
//   ll ax_quo = zx / ax;
//   cout << "ax_rem: " << ax_rem << endl;
//   cout << "ax_quo: " << ax_quo << endl;
//   cout << "ax * ax_quo: " << (ax * ax_quo) << endl;
//   cout << "ax * ax_quo + ax_rem: " << ax * ax_quo + ax_rem << endl;
//   // cout << "ay_rem: " << ay_rem << endl;

//   // ll ay_quo = zy / ay;
//   // cout << "ay_quo: " << ay_quo << endl;

//   // now let's look at the b button
//   ll bx = 22;
//   ll axbx = ax + bx;
//   // cout << (zx % (ax * ax_quo + 0 * bx)) << endl;
//   cout << (zx % (ax * (ax_quo - 1) + 0 * bx)) << endl;
//   // cout << (zx % (ax * (ax_quo - 1) + 1 * bx)) << endl;
//   cout << (zx % (ax * (ax_quo - 2) + 0 * bx)) << endl;
//   cout << (zx % (ax * (ax_quo - 3) + 0 * bx)) << endl;
//   cout << (zx % (ax * (ax_quo - 4) + 0 * bx)) << endl;

//   ll alert_count = 0;
//   for (int i = 0; i <= 100; ++i) {
//     ll rem = zx % (ax * (ax_quo - i));
//     cout << "-----" << endl;
//     cout << "rem: " << rem << endl;
//     if (rem % bx == 0) {
//       alert_count++;
//       cout << "++++++ ALERT ++++++" << endl;
//       cout << "rem: " << rem << endl;
//       cout << "ax * (ax_quo - i) + rem / bx * bx) = \n"
//            << ax << " * " << (ax_quo - i) << " + " << rem << " / " << bx
//            << " * " << bx << " = " << (ax * (ax_quo - i) + rem / bx * bx)
//            << endl;
//       cout << "rem / bx: " << (rem / bx) << endl;
//       cout << "zx: " << zx << endl;
//       cout << "this should tell us a valid button press pair is...\n";
//       cout << (ax_quo - i) << " a button presses and " << (rem / bx)
//            << " b button presses..." << endl;
//     }
//     if (alert_count == 5) {
//       break;
//     }
//   }
//   // cout << (zx % (ax * ax_quo + 1 * bx)) << endl;
//   // cout << (zx % (ax * ax_quo + 2 * bx)) << endl;
//   // cout << (zx % (ax * ax_quo + 3 * bx)) << endl;
//   // cout << (zx % (ax * ax_quo + 4 * bx)) << endl;
//   // cout << "axbx: " << axbx << endl;
//   // cout << "zx mod axbx: " << zx % axbx << endl;
//   // ll by = 67;

//   // ll bx_rem = zx % bx;
//   // ll by_rem = zy % by;
//   // cout << "bx_rem: " << bx_rem << endl;
//   // cout << "by_rem: " << by_rem << endl;

//   // ll bx_quo = zx / bx;
//   // ll by_quo = zy / by;
//   // cout << "bx_quo: " << bx_quo << endl;
//   // cout << "by_quo: " << by_quo << endl;
// }

int main(int argc, char* argv[]) {
  // Default to sample.in if no arguments were provided.
  string file_name = "sample.in";
  if (argc == 2) {
    file_name = argv[1];
  }
  // exploration();
  solve(file_name);
  return 0;
}
