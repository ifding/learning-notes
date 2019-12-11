#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

typedef pair<int, int> Candy;

bool comparison(const Candy &a, const Candy &b )
{
  return double(a.second)/double(a.first) > double(b.second)/double(b.first);
}

int calculate_left(const vector<Candy> &c, int ind, const vector<Candy> &sf)
{
  vector<bool> avail(c.size(), true);
  int i=0, count = 0, bag = 0, value = 0; 
  i = ind;
  while(count < 3) {
    if (avail[i] && (bag + c[i].first) < 2000 - sf[count].first) {
      bag += c[i].first;
      value += c[i].second;
      avail[i] = false;
    }
    i++;
    if (i == c.size()) {
      i = ind;
      bag = 0;
      count++;
    }
  }
  return value;
}

int greedy(const vector<Candy> &c)
{
  int value = 0;
  vector<Candy> so_far;
  //0, 1 and 2for bag 1, bag 2 and bag 3, 3 means unused 
  for(int i=0; i<3; i++) so_far.push_back(make_pair(0,0));
  value = calculate_left(c, 0, so_far); 
  return value;
}

int calculate_value(const vector<Candy> &c, const vector<int> &s)
{
  int i=0, ind = 0, bag = 0, value = 0;
  while(ind < 3) {
    if (s[i] == ind && (bag + c[i].first) < 2000) {
      bag += c[i].first;
      value += c[i].second;
    }
    i++;
    if (i == c.size()) {
      i = 0;
      bag = 0;
      ind++;
    }
  }
  return value;
}

void initialize(const vector<Candy> &c, vector<int> &s)
{
  vector<int> bags;  
  for (int i=0; i<c.size(); i++) {
    bags.push_back(i);
    s.push_back(-1);
  }
  random_shuffle(bags.begin(), bags.end());
  int bag = 0, id = 0;
  for (int i=0; i<c.size(); i++) {
    int ind = bags[i];
    if (bag < 2000) {
       bag += c[ind].first;
       s[ind] = id%3;
    }
    if (bag >= 2000) {
      if (id < 3) { bag = 0; }
      else { bag = 1999; } // just one more candy will > 2000
      id++;
    }
  }
}

int refinement(const vector<Candy> &c)
{
  vector<int> selected;
  int before = 0, best = 0;

  for (int iter=0; iter<10; iter++) {
    initialize(c,selected);
    for (int j=0; j<c.size()-1; j++) {
     for (int k=j+1; k<c.size(); k++) {
       before = calculate_value(c, selected); 
       if ( selected[j] != selected[k]) { 
          swap(selected[j], selected[k]);
          if (before > calculate_value(c, selected))
            swap(selected[j], selected[k]); // swap back
        }
      }
    }
    if (best < calculate_value(c, selected))
      best = calculate_value(c, selected);
    
    selected.clear();
  }
  return best;
}

void recurse(const vector<Candy> &c, vector<int> &s, int ind, vector<Candy> &sf, int &best)
{
  int already = 0, left = 0;   
  // base case
  if (ind == c.size()-1) {
    int option = 3, current = 0;
    for (int i=0; i<3; i++) {
      if (sf[i].first + c[ind].first < 2000 && already +c[ind].second > best) {
        option = i;
        best = already + c[ind].second; 
      }
    }
    s[ind] = option;
  }
  
  for(int i=0; i<3; i++) {
    if (sf[i].first > 2000) return;
    already += sf[i].second;
  } 
  if (already > best) best = already;
  left = calculate_left(c, ind, sf);

  if (already + left < best*9/10 ) return;
  //else {
  //  for(int i=0; i<c.size(); i++) {
  //    cout << s[i] << " ";
  //  }
  //  cout << "\n" << already << " " << left << "\n";
  //}

  int choice = 3;
  for (int j=ind; j<c.size()-1; j++) {
    if (ind == 0) choice = 1;
    else {
      choice = 2;
      for (int k=0; k<ind; k++)
        if (s[k] >0 && s[k] < 3) choice = 3;
    }
    for (int i=0; i<choice; i++) {
      s[j] = i;
      sf[i].first += c[j].first;
      sf[i].second += c[j].second;
      recurse(c, s, j+1, sf, best);
      s[j] = 3;
      sf[i].first -= c[j].first;
      sf[i].second -= c[j].second;
    }
  }
}

int exhaustive(const vector<Candy> &c)
{
  int best = 0;
  vector<int> selected (c.size(), 3);
  vector<Candy> so_far;
  //0, 1 and 2for bag 1, bag 2 and bag 3, 3 means unused 
  for(int i=0; i<3; i++) so_far.push_back(make_pair(0,0));
  best = calculate_left(c, 0, so_far); 
  recurse(c, selected, 0, so_far, best);
 
  return best;
}


int main(void)
{
  srand(unsigned(time(NULL))); 
  vector<Candy> candy;
  int x, y;
  ifstream candyfile("candy.txt");
  while (candyfile >> x >> y) 
    candy.push_back(make_pair(x,y));
  
  sort(candy.begin(), candy.end(), comparison);

  cout << "Greedy: " << greedy(candy) << "\n";

  cout << "Refinement: " << refinement(candy) << "\n";

  cout << "Exhaustive: " << exhaustive(candy) << "\n";

  return 0;
}
