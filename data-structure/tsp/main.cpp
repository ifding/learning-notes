#include <string>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace std;
typedef pair<double, double> Point;

double compute(Point const &p1, Point const &p2) {
  return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2)); 
}

void randomize(vector<Point> &v, vector<int> &seq) {
  int j = 0;
  for (size_t i=1; i<v.size(); i++) {
    srand(time(NULL));
    j = rand() % (i+1);
    swap(v[i], v[j]);
    swap(seq[i], seq[j]);
  } 
}

void reverse_array(vector<Point> &p_v, vector<int> &p_i, int start, int end) {
  while (start < end) {
    Point p_temp = p_v[start];
    p_v[start] = p_v[end];
    p_v[end] = p_temp;
    int i_temp = p_i[start];
    p_i[start] = p_i[end];
    p_i[end] = i_temp;
    start++;
    end--;
  }
}

double refinement(vector<Point> &v, vector<int> &seq) {
  double original = 0.0;
  double changed = 0.0;
  for (size_t i=0; i<v.size()-1; i++) {
    for(size_t j=i+1; j<v.size(); j++) {
      original = compute(v[i], v[i+1]) + compute(v[j], v[j-1]);
      changed = compute(v[i], v[j-1]) + compute(v[j], v[i+1]);
      if (original > changed)  reverse_array(v, seq, i+1, j-1);
    }
  }
  double sum = 0.0;
  for (size_t i=0; i<v.size()-1; i++) {
    sum += compute(v[i], v[i+1]);
  }
  sum += compute(v[0], v[v.size()-1]); 
  return sum;
}

int main(void)
{
  vector<Point> v;
  vector<int> seq;
  string x, y;
  int j=0;
  ifstream pointfile("tsp_points.txt");
  while (pointfile >> x >> y) {
    seq.push_back(j);
    j++;
    v.push_back(make_pair(stod(x),stod(y)));
  }
  pointfile.close();
  
  double sum=0.0;
  sum = refinement(v, seq);
  while(sum > 650) {
    for (int i=0; i<100; i++) randomize(v,seq);
    sum = refinement(v, seq);
  }
  for (size_t i=0; i<seq.size(); i++) cout << seq[i] << " ";
  cout << "\n";
  cout << sum << "\n";
  return 0;
}
