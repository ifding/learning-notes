#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
using namespace std;

typedef pair<double, double> Point;

double distance(Point &p1, Point &p2) {
  double dx = p1.first - p2.first;
  double dy = p1.second - p2.second;
  return sqrt(dx*dx + dy*dy);
}

int main(void)
{
  vector<Point> v;
  set<Point> s;
  double x, y;
  ifstream pointfile("points.txt");
  while (pointfile >> x >> y)
    v.push_back(make_pair(x, y));
  pointfile.close();
  sort(v.begin(), v.end());
  
  double dist = distance(v[0], v[1]);
  double D = v[1].first - v[0].first;
  int i=0;
  for (int j=0; j<v.size(); j++) {
    while(v[j].first - v[i].first > D) {
      s.erase(make_pair(v[i].second, v[i].first));    
      i++;
    }
    s.insert(make_pair(v[j].second, v[j].first));

    set<Point>::iterator it;
    Point new_point; 
    it = s.find(make_pair(v[j].second, v[j].first));
    it++;
    while (it != s.end() && (it->first - v[j].second < D)) {
      new_point = make_pair(it->second, it->first);
      if (D > distance(new_point, v[j])) 
        D = distance(new_point, v[j]);
      it++;
    }
    it = s.find(make_pair(v[j].second, v[j].first));
    while (it != s.begin() && (v[j].second - it->first < D)) {
     it--;
     new_point = make_pair(it->second, it->first);
     if (D > distance(new_point, v[j])) 
        D = distance(new_point, v[j]);
   }  
  }
  cout << D << "\n";

  //for (int i=0; i<v.size(); i++)
  //  cout << v[i].first << " " << v[i].second << "\n";
  return 0;
}
