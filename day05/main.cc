#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Not good practice, but good for convenience.
using namespace std;

string vects(const vector<int>& v) {
  string s = "{";
  string separator = "";
  for (int e : v) {
    s += (separator + to_string(e));
    separator = ",";
  }
  s += "}";
  return s;
}

vector<int> parse_report_str(string& report_str) {
  stringstream ss(report_str);
  vector<int> report;
  while (ss.good()) {
    string substr;
    getline(ss, substr, ',');
    report.push_back(stoi(substr));
  }
  return report;
}

bool is_valid_report(vector<int>& report,
                     unordered_map<int, vector<int>>& nums_that_come_before) {
  unordered_set<int> seen;
  for (int n : report) {
    // Does n need to come before any numbers?
    if (nums_that_come_before.find(n) != nums_that_come_before.end()) {
      for (int e : nums_that_come_before[n]) {
        if (seen.find(e) != seen.end()) {
          return false;
        }
      }
    }
    seen.insert(n);
  }
  return true;
}

void swap(vector<int>& v, int i, int j) {
  int t = v[i];
  v[i] = v[j];
  v[j] = t;
}

void fix_report(vector<int>& report,
                unordered_map<int, vector<int>>& nums_that_come_before) {
  // cout << "-----" << endl;
  // cout << "in fix report: " << endl;
  // create the indices map
  unordered_map<int, int> report_to_index;
  for (int i = 0; i < report.size(); ++i) {
    report_to_index[report[i]] = i;
  }

  bool retry = false;
  // now we go through and swap any messed up numbers.
  for (int i = 0; i < report.size(); ++i) {
    int num = report[i];
    vector<int>& nums = nums_that_come_before[num];
    // cout << "from map: " << num << ": " << vects(nums) << endl;
    // cin.ignore();
    for (int j = 0; j < nums.size(); ++j) {
      int before_num = nums[j];
      if (report_to_index.find(before_num) != report_to_index.end() &&
          report_to_index[before_num] > i) {
        // cout << "need to swap " << num << " and " << before_num << ": "
        //      << vects(report) << endl;
        // cin.ignore();
        swap(report, i, report_to_index[before_num]);
        // cout << "after swapping " << num << " and " << before_num << ": "
        //      << vects(report) << endl;
        retry = true;
        break;
      }
    }
  }
  if (retry) {
    fix_report(report, nums_that_come_before);
  }
}

void solve() {
  ifstream fin("day5.in");
  string line;
  unordered_map<int, vector<int>> nums_that_must_come_before;
  while (getline(fin, line) && line != "") {
    string delimiter = "|";
    size_t pos = line.find(delimiter);
    int a = stoi(line.substr(0, line.find(delimiter)));
    int b = stoi(line.substr(pos + delimiter.length()));
    if (nums_that_must_come_before.find(a) !=
        nums_that_must_come_before.end()) {
      nums_that_must_come_before[a].push_back(b);
    } else {
      vector<int> v;
      v.push_back(b);
      nums_that_must_come_before[a] = v;
    }
  }

  // for (auto& kv : nums_that_must_come_before) {
  //   cout << kv.first << ":" << vects(kv.second) << endl;
  // }

  int sum = 0;
  int incorrect_sum = 0;
  // now solve the reports
  while (getline(fin, line)) {
    // cout << "---" << endl;
    // cout << "line: " << line << endl;
    vector<int> report = parse_report_str(line);
    // cout << "report: " << vects(report) << endl;
    if (is_valid_report(report, nums_that_must_come_before)) {
      // cout << line << " is a valid report..." << endl;
      // cout << "adding middle num to sum..." << endl;
      if (report.size() % 2 == 0) {
        // cout << "even number of elements in report array: " << vects(report)
        //  << endl;
      } else {
        int middle_num = report[report.size() / 2];
        // cout << "middle num: " << middle_num << endl;
        sum += middle_num;
      }
    } else {
      // cout << "not valid report: " << line << endl;
      fix_report(report, nums_that_must_come_before);
      // cout << vects(report) << endl;
      int middle_num = report[report.size() / 2];
      incorrect_sum += middle_num;
    }
  }
  cout << "sum: " << sum << endl;
  cout << "incorrect sum: " << incorrect_sum << endl;
  return;
}

int main() {
  solve();
  return 0;
}