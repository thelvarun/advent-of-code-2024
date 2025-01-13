#include <cctype>
#include <fstream>
#include <iostream>

using namespace std;

struct output {
  int next_index = -1;
  long long mul_value = 0;
  bool valid_mul_string = false;
};

// given a string s and a start index si, parse through the string starting at
// si, and see if there exists a valid mul string including and after si.
output parse_mul(string& s, int si) {
  // cout << "-----" << endl;
  // cout << "inside parse mul..." << endl;
  // cout << "si: " << si << endl;
  // cin.ignore();
  string mul_string = "mul(";
  for (int i = 0; i < mul_string.size(); ++i) {
    // cout << "i: " << i << endl;
    // cout << "si: " << si << endl;
    // cout << "si + i: " << si + i << endl;
    // cout << "s[si]: " << s[si] << endl;
    // cout << "s[si+i]" << s[si + i] << endl;
    // at the very first point that this string doesn't match, we should return.
    if (s[si + i] != mul_string[i]) {
      cout << "first mul string doesn't match" << endl;
      // cout << "s[si+1]=" << s[si + i] << ", mul_string[i]=" << mul_string[i]
      //      << endl;
      return output{
          .next_index = si + 1, .mul_value = 0, .valid_mul_string = false};
    }
  }

  // by this point, we know there exists a substring mul( starting with si.
  // now, we need to parse X,Y), where X and Y are integers between 1-3
  // characters long.
  int part_2_index = si + mul_string.size();
  int x_size = 0;
  int x = 0;

  // parse the first number
  while (isdigit(s[part_2_index]) && x_size < 3) {
    int dig = s[part_2_index] - '0';
    cout << "dig: " << dig << endl;
    x *= 10;
    x += dig;
    x_size++;
    part_2_index++;
  }
  cout << "x: " << x << endl;

  // we ran into a 4 digit or more number
  if (x_size > 3) {
    cout << "x has too much digits" << endl;
    // .next_index = si+1 is conservative but not wrong.
    return output{
        .next_index = si + 1, .mul_value = 0, .valid_mul_string = false};
  }

  // otherwise, we know that s[index] is not a number, so we have to make sure
  // it's a comma
  if (s[part_2_index] != ',') {
    cout << "no comma between x and y" << endl;
    // .next_index = si+1 is conservative but not wrong.
    return output{
        .next_index = si + 1, .mul_value = 0, .valid_mul_string = false};
  }

  // now do the same parsing
  part_2_index++;  // increment to "move past" the comma.
  int y_size = 0;
  int y = 0;

  // parse the first number
  while (isdigit(s[part_2_index]) && y_size < 3) {
    int dig = s[part_2_index] - '0';
    y *= 10;
    y += dig;
    y_size++;
    part_2_index++;
  }

  // we ran into a 4 digit or more number
  if (y_size > 3) {
    cout << "y has too much digits" << endl;
    // .next_index = si+1 is conservative but not wrong.
    return output{
        .next_index = si + 1, .mul_value = 0, .valid_mul_string = false};
  }

  // otherwise, we need to check that the mul last character is a closing
  // parenthesis
  if (s[part_2_index] != ')') {
    cout << "no ) at end of mul" << endl;
    // .next_index = si+1 is conservative but not wrong.
    return output{
        .next_index = si + 1, .mul_value = 0, .valid_mul_string = false};
  }

  cout << "x: " << x << ", y: " << y << endl;
  return output{.next_index = part_2_index + 1,
                .mul_value = (x * y),
                .valid_mul_string = true};
}

void solve() {
  ifstream fin("day3.in");
  string s;
  string total;
  long long sum = 0;
  string do_string = "do()";
  string dont_string = "don't()";
  bool enabled = true;
  // inefficient reading... maybe fix...
  while (getline(fin, s)) {
    total += s;
  }
  for (int i = 0; i < total.size(); ++i) {
    if (total[i] == 'd') {
      // first check do
      if (i + do_string.size() < total.size()) {
        if (total.substr(i, do_string.size()) == do_string) {
          enabled = true;
          continue;
        }
      }

      if (i + dont_string.size() < total.size()) {
        if (total.substr(i, dont_string.size()) == dont_string) {
          enabled = false;
          continue;
        }
      }
    } else if (total[i] == 'm' && enabled) {
      output o = parse_mul(total, i);
      if (o.valid_mul_string) {
        sum += o.mul_value;
      }
    }
  }
  cout << sum << endl;
}

int main() {
  solve();
  return 0;
}