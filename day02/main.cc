#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

enum class status {
  MONOTONICALLY_INCREASING = 0,
  MONOTONICALLY_DECREASING = 1,
  NOT_MONOTINCALLY_MOVING = 2,
};

bool diff_is_in_range(int a, int b, bool is_increasing_sequence) {
  if (is_increasing_sequence) {
    return (b - a) >= 1 && (b - a) <= 3;
  } else {
    return (a - b) >= 1 && (a - b) <= 3;
  }
}

void solve1() {
  ifstream fin("day2.in");
  string s;
  int count = 0;
  while (getline(fin, s)) {
    // cout << "---" << endl;
    // cout << "s: " << s << endl;
    bool is_increasing_sequence = false;
    int prev = -1;
    int cur = -1;
    int x;
    stringstream ss(s);
    bool valid = true;
    while (!ss.eof()) {
      ss >> x;
      // cout << "x: " << x << endl;
      if (prev == -1 && cur == -1) {
        // cout << "in first num case" << endl;
        // we're on the first number in the sequence
        cur = x;
      } else if (cur != -1 && prev == -1) {
        // cout << "in second num case" << endl;
        // we're at the second number in the sequence. here, we can diagnose
        // if this will be an increasing sequence or decreasing sequence. even
        // here, we must also check if the difference is out of range.
        prev = cur;
        cur = x;

        if (cur > prev) {
          is_increasing_sequence = true;
        } else {
          is_increasing_sequence = false;
        }

        if (!diff_is_in_range(prev, cur, is_increasing_sequence)) {
          valid = false;
          break;
        }
      } else {
        // cout << "in third num case" << endl;
        prev = cur;
        cur = x;
        if (!diff_is_in_range(prev, cur, is_increasing_sequence)) {
          valid = false;
          break;
        }
      }
    }
    if (valid) count++;
  }
  cout << count << endl;
}

bool in_range(int greater, int smaller) {
  return (greater - smaller) >= 1 && (greater - smaller) <= 3;
}

bool is_valid_report(vector<int> report, int skip_first_or_second_element) {
  // cout << "-------" << endl;
  bool skipped_num_already = false;
  // we need to establish the type of report this is (increasing, decreasing)
  int prev = 0;
  int cur = 1;
  if (skip_first_or_second_element == 0) {
    prev = 1;
    cur = 2;
    skipped_num_already = true;
  } else if (skip_first_or_second_element == 1) {
    report[1] = report[0];
    prev = 1;
    cur = 2;
    skipped_num_already = true;
  }

  // cout << "p1: " << prev << ", p2: " << cur << endl;
  bool is_increasing_sequence = (report[cur] > report[prev]);
  // cout << "is increasing?: " << is_increasing_sequence << endl;
  // cout << "report.size(): " << report.size() << endl;
  for (int i = cur; i < report.size(); ++i) {
    // cout << "I: " << i << endl;
    if (!diff_is_in_range(report[prev], report[i], is_increasing_sequence)) {
      // cout << "diff not in range" << endl;
      // cout << "prev: " << prev << ", i: " << i << endl;
      if (skipped_num_already) {
        return false;
      }
      // otherwise, we will skip this number
      skipped_num_already = true;
      continue;
    }
    prev = i;
  }
  return true;
}

// assumes the vector does not have any skips remaining
status monotonically_moving(vector<int> v) {
  if (v.size() == 1) return status::MONOTONICALLY_INCREASING;
  bool is_increasing = false;
  if (v[1] > v[0]) {
    is_increasing = true;
  } else if (v[1] == v[0]) {
    return status::NOT_MONOTINCALLY_MOVING;
  }

  for (int i = 1; i < v.size(); ++i) {
    if (is_increasing) {
      if (v[i] <= v[i - 1] || abs(v[i] - v[i - 1]) < 1 ||
          abs(v[i] - v[i - 1]) > 3) {
        return status::NOT_MONOTINCALLY_MOVING;
      }
    } else {
      if (v[i] >= v[i - 1] || abs(v[i] - v[i - 1]) < 1 ||
          abs(v[i] - v[i - 1]) > 3) {
        return status::NOT_MONOTINCALLY_MOVING;
      }
    }
  }

  return (is_increasing ? status::MONOTONICALLY_INCREASING
                        : status::MONOTONICALLY_DECREASING);
}

void solve3() {
  ifstream fin("day2.in");
  string s;
  int count = 0;
  while (getline(fin, s)) {
    vector<int> report;
    int t;
    stringstream ss(s);
    while (!ss.eof()) {
      ss >> t;
      report.push_back(t);
    }

    // cout << "---" << endl;
    // cout << "report: ";
    // for (int x : report) {
    //   cout << x << " ";
    // }
    cout << endl;

    for (int i = 0; i < report.size(); ++i) {
      vector<int> copy;
      for (int c : report) {
        copy.push_back(c);
      }
      // cout << "copy: ";
      // for (int c : copy) {
      //   cout << c << " ";
      // }
      // cout << endl;
      copy.erase(copy.begin() + i);
      // cout << "copy after erasing: ";
      // for (int c : copy) {
      //   cout << c << " ";
      // }
      if (monotonically_moving(copy) != status::NOT_MONOTINCALLY_MOVING) {
        count++;
        break;
      }
    }

    // // now we do an edge case check -- if the first two elements aren't
    // valid,
    // // we need to inform our validator function.
    // int first_elem = report[0];
    // int second_elem = report[1];
    // bool valid_report = false;
    // if (abs(second_elem - first_elem) < 1 ||
    //     abs(second_elem - first_elem) > 3) {
    //   valid_report = is_valid_report(report, 0) || is_valid_report(report,
    //   1); if (valid_report) {
    //     count++;
    //   }
    // } else {
    //   // cout << "in here" << endl;
    //   valid_report = is_valid_report(report, -1);
    //   // cout << "valid report: " << valid_report << endl;
    //   if (valid_report) {
    //     count++;
    //   }
    // }
  }
  cout << count << endl;
}

void solve() {
  ifstream fin("day2.in");
  string s;
  int count = 0;
  while (getline(fin, s)) {
    vector<int> report;
    int t;
    stringstream ss(s);
    while (!ss.eof()) {
      ss >> t;
      report.push_back(t);
    }

    vector<int> report_copy(report);

    // first, check if this will work if we remove either the first or the
    // second element.
    status remove_first =
        monotonically_moving(vector<int>(report.begin() + 1, report.end()));
    if (remove_first != status::NOT_MONOTINCALLY_MOVING) {
      count++;
      continue;
    }

    report_copy[1] = report_copy[0];
    status remove_second =
        monotonically_moving(vector<int>(report.begin() + 1, report.end()));
    if (remove_second != status::NOT_MONOTINCALLY_MOVING) {
      count++;
      continue;
    }

    // otherwise, now we've established which direction we are moving in, so we
    // can simply do our regular check.
    bool is_increasing = report[1] > report[0];
    bool skipped_one = false;
    bool works = true;
    int prev = 0;
    for (int i = 1; i < report.size(); ++i) {
      if ((abs(report[i] - report[prev]) < 1 ||
           abs(report[i] - report[prev]) > 3) ||
          (is_increasing && report[i] <= report[prev]) ||
          (!is_increasing && report[i] >= report[prev])) {
        if (skipped_one) {
          works = false;
          break;
        }

        skipped_one = true;
        continue;
      }
      prev = i;
    }

    if (works) {
      count++;
    }
  }
  cout << count << endl;
}

int main() {
  solve3();
  return 0;
}