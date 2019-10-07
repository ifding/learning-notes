#include <iostream>
#include <algorithm>
using namespace std;

// Find number of distinct elements using a hash table

struct Node {
  int val;
  Node *next;
  Node (int v, Node *n) { val = v; next = n; }
};

Node **T; // array of Node *s for hash table
int table_size = 10000;  

// check out this awesome hash function!
int hash(int x)
{
  return ((unsigned int)x * 3 + 17) % table_size;
}

bool find(int x)
{
  for (Node *n = T[hash(x)]; n != NULL; n = n->next)
    if (n->val == x) return true;
  return false;
}

void insert(int x)
{
  int index = hash(x);
  T[index] = new Node(x, T[index]);
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

  // initialize hash table -- if I make it have size 10,000, then the
  // average length of a linked list should be small...
  T = new Node *[table_size]; 
  for (int i=0; i<table_size; i++)
    T[i] = NULL;

  int count = 0;
  for (int i=0; i<N; i++)
    if (!find(A[i])) {
      count++;
      insert(A[i]);
    }

  cout << "The input contains " << count << " distinct integers.\n";

  return 0;
}
