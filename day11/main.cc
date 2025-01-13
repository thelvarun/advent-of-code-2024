#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

typedef long long ll;

struct node {
  node* prev = nullptr;
  node* next = nullptr;
  ll val = -1;
};

void traverse_node(node* sen_head) {
  node* p = sen_head;
  while (p) {
    // cout << "---" << endl;
    // cout << "cur address: " << p << ", cur val: " << p->val
    //      << "\nprev address: " << p->prev << ", next address: " << p->next
    //  << endl;
    cout << p->val << " ";
    p = p->next;
  }
}

ll num_nodes(node* sen_head) {
  node* p = sen_head;
  ll sum = 0;
  while (p) {
    sum++;
    // cout << "---" << endl;
    // cout << "cur address: " << p << ", cur val: " << p->val
    //      << "\nprev address: " << p->prev << ", next address: " << p->next
    //  << endl;
    // cout << p->val << " ";
    p = p->next;
  }
  return sum - 2;
}

void zero_rule(node* node) {
  assert(node->val == 0);
  node->val = 1;
}

void even_dig_rule(node* cur_node, node* new_node) {
  // instead of actually creating two new nodes, we'll change the value of this
  // node and create another node.
  string s = to_string(cur_node->val);
  assert(s.length() % 2 == 0);
  ll first_half = stoll(s.substr(0, s.length() / 2));
  ll second_half = stoll(s.substr(s.length() / 2));
  cur_node->val = first_half;
  new_node->val = second_half;

  cur_node->next->prev = new_node;
  new_node->next = cur_node->next;
  new_node->prev = cur_node;
  cur_node->next = new_node;
}

void mult_by_2024_rule(node* node) { node->val *= 2024; }

void solve() {
  ifstream fin("day11.in");
  string line;
  int x;
  node* sen_head = new node;
  node* sen_tail = new node;
  getline(fin, line);
  stringstream ss(line);
  while (ss >> x) {
    // cout << "+++++" << endl;
    // cout << "x: " << x << endl;
    // cout << "sen head address: " << &sen_head << endl;
    // cout << "sen tail address: " << &sen_tail << endl;
    // cout << "sen head next address: " << sen_head.next << endl;
    // cout << "sen tail prev address: " << sen_tail.prev << endl;
    node* cur = new node;
    // cout << "cur address: " << &cur << endl;
    cur->val = x;

    // This is the first element, add it in.
    if (sen_head->next == nullptr && sen_tail->prev == nullptr) {
      // cout << "im in here" << endl;
      // cout << "cur value: " << cur.val << endl;
      cur->prev = sen_head;
      cur->next = sen_tail;
      sen_head->next = cur;
      sen_tail->prev = cur;
      // cout << "traversing node inside the first if case..." << endl;
      // traverse_node(sen_head);
    } else {
      // Take the last element in the linked list and append the new cur to
      // it.
      // cout << "mmmmm" << endl;
      node* last_elem = sen_tail->prev;
      // cout << "last elem: " << last_elem->val << endl;
      last_elem->next = cur;
      cur->prev = last_elem;
      cur->next = sen_tail;
      sen_tail->prev = cur;
    }
  }
  // traverse_node(sen_head);
  for (int blinki = 0; blinki < 25; ++blinki) {
    // traverse through each node and apply the rules
    node* p = sen_head->next;
    while (p != sen_tail) {
      if (p->val == 0) {
        zero_rule(p);
        p = p->next;
      } else if (to_string(p->val).length() % 2 == 0) {
        node* new_node = new node;
        even_dig_rule(p, new_node);
        // skip the new node
        p = new_node->next;
      } else {
        mult_by_2024_rule(p);
        p = p->next;
      }
    }
  }

  // traverse_node(sen_head);
  ll out = num_nodes(sen_head);
  cout << out << endl;
  return;
}

void transform_stone(node* n, int num_iters) {
  // sanity check
  node sen_head;
  node sen_tail;
  n->prev = &sen_head;
  sen_head.next = n;
  n->next = &sen_tail;
  sen_tail.prev = n;
  unordered_set<ll> seen;
  for (int i = 0; i < num_iters; ++i) {
    cout << "-------" << endl;
    cout << "iteration: " << i << endl;
    traverse_node(&sen_head);
    cout << endl;
    node* p = sen_head.next;
    while (p != &sen_tail) {
      cout << "++++++++++++" << endl;
      cout << "p->val: " << p->val << endl;
      if (seen.find(p->val) != seen.end()) {
        cout << "here of course..." << endl;
        node* next_node = p->next;
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->next = nullptr;
        p->prev = nullptr;
        p = next_node;
        continue;
      } else {
        seen.insert(p->val);
      }

      cout << "printing set..." << endl;
      for (ll num : seen) {
        cout << num << " ";
      }
      cout << endl;

      if (p->val == 0) {
        zero_rule(p);
        p = p->next;
      } else if (to_string(p->val).length() % 2 == 0) {
        node* new_node = new node;
        even_dig_rule(p, new_node);
        // skip the new node
        p = new_node->next;
      } else {
        mult_by_2024_rule(p);
        p = p->next;
      }
    }
  }
}

void solve2_ll_impl() {
  ifstream fin("day11.in");
  string line;
  int x;

  getline(fin, line);
  stringstream ss(line);
  list<ll> l;
  while (ss >> x) {
    l.push_back(x);
  }

  for (int blinki = 0; blinki < 75; ++blinki) {
    cout << "---" << endl;
    cout << "blinki: " << blinki << endl;
    cout << "l size: " << l.size() << endl;
    // traverse through each node and apply the rules

    list<ll>::iterator it = l.begin();
    while (it != l.end()) {
      if (*it == 0) {
        *it = 1;
      } else if (to_string(*it).length() % 2 == 0) {
        string s = to_string(*it);
        ll first_half = stoll(s.substr(0, s.length() / 2));
        ll second_half = stoll(s.substr(s.length() / 2));
        *it = second_half;
        l.insert(it, first_half);
      } else {
        *it = *it * 2024;
      }
      ++it;
    }
  }

  // list<ll>::iterator it;
  // ll sum = 0;
  // for (it = l.begin(); it != l.end(); ++it) {
  //   cout << *it << " ";
  //   sum++;
  // }
  cout << l.size() << endl;
}

struct HASH {
  size_t operator()(const pair<ll, ll>& x) const {
    return hash<ll>()((x.first) ^ ((x.second) << 32));
  }
};
unordered_map<pair<ll, ll>, ll, HASH> cache;

// Returns the number of stones after b generations if the stone's value is a.
ll f(ll a, ll b) {
  if (b == 0) {
    return 1;
  }
  if (cache.find({a, b}) != cache.end()) {
    return cache[{a, b}];
  }
  if (a == 0) {
    ll output = f(1, b - 1);
    cache[{a, b}] = output;
    return output;
  } else if (to_string(a).length() % 2 == 0) {
    string s = to_string(a);
    ll first_half = stoll(s.substr(0, s.length() / 2));
    ll second_half = stoll(s.substr(s.length() / 2));
    ll left = f(first_half, b - 1);
    ll right = f(second_half, b - 1);
    ll output = left + right;
    cache[{a, b}] = output;
    return output;
  } else {
    ll output = f(a * 2024, b - 1);
    cache[{a, b}] = output;
    return output;
  }
}

void solve2_using_transform_stone_doesnt_work() {
  ifstream fin("sample.in");
  string line;
  int x;
  vector<node> initial_nodes;
  getline(fin, line);
  stringstream ss(line);
  while (ss >> x) {
    // cout << "+++++" << endl;
    // cout << "x: " << x << endl;
    // cout << "sen head address: " << &sen_head << endl;
    // cout << "sen tail address: " << &sen_tail << endl;
    // cout << "sen head next address: " << sen_head.next << endl;
    // cout << "sen tail prev address: " << sen_tail.prev << endl;
    node cur;
    node sen_head;
    node sen_tail;
    cur.val = x;
    sen_head.next = &cur;
    sen_tail.prev = &cur;
    initial_nodes.push_back(cur);
  }

  for (int i = 0; i < initial_nodes.size(); ++i) {
    transform_stone(&initial_nodes[i], 75);
    // traverse_node(&initial_nodes[i]);
  }

  ll sum = 0;
  for (int i = 0; i < initial_nodes.size(); ++i) {
    sum += num_nodes(&initial_nodes[i]);
  }
  cout << sum - 2 << endl;
  return;
}

void solve2() {
  ifstream fin("day11.in");
  string line;
  ll x;
  getline(fin, line);
  stringstream ss(line);
  vector<ll> stones;
  while (ss >> x) {
    stones.push_back(x);
  }
  ll output = 0;
  for (ll stone : stones) {
    output += f(stone, 75);
  }
  cout << output << endl;
}

int main() {
  // exploration();
  solve2();
  return 0;
}