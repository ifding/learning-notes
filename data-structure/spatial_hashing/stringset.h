#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <string>
using namespace std;

class Stringset {

 private: 

  struct Node {
    string x;
    string y;
    Node *next;
    Node(string x_, string y_, Node *n) { x = x_;  y = y_; next = n; }
    Node() { x = ""; y = ""; next = NULL; }
  };

  //by me: https://stackoverflow.com/questions/9285742/double-pointers-to-c-objects
  //by me: If you want a dynamic (raw) array of (raw) pointers, then you'll indeed need a pointer-to-pointer.
  Node ***array;  // array of pointers to linked lists
  int size;      // size of table, as currently allocated
  
 public:
  Stringset();
  ~Stringset();
  bool find(string x, string y);
  void insert(string x, string y);
  double compute();
  void print(void);
};

#endif
