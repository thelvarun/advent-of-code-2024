#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

ll checksum(vector<string>& v) {
  ll sum = 0;
  for (int i = 0; i < v.size(); ++i) {
    if (v[i] == ".") {
      return sum;
    }
    sum += i * stoll(v[i]);
  }
  return sum;
}

ll checksum2(vector<string>& v) {
  ll sum = 0;
  for (ll i = 0; i < v.size(); ++i) {
    if (v[i] != ".") {
      // cout << "stoll[vi]: " << stoll(v[i]) << endl;
      sum += (i * stoll(v[i]));
      if (sum < 0) {
        // cout << "overflow..." << endl;
      }
    }
  }
  return sum;
}

struct block {
  string id = "";
  int start_index = -1;
  int size = 0;

  string s() const {
    string output = "{";
    output += ("id=" + id) + (", start_index=" + to_string(start_index)) +
              (", size=" + to_string(size)) + "}";
    return output;
  }
};

ostream& operator<<(ostream& o, const block& rhs) {
  o << rhs.s();
  return o;
}

void compress(vector<string>& v, vector<int>& empty_block_indices) {
  int back_of_v_ptr = v.size() - 1;
  // cout << "empty block indices: " << empty_block_indices.size() << endl;
  for (int i = 0; i < empty_block_indices.size(); ++i) {
    // cout << "-----" << endl;
    // cout << "back_of_v_ptr: " << back_of_v_ptr
    //      << " = v[back_of_v_ptr]: " << char(v[back_of_v_ptr]) << endl;
    while (!isdigit(v[back_of_v_ptr][0])) {
      back_of_v_ptr--;
    }
    // cout << "back_of_v_ptr: " << back_of_v_ptr
    //      << " = v[back_of_v_ptr]: " << char(v[back_of_v_ptr]) << endl;
    if (empty_block_indices[i] > back_of_v_ptr) {
      break;
    }
    v[empty_block_indices[i]] = v[back_of_v_ptr];
    v[back_of_v_ptr] = ".";
    // for (int j = 0; j < v.size(); ++j) {
    //   cout << v[j];
    // }
    // cout << endl;
  }
}

void compress2(vector<string>& v, vector<block>& file_blocks,
               vector<block>& free_blocks) {
  for (int file_i = (int)(file_blocks.size() - 1); file_i >= 1; --file_i) {
    // now search through our empty blocks from left to right. choose the first
    // empty block that has enough space to fit the file block, and then move
    // the file block to the free space (update v).
    // cout << "---" << endl;
    // cout << "current file i: " << file_i << endl;
    block file_block = file_blocks[file_i];
    for (int free_i = 0; free_i < (int)free_blocks.size(); ++free_i) {
      // cout << "freei: " << free_i << endl;
      block& free_block = free_blocks[free_i];
      // We found a free block that's big enough, time to move the file block.
      // Make sure update the free block's size too.
      if (free_block.size >= file_block.size &&
          free_block.start_index < file_block.start_index) {
        // cout << "-----" << endl;
        for (int vi = 0; vi < file_block.size; ++vi) {
          // cout << "got in here 2..." << endl;
          v[free_block.start_index + vi] = file_block.id;
          v[file_block.start_index + vi] = ".";
        }
        free_block.size -= file_block.size;
        free_block.start_index += file_block.size;
        // cout << "breaking here..." << endl;
        break;
      }
    }
  }
}

void solve() {
  ifstream fin("day9.in");
  string line;
  getline(fin, line);
  vector<string> v;
  bool is_file_block = true;
  int file_id = 0;
  vector<block> file_blocks;
  vector<block> free_blocks;
  vector<int> empty_block_indices;
  for (char c : line) {
    int int_c = c - '0';
    string appender = ".";
    if (is_file_block) {
      appender = to_string(file_id);
      file_id++;
    }
    block b{.id = appender, .start_index = int(v.size()), .size = int_c};
    if (is_file_block) {
      file_blocks.push_back(b);
    } else {
      free_blocks.push_back(b);
    }
    for (int i = 0; i < int_c; ++i) {
      if (!is_file_block) {
        empty_block_indices.push_back(v.size());
      }
      v.push_back(appender);
    }
    is_file_block = !is_file_block;
  }

  // for (int i = 0; i < v.size(); ++i) {
  //   cout << v[i];
  // }
  // cout << endl;
  // for (auto& block : file_blocks) {
  //   cout << block << endl;
  // }
  // for (auto& block : free_blocks) {
  //   cout << block << endl;
  // }
  // cout << endl;

  // for (int i = 0; i < empty_block_indices.size(); ++i) {
  //   cout << empty_block_indices[i];
  // }
  // cout << endl;

  compress(v, empty_block_indices);
  // for (int i = 0; i < v.size(); ++i) {
  //   cout << v[i];
  // }
  // cout << endl;
  ll sum = checksum(v);
  cout << sum << endl;
  // for (int i = 0; i < v.size(); ++i) {
  //   if (v[i] == ".") {
  //     break;
  //   }
  //   cout << v[i] << ", ";
  // }
  // cout << endl;

  // cout << "0099811188827773336446555566.............." << endl;
  return;
}

void solve2() {
  ifstream fin("day9.in");
  ofstream fout("day9.out");
  string line;
  getline(fin, line);
  vector<string> v;
  bool is_file_block = true;
  int file_id = 0;
  vector<block> file_blocks;
  vector<block> free_blocks;
  vector<int> empty_block_indices;
  for (char c : line) {
    int int_c = c - '0';
    string appender = ".";
    if (is_file_block) {
      appender = to_string(file_id);
      file_id++;
    }
    block b{.id = appender, .start_index = int(v.size()), .size = int_c};
    if (is_file_block) {
      file_blocks.push_back(b);
    } else {
      free_blocks.push_back(b);
    }
    for (int i = 0; i < int_c; ++i) {
      if (!is_file_block) {
        empty_block_indices.push_back(v.size());
      }
      v.push_back(appender);
    }
    is_file_block = !is_file_block;
  }

  for (int i = 0; i < v.size(); ++i) {
    fout << v[i] << ", ";
  }
  // cout << endl;
  // for (auto& block : file_blocks) {
  //   cout << block << endl;
  // }
  // for (auto& block : free_blocks) {
  //   cout << block << endl;
  // }
  // cout << endl;

  // for (int i = 0; i < empty_block_indices.size(); ++i) {
  //   cout << empty_block_indices[i];
  // }
  // cout << endl;
  compress2(v, file_blocks, free_blocks);
  // for (int i = 0; i < v.size(); ++i) {
  //   cout << v[i];
  // }
  // cout << endl;
  // cout << "00992111777.44.333....5555.6666.....8888.." << endl;
  ll sum = checksum2(v);
  cout << sum << endl;
  // for (int i = 0; i < v.size(); ++i) {
  //   if (v[i] == ".") {
  //     break;
  //   }
  //   cout << v[i] << ", ";
  // }
  // cout << endl;

  // cout << "0099811188827773336446555566.............." << endl;
  return;
}

int main() {
  solve2();
  return 0;
}