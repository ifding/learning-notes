#ifndef STRINGINTMAP_H
#define STRINGINTMAP_H

#include<iostream>
#include<string>

using namespace std;

class StringIntMap {
  private:
    struct Node {
      string key;
      int val;
      Node *next;
      Node(string k, int v, Node *n) { key=k; val=v; next=n; }
      Node() { key=""; val=0; next=NULL; }
    };
    
    Node **table;
    int size;
    int num_elems;

  public:
    StringIntMap();
    ~StringIntMap();
    bool find(string str);
    int &operator[](string str);
    void insert(string str, int val);
    void print();
};

#endif
