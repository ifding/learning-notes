#include <iostream>
#include <string>
using namespace std;
typedef pair<int, int> State;

const int N=1000;

const string Action[6] = {"Fill jug 1","Fill jug 2", "Empty jug 1",  
                          "Empty jug 2", "Pour 1->2", "Pour 2->1"};

string visit(State &s, bool (&beenthere)[N][N], const int a, const int b, const int x)
{
  int pour_a_a = s.first > (b-s.second) ? s.first-(b-s.second) : 0;
  int pour_a_b = s.first > (b-s.second) ? b : s.first+s.second;
  int pour_b_b = s.second > (a-s.first) ? s.second-(a-s.first) : 0;
  int pour_b_a = s.second > (a-s.first) ? a : s.first+s.second;
  int a_ops[6] = {a, s.first, 0, s.first, pour_a_a, pour_b_a};
  int b_ops[6] = {s.second, b, s.second, 0, pour_a_b, pour_b_b};
  if (s.first + s.second == x) return "\n";
  for (int i=0; i<6; i++) {
    if (beenthere[a_ops[i]][b_ops[i]] != true) {
      s.first = a_ops[i];
      s.second = b_ops[i];
      beenthere[s.first][s.second] = true;
      string str = " [a=" + to_string(s.first) + ", b=" + to_string(s.second) + "]\n"; 
      string result = visit(s, beenthere, a, b, x); 
      if (result.empty()) {
        beenthere[s.first][s.second] = false;
        s.first = a_ops[i];
        s.second = b_ops[i];
        return "";
      } else
        return Action[i] + str + result; 
    } 
  }
  return "";
}

int main(void)
{
  State state(0, 0);
  bool beenthere[N][N];
  int a, b, x;
  cout << "Enter A:"; cin>>a;
  cout << "Enter B:"; cin>>b;
  cout << "Enter X:"; cin>>x;
  beenthere[0][0] =true;
  string result = visit(state, beenthere, a, b, x);
  if(!result.empty())
    cout << result;
  else
    cout << "Impossible!\n";
}
