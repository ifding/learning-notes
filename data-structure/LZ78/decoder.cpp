#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_map>
using namespace std;

typedef pair<int, int> Code;

void print_output(const vector<Code> &vec, int ind)
{
  if (vec[ind].first > 0)  print_output(vec, vec[ind].first);
  if (vec[ind].second !=EOF) cout << char(vec[ind].second);
}


int main(void)
{
  int id, c;
  vector<Code> vec;
  vec.push_back(make_pair(0, -1)); // add root to start with
  while (cin>>id>>c) { 
    vec.push_back(make_pair(id, c)); 
    print_output(vec, vec.size()-1);
  }
 
  return 0;
}
