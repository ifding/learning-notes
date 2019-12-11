#include <iostream>
#include <vector>
#include <string>

using namespace std;

void print_chess(const vector< vector<int> > &chess) {
  for(int i=0; i<chess.size(); i++) {
    for(int j=0; j<chess[i].size(); j++)
      cout << chess[i][j] << " ";
    cout << "\n";
  }
  cout << "\n";
}

bool check_attack(const vector< vector<int> > &chess, int r, int c)
{
  int n = chess.size();
  for(int i=r-1; i>=0; i--) {
    if (chess[i][c] == 1) return true;
    if (c+r-i < n && chess[i][c+r-i] == 1) return true;
    if (c-r+i >=0 && chess[i][c-r+i] == 1) return true;
  }
  return false;
}

void check_row(vector< vector<int> > &chess, int r, int &solution) 
{
  int n = chess.size();
  // base case
  if (r == n-1) {
    for (int i=0; i<n; i++) {
      if ( !check_attack(chess, r, i) ) {
        solution++;
        //print_chess(chess);
        //cout << solution << "\n";
      }
    }
    return;
  }
  for (int i=0; i<chess.size(); i++) {
    if ( !check_attack(chess, r, i) ) {
      chess[r][i] = 1;
      check_row(chess, r+1, solution);
      chess[r][i] = 0;
    }
  }
}

int main(int argc, char** argv)
{
  int n = 0, solution=0;
  if (argc >= 2) n = stoi(argv[1]);

  if (n < 1) exit(0);

  vector<vector<int> > chess(n, vector<int> (n, 0));
  
  for (int i=0; i<chess.size()/2; i++) {
    chess[0][i] = 1;
    check_row(chess, 1, solution);
    chess[0][i] = 0;
  }
  solution *= 2;
  if (chess.size()%2 != 0 ) {
    chess[0][chess.size()/2] = 1;
    check_row(chess, 1, solution);
    chess[0][chess.size()/2] = 0; 
  }

  if(chess.size() == 1) solution = 1;
 
  cout << solution << "\n";

  return 0;
}

