#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <string>
using namespace std;

class Stringset {

 private: 

  struct Node {
    string key;
    Node *next;
    Node(string k, Node *n) { key = k; next = n; }
    Node() { key = ""; next = NULL; }
  };

  //by me: https://stackoverflow.com/questions/9285742/double-pointers-to-c-objects
  //by me: If you want a dynamic (raw) array of (raw) pointers, then you'll indeed need a pointer-to-pointer.
  Node **table;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  int num_elems; // number of elements stored in the table
  
 public:
  Stringset();
  ~Stringset();
  bool find(string key);
  void insert(string key);
  void remove(string key);
  void print(void);
};

#endif
