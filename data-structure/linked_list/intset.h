#ifndef INTSET_H /* Prevent multiple inclusion... */
#define INTSET_H

class Node
{
  public:
    Node* next;
    int data;
};


class Intset {

 private:
  int size;
  Node* head;

 public:
  Intset();
  ~Intset();
  bool find(int key);
  void insert(int key);
  void remove(int key);
  void print(void);
};

#endif