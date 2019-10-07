#include <iostream>
#include <algorithm>
using namespace std;

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
  cout << T->key << " ";
  print_inorder(T->right);
}

// Split tree T on rank r into tree L (containing ranks < r) and 
// a tree R (containing ranks >= r)
void split(Node *T, int r, Node **L, Node **R)
{
  // TBD: please fill in this function appropriately
  if (T==NULL) {
    *L=NULL;
    *R=NULL;
    return;
  }
  int rank_of_root = T->left ? T->left->size : 0;
  if (r>rank_of_root) {
    split(T->right, r - rank_of_root - 1, &T->right, R);
    *L = T;
  } else {
    split(T->left, r, L, &T->left);
    *R = T;
  }
  fix_size(T);
}

// insert value v at rank r
Node *insert_keep_balanced(Node *T, int v, int r)
{
  // TBD: fill this in
  if (T==NULL) return new Node(v);
  if (rand() % (T->size + 1) == 0) {
    Node *new_root = new Node(v);
    split(T, r, &new_root->left, &new_root->right);
    fix_size(new_root);
    return new_root; 
  }
  int rank_of_root = T->left ? T->left->size : 0;
  if (r > rank_of_root) T->right = insert_keep_balanced(T->right, v, r - rank_of_root - 1);
  else T->left = insert_keep_balanced(T->left, v, r);
  fix_size(T);
  return T;
}

// implement in a balanced BST
void generate_adversarial(Node **T, int N)
{
  if (N==1) {
    *T = insert_keep_balanced(*T, 1, 0);
    return;
  }
  generate_adversarial(T, N-1);
  int pivot = get_rand(N);

  *T = insert_keep_balanced(*T, N, pivot);
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
 
  // Generate and print bad input of size N for prog1
  // (currently just generates an input of N random numbers)
  cout << N << "\n";
  
  //int *A = new int[N];
  //for (int i=0; i<N; i++)
  //  A[i] = rand() % 1000000;

  Node *T=NULL;
  generate_adversarial(&T, N);
  
  print_inorder(T);
  //for (int i=0; i<N; i++)
  //  cout << A[i] << "\n";
  
  return 0;
}
