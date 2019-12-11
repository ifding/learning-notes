#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <set>
#include "graphics.h"
using namespace std;

// Each point represents (x, y, z=elevation above sea level)
// units are in feet; x and y increase to the east and north, respectively
struct Point {
  double x, y, z;
};

// kd-tree
struct Node {
  int index;
  Node *left, *right;
  Node (int i) { index = i; left = right = NULL; }
};

typedef pair<double, int> Neighbor;

// All the points in our LIDAR data set
vector<Point> all_points;

priority_queue<Neighbor> knn;

map<int, vector<int>> nbrs;
map<pair<int,int>, double> edge_wt;

map<int, double> dists;
map<int, int> pred;

Node *root = NULL;

Point mins, maxes; // highest and lowest values in dataset
Point winmin, winmax; // corners of window being displayed on screen

double infinity = 999999999;

// Size of our window on screen
double window_xsize = 1000, window_ysize = 800;

// Source and destination nodes for shortest path calculation
int source_node = -1;
int destination_node = -1;

//when inserting in a kd tree, not inserting a single value
Node *insert(Node *root, int index, int d)
{
  if (root == NULL) return new Node(index);
  bool comp = false;
  if(d == 0) comp = (all_points[index].x <= all_points[root->index].x);
  else comp = (all_points[index].y <= all_points[root->index].y);
  if (comp) root->left = insert(root->left, index, (d+1)%2);
  else root->right = insert(root->right, index, (d+1)%2);
  return root;
}

double dist(Point &p1, Point &p2, int d)
{
  double total = pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
  if (d > 2)  total += pow(p1.z - p2.z, 2);
  return sqrt(total);
}

double slope(Point &p1, Point &p2)
{
  double total = pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
  return fabs(p1.z - p2.z)/sqrt(total);
}

void find_nearest_neighbor(Node *root, Point &p, int d)
{
  if (root == NULL) return;
  // p is not equal root->index
  if (dist(p, all_points[root->index], 3) > 1e-5) {
    knn.push(Neighbor(dist(p, all_points[root->index], 2), root->index));
    knn.pop();
  }
  Node *visit_first = root->left, *visit_second = root->right;
  bool comp = false;
  int i=0;
  if(d == 0) comp = (p.x > all_points[root->index].x);
  else comp = (p.y > all_points[root->index].y);
  if (comp) swap(visit_first, visit_second);
  find_nearest_neighbor(visit_first, p, (d+1)%2);
  if(d == 0) i = fabs(p.x - all_points[root->index].x);
  else i = fabs(p.y - all_points[root->index].y);
  if (i > knn.top().first) return;
  find_nearest_neighbor(visit_second, p, (d+1)%2);
}

void build_edges(int index)
{
  while(!knn.empty()) knn.pop();
  for(int k=0; k<10; k++) knn.push(Neighbor(infinity,0));
  find_nearest_neighbor(root, all_points[index], 0);
  while(!knn.empty()){
    nbrs[index].push_back(knn.top().second);
    edge_wt[make_pair(index, knn.top().second)] = knn.top().first;
    knn.pop();
  }
} 

void dijkstra()
{
  set<pair<double,int>> S;
  dists[source_node] = 0.0;
  S.insert(make_pair(0.0, source_node));
  int last_one;
  while (!S.empty()) {
    int to_visit = S.begin()->second;
    if (to_visit == destination_node) break;
    last_one = to_visit;
    S.erase(S.begin());
    build_edges(to_visit);
    for (int n : nbrs[to_visit]) {
      if (dists.find(n) == dists.end()) dists[n] = infinity; 
      if (slope(all_points[to_visit], all_points[n]) < 1) {
        pair<int,int> edge = make_pair(to_visit, n);
        if (dists[to_visit] + edge_wt[edge] < dists[n]) {
  	  dists[n] = dists[to_visit] + edge_wt[edge];
  	  S.insert(make_pair(dists[n], n));
          pred[n] = to_visit;
        }
      }
    }		      
  }
}


// Called whenever a key is pressed
void keyhandler(int key)
{
  double x_range = winmax.x - winmin.x;
  double y_range = winmax.y - winmin.y;  
  double x_center = winmin.x + x_range/2;
  double y_center = winmin.y + y_range/2;

  if (key == KEY_LEFT) { winmin.x -= x_range/10; winmax.x -= x_range/10; }
  if (key == KEY_RIGHT) { winmin.x += x_range/10; winmax.x += x_range/10; }
  if (key == KEY_UP) { winmin.y += y_range/10; winmax.y += y_range/10; }
  if (key == KEY_DOWN) { winmin.y -= y_range/10; winmax.y -= y_range/10; }

  if (key == '=') { // Zoom in
    winmin.x = x_center - x_range / 2 * 0.8;
    winmax.x = x_center + x_range / 2 * 0.8;
    winmin.y = y_center - y_range / 2 * 0.8;
    winmax.y = y_center + y_range / 2 * 0.8;
  }
  if (key == '-') { // Zoom out
    winmin.x = x_center - x_range / 2 / 0.8;
    winmax.x = x_center + x_range / 2 / 0.8;
    winmin.y = y_center - y_range / 2 / 0.8;
    winmax.y = y_center + y_range / 2 / 0.8;
  }

  if (key == 'n') {
    // find the 10 nearest neighbors of (x_center, y_center)
    // Then store them in a global structure so they can be highlighted in the rendering code
    while(!knn.empty()) knn.pop();
    // ignore z value: 0.0 
    Point p = {x_center, y_center, 0.0};
    for(int k=0; k<10; k++) knn.push(Neighbor(infinity,0));
    find_nearest_neighbor(root, p, 0);
  }
  
  if (key == 's') { // set source
    // set source_node = nearest neighbor in (x,y) plane to (x_center, y_center)
    while(!knn.empty()) knn.pop();
    // ignore z value: 0.0 
    Point p = {x_center, y_center, 0.0};
    knn.push(Neighbor(infinity,0));
    find_nearest_neighbor(root, p, 0);
    source_node = knn.top().second;
    knn.pop();
    cout << "Set source node = " << source_node << "\n";
  }
  if (key == 'd') { // set destination and compute shortest path
    // set destination_node = nearest neighbor in (x,y) plane to (x_center, y_center)
    while(!knn.empty()) knn.pop();
    // ignore z value: 0.0 
    Point p = {x_center, y_center, 0.0};
    knn.push(Neighbor(infinity,0));
    find_nearest_neighbor(root, p, 0);
    destination_node = knn.top().second;
    knn.pop();
    cout << "Set destination node = " << destination_node << "\n";
  }
  if ((key == 's' || key == 'd') && source_node != -1 && destination_node != -1) {
    cout << "Computing shortest path from source to destination...\n";
    // compute shortest path from source to destination and record it so it can be visualized
    // if no path (e.g., all paths would require stepping at more than a 45 degree incline, print "No path"
    dijkstra();
  }
  
  if (key == 'q') exit(0);
}

// Returns where on the screen to plot a point, offsetting it slightly based on elevation
// from its usual (x,y) location to make tall things look tall
pair<double,double> get_point_screen_location(Point &p)
{
  double x = (p.x - winmin.x) / (winmax.x - winmin.x) * window_xsize;
  double y = window_ysize - (p.y - winmin.y) / (winmax.y - winmin.y) * window_ysize;
  double offset = (p.z - 700) / 5; // how much we offset pixel to emphasize its height
  return make_pair(x-offset, y-offset);
}

// Called whenever we need to re-render the current window
void render(void)
{
  // Feel welcome to turn off this message if you want...
  cout << "Rendering (" << winmin.x << "," << winmin.y << ") - (" << winmax.x << "," << winmax.y << ")\n";

  for (Point &p : all_points) {
    pair<double, double> loc = get_point_screen_location(p);
    double x = loc.first, y = loc.second;
    if (x >= 0 && y >= 0 && x < window_xsize && y < window_ysize) {
      double color = min(1.0, max(0.0, (p.z - 700) / 80.0)); // color in range [0,1] based on height
      set_color (color, 0.8, 0.5);
      draw_pixel (x, y);
    }
  }

  // Draw small crosshairs at center of window
  set_color(1,1,1);
  draw_line(window_xsize/2-3, window_ysize/2, window_xsize/2+3, window_ysize/2);
  draw_line(window_xsize/2, window_ysize/2-3, window_xsize/2, window_ysize/2+3);

  // highlight the points that were returned in response to asking for nearest neighbors
  while(!knn.empty()){
    Point p = all_points[knn.top().second];
    pair<double, double> loc = get_point_screen_location(p);
    set_color(1,0,0);
    draw_line(loc.first-3, loc.second-3, loc.first+3, loc.second+3);
    draw_line(loc.first-3, loc.second+3, loc.first+3, loc.second-3); 
    knn.pop();
  }
 
  // TBD: plot a sequence of line segments depicting the shortest path, if one has been computed
  if(source_node != -1 && destination_node != -1) {

    int index = destination_node;
    int last_node;
    double total = 0.0;
    Point p1, p2;
    pair<double, double> loc1, loc2;    
    while(source_node != index) {
      last_node = index;
      if (pred.find(index) != pred.end()) {
        p1 = all_points[index];
        p2 = all_points[pred[index]];        
        loc1 = get_point_screen_location(p1);
        loc2 = get_point_screen_location(p2);
        set_color(1,1,1);
        draw_line(loc1.first, loc1.second, loc2.first, loc2.second);
        total += dist(p1, p2, 3);
        index = pred[index];
      }
    }
    p1 = all_points[index];
    p2 = all_points[last_node];        
    loc1 = get_point_screen_location(p1);
    loc2 = get_point_screen_location(p2);
    set_color(1,1,1);
    draw_line(loc1.first, loc1.second, loc2.first, loc2.second);
    total += dist(p1, p2, 3);    

    p1 = all_points[source_node];
    loc1 = get_point_screen_location(p1);
    set_color(1,1,1);
    draw_line(loc1.first-3, loc1.second-3, loc1.first+3, loc1.second+3);
    draw_line(loc1.first-3, loc1.second+3, loc1.first+3, loc1.second-3); 

    p2 = all_points[destination_node];
    loc2 = get_point_screen_location(p2);
    set_color(1,1,1);
    draw_line(loc2.first-3, loc2.second-3, loc2.first+3, loc2.second+3);
    draw_line(loc2.first-3, loc2.second+3, loc2.first+3, loc2.second-3);

    cout << "Source node: " << source_node << ", Destination node:" << destination_node << "\n";
    cout << "The length of shortest paths: " << total << "\n";

    // removes all elements for the next calculation
    nbrs.clear();
    edge_wt.clear();
    dists.clear();
    pred.clear();
    source_node = -1;
    destination_node = -1;
  }

}

int main(int argc, char *argv[])
{
  // Read in all the data
  ifstream fin ("points.txt");
  Point p;
  int count = 0;
  while (fin >> p.x >> p.y >> p.z)
    all_points.push_back(p);
    
  // Find initial view window corner points
  mins = maxes = all_points[0];
  for (auto &p : all_points) {
    mins.x = min(mins.x, p.x);
    mins.y = min(mins.y, p.y);
    mins.z = min(mins.z, p.z);
    maxes.x = max(maxes.x, p.x);
    maxes.y = max(maxes.y, p.y);
    maxes.z = max(maxes.z, p.z);
  }
  // Re-adjust y to match aspect ratio of window
  winmin = mins; winmax = maxes;
  winmax.y = winmin.y + (winmax.x - winmin.x) * window_ysize / window_xsize;

  // build 2-dimensional kd-tree here
  // Normally you'd want to insert them in random order but they are already randomly shuffled, so no need to shuffle again
  for (int i=0; i<all_points.size(); i++)  root = insert(root, i, 0);

  // Launch graphics; all remaining interaction is via callback to render and keyhandler function
  init_graphics(argc, argv, window_xsize, window_ysize, render, keyhandler);
  return 0;
}
