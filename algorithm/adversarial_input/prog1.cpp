#include <iostream>
#include <algorithm>
using namespace std;

// Solve the "longest line of sight" problem from lecture...

int main(void)
{
  int N; // size of input
  if (!(cin >> N) || N < 1 || N > 100000) {
    cout << "Invalid input size!\n";
    return 0;
  }

  // read input
  int *A = new int[N];
  for (int i=0; i<N; i++) 
    if (!(cin >> A[i]) || A[i]<0) {
      cout << "Invalid input!\n";
      return 0;
    }
  
  // compute max to left and max to right from each point
  int *max_to_left = new int[N];
  int *max_to_right = new int[N];
  max_to_left[1] = A[0];
  for (int i=2; i<N; i++) 
    max_to_left[i] = max(A[i-1], max_to_left[i-1]);
  max_to_right[N-2] = A[N-1];
  for (int i=N-3; i>=0; i--) 
    max_to_right[i] = max(A[i+1], max_to_right[i+1]);

  // now find the longest line of sight:
  // for each element, scan left and right until we reach the
  // first larger element; record farthest such element found.
  int best = -1;
  for (int i=0; i<N; i++) {
    if (i>0 && max_to_left[i] >= A[i]) // is there someone larger on left?
      for (int j=i-1; j>=0; j--) // scan left until we reach someone larger
	if (A[j] >= A[i]) { best = max(best, i-j); break; }	
    if (i<N-1 && max_to_right[i] >= A[i]) // is there someone larger on right?
      for (int j=i+1; j<N; j++) // scan right until we reach someone larger
	if (A[j] >= A[i]) { best = max(best, j-i); break; }
  }
  
  cout << "Longest line of sight: " << best << "\n";
  return 0;
}
