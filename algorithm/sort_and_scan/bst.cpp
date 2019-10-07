#include <iostream>
#include <cstdlib>
#include <fstream>
#include <algorithm>

using namespace std;

struct Person {
  int index;
  int location;
  int height;
  Person():index(0), location(0), height(0) { }
  Person(int i, int l, int h) { index=i; location=l; height=h; }

};

struct Node {
  int location;
  int size;
  Node *left;
  Node *right;
  Node(int x) { location=x; size=1; left=right=NULL; }
};

bool operator< (const Person &A, const Person &B)
{
  return A.height > B.height;
}

void update_size(Node *T)
{
  T->size = 1;
  if(T->left) T->size += T->left->size;
  if(T->right) T->size += T->right->size;
}

Node *find(Node *T, int l)
{
  if (T==NULL) {return NULL;}
  if (l == T->location) {return T;}
  if (l < T->location) return find(T->left, l);
  else return find(T->right, l);
}

void split(Node *T, int l, Node **L, Node **R)
{
  if (T==NULL) {*L=NULL; *R=NULL; return; }
  if (l < T->location) {
    split(T->left, l, L, &T->left);
    *R = T;
  } else {
    split(T->right, l, &T->right, R);
    *L = T;
  }
  update_size(T);
}

Node *insert_keep_balanced(Node *T, int l)
{
  if (T==NULL) return new Node(l);
  if (rand()%(T->size+1) == 0) {
    Node *root = new Node(l);
    split(T, l, &root->left, &root->right);
    update_size(root);
    return root;
  }
  if (l < T->location) T->left = insert_keep_balanced(T->left, l);
  else T->right = insert_keep_balanced(T->right, l);
  update_size(T);
  return T;
}

Node *prefind(Node *root, int x)
{
  if (root == NULL) return NULL;
  if (x == root->location) return root;
  if (x < root->location) return prefind(root->left, x);
  //if (x > root->location) {
  Node *n = prefind(root->right, x);
  if(n) return n;
  else return root;
  
}

Node *succfind(Node *root, int x)
{
  if (root == NULL) return NULL;
  if (x == root->location) return root;
  if (x > root->location) return succfind(root->right, x);
  //if (x < root->location) {
  Node *n = succfind(root->left, x);
  if(n) return n;
  else return root;
}

void print_inorder(Node *T)
{
  if(T==NULL) return;
  print_inorder(T->left);
  cout << T->size << " " << T->location << "\n";
  print_inorder(T->right);
}

int main(void)
{
  int N, x, h;
  Node *T = NULL;
  ifstream samplefile("sample.in");
  samplefile >> N;

  Person *input_array = new Person[N];
  int *pre_array = new int[N];
  int *suc_array = new int[N];
  int i = 0;
  while (samplefile >> x >> h) {
    input_array[i] = Person(i, x, h); 
    i++;
  }
  samplefile.close();

  sort(input_array, input_array+N);

  Node *pre_node = NULL; 
  Node *suc_node = NULL;

  for(int i=0; i<N; i++) {  
    pre_node = prefind(T, input_array[i].location);
    if (pre_node) {
      pre_array[i] = input_array[i].location - pre_node->location;
    } else {
      pre_array[i] = -1;
    }
    //  cout << i << " " << pre_node->location << "\n";
    suc_node = succfind(T, input_array[i].location);
    if (suc_node) {
      suc_array[i] = suc_node->location - input_array[i].location;
    } else {
      suc_array[i] = -1;
    }
    //  cout << i << " " << suc_node->location << "\n";
    T = insert_keep_balanced(T, input_array[i].location);
  }
  for(int i=0; i<N; i++) {
    cout << pre_array[i] << " " << suc_array[i] << "\n";
  }
  //print_inorder(T);
  
  return 0;
}
