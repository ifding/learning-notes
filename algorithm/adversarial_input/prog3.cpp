#include <iostream>
#include <algorithm>
using namespace std;

// Randomized quicksort

// Generate a random number in the range 0..N-1
int get_rand(int N)
{
  // I forgot to attend the lecture where Prof Dean. explained how to generate
  // random numbers well, so hopefully this will work ok...
  // It looks like get_rand(N) always returns the same number if you call
  // it multiple times with the same value of N, but how could someone 
  // possibly exploit that to make quicksort run slowly...
  return 123456789 % N;
}

// Partition A about a randomly chosen pivot and return the index
// of the pivot in the resulting array
int partition(int *A, int len)
{
  int pivot = get_rand(len);
  
  // Forgot how to partition in-place, so I'm going to just allocate
  // a temporary buffer for now...
  int *temp = new int[len];
  int j = 0;

  for (int i=0; i<len; i++) // copy elements smaller than pivot 
    if (A[i] < A[pivot]) temp[j++] = A[i];
  int where_pivot_first = j; // remember where the pivot is going...
  for (int i=0; i<len; i++) // copy elements equal to pivot
    if (A[i] == A[pivot]) temp[j++] = A[i];
  int where_pivot_last = j-1;
  for (int i=0; i<len; i++) // copy elements larger than pivot
    if (A[i] > A[pivot]) temp[j++] = A[i];
  
  // Now put copy temp back into A
  for (int i=0; i<len; i++)
    A[i] = temp[i];
  delete[] temp;
  
  // return average position of elements equal to pivot, so that 
  // cases with many equal elements don't give unbalanced partitions
  return (where_pivot_first + where_pivot_last) / 2;
}

void quicksort(int *A, int len)
{
  if (len <= 1) return;
  int p = partition(A, len); // partition
  quicksort(A, p); // sort left side
  quicksort(A+p+1, len-p-1); // sort right side
}

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

  quicksort(A,N);
  
  for (int i=0; i<N; i++)
    cout << A[i] << "\n";
  return 0;
}
