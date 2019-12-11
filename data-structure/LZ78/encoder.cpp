#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
using namespace std;

typedef unordered_map<char, int> Node;
typedef pair<int, int> Code;

int main(void)
{
  char c;
  vector<Node> trie;
  vector<Code> vec;
  trie.push_back(Node());  //root 0
  int cur_pos = 0, item = 0;
  do {
    // read one character from standard input, whitespace included
    c = cin.get();
    // not found
    if (trie[cur_pos].find(c) == trie[cur_pos].end()) {
      trie.push_back(Node());
      trie[cur_pos].insert(make_pair(c, trie.size()-1));
      vec.push_back(make_pair(cur_pos, int(c)));
      cur_pos = 0;
    } else {
      cur_pos = trie[cur_pos][c];
    }
  } while (c != EOF);
 
  for (int i=0; i<vec.size(); i++)
    cout << vec[i].first << " " << vec[i].second << "\n";
  return 0;
}
