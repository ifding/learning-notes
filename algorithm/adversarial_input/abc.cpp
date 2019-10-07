#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <assert.h>

using namespace std;

struct Node {
  int key;
  int size;
  Node *left;
  Node *right;
  Node (int k) { key = k; size = 1; left = right = NULL; }
};

void fix_size(Node *T)
{
  T->size = 1;
  if (T->left) T->size += T->left->size;
  if (T->right) T->size += T->right->size;
}

// prints out the inorder traversal of T (i.e., the contents of T in sorted order)
void print_inorder(Node *T)
{
  if (T == NULL) return;
  print_inorder(T->left);
  cout << T->key << "\n";
  print_inorder(T->right);
}

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node *T, int r, Node **L, Node **R)
{
  if (T == NULL) {
    *L = NULL;
    *R = NULL;
    return;
  }
  int rank_of_root = T->left ? T->left->size : 0;
  if (r <= rank_of_root) {
    // recursively split left subtree
    split(T->left, r, L, &T->left);
    *R = T;
  } else {
    split(T->right, r-rank_of_root-1, &T->right, R);
    *L = T;
  }
  fix_size(T);
}

// insert value v at rank r
Node *insert_random(Node *T, int v, int r)
{
  // If k is the Nth node inserted into T, then:
  // with probability 1/N, insert k at the root of T
  // otherwise, insert_random k recursively left or right of the root of T
  if (T == NULL) return new Node(v);
  if (rand() % (T->size + 1) == 0) {
    // insert at root
    Node *new_root = new Node(v);
    split(T, r, &new_root->left, &new_root->right);
    fix_size(new_root);
    return new_root;
  }
  int rank_of_root = T->left ? T->left->size : 0;
  if (r <= rank_of_root) T->left = insert_random(T->left, v, r);
  else T->right = insert_random(T->right, v, r - rank_of_root - 1);
  fix_size(T);
  return T;
}

Node *T;

void generate(int N)
{
  if (N==1) {
    T = insert_random(T, 1, 0);
    return;
  }
  generate(N-1);
  int pivot = 123456789%N;

  //  Old array-based solution:
  //  for (int i=N-1; i>pivot; i--)
  //    A[i] = A[i-1];
  //  A[pivot] = N;

  // Faster solution by encoding sequence in a balanced BST:
  T = insert_random(T, N, pivot);
}

int main(int argc, char *argv[])
{
  if (argc != 2) {
    cout << "Usage: bad1 <input size>\n";
    return 0;
  }

  int N = atoi(argv[1]);  // get first command-line argument
  if (N<1 || N>100000) {
    cout << "Invalid input size!\n";
    return 0;
  }
  
  cout << N << "\n";
  generate(N);
  print_inorder(T);
  
  return 0;
}
