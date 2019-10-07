#include<iostream>
#include<string>
#include "stringintmap.h"

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int myhash(string s, int table_size)
{
  unsigned int i, h = 0;
  for (i=0; i<s.length(); i++)
    h = (h * 2917 + (unsigned int)s[i]) % table_size;
  return h;
}

StringIntMap::StringIntMap()
{
  size = 4;
  table = new Node *[size];
  for(int i=0; i<size; i++) {
    table[i] = NULL;
  }
  num_elems = 0;
}


StringIntMap::~StringIntMap()
{
  for(int i=0; i<size; i++) {
    if (table[i] != NULL) {
      while (table[i]!=NULL) {
        Node *temp = table[i];
        table[i] = table[i]->next;
        delete temp;
      }
    }
  }
  delete[] table;
}

bool StringIntMap::find(string str)
{
  int h = myhash(str, size);
  for(Node *temp=table[h]; temp!=NULL; temp=temp->next) {
    if(temp->key == str) return true;
  }
  return false;
}

int &StringIntMap::operator[](string str)
{
  if(!find(str)) insert(str, 0);
  int h = myhash(str, size);
  Node *temp=table[h];
  while(temp->key != str) temp = temp->next;
  return temp->val;
}

void StringIntMap::insert(string str, int val)
{
  assert (!find(str));
  num_elems++;
  if (num_elems == size) {
    Node **old_table = table;
    size *= 2;
    table = new Node *[size];
    for (int i=0; i<size; i++)
      table[i] = NULL;
    
    // Transfer all elements from old_table into table
    for (int i=0; i<size/2; i++) {
      Node *temp = old_table[i];
      while (temp != NULL) {  
        int h = myhash(temp->key, size); 
        table[h] = new Node(temp->key, temp->val, table[h]);
        Node *to_delete = temp;
        temp = temp->next;
        delete to_delete;
      }
    }
    delete [] old_table;
  }
  int h = myhash(str, size);

  table[h] = new Node(str, val, table[h]);
}

void StringIntMap::print()
{
  for(int i=0; i<size; i++)
  {
    for(Node *temp=table[i]; temp!=NULL; temp=temp->next)
      cout << temp->key << " " << temp->val << "\n";
  }
}
