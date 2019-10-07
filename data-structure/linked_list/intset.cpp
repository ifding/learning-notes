#include <iostream>
#include <assert.h>
#include "intset.h"

using namespace std;

Intset::Intset()
{ 
   this->size = 0;
   this->head = NULL;
}

Intset::~Intset()
{ 
  Node* head = this->head;
  while(head){
    Node* del = head;
    head = head->next;
    delete del;
  }
}

/* Return true if key is in the set */
bool Intset::find(int key)
{ 

  Node* head = this->head;
  int i = 1;
  while(head){
    if (head->data == key) return true;
    head = head->next;
    i++;
  }

  return false;


//  int low = 0, high = size-1, mid;
  
  /* Binary search for key... */
//  while (low <= high) {
//    mid = (low + high) / 2;
//    if (key == A[mid]) return true;
//    if (key > A[mid]) low = mid+1;
//    else high = mid-1;
//  }
//  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Intset::insert(int key)
{
    assert (!find(key));

    Node* node = new Node();
    node->data = key;

    if (this-> size == 0) {
      node->next = this->head;
      this->head = node;
      this->size++;
    } else if (this->head->data >= key) {
      node->next = this->head;
      this->head = node;
      this->size++;
    } else {
      Node* head = this->head;   
      while(head->next && head->next->data < key) {
	head = head->next;
      }     
      
      node->next = head->next;
      head->next = node;
      this->size++;  
    }

//  int i;
//  
//  assert (!find(key));
//  
//  /* Enlarge (by 2x) the underlying array if it isn't large enough... */
//  if (size == allocated) {
//    allocated *= 2; 
//    int *newA = new int[allocated];
//    for (i=0; i<size; i++) newA[i] = A[i];
//    delete[] A;
//    A = newA;
//  }
//
//  /* Shift up elements in array to make room for new element... */
//  size++;
//  i = size-1;
//  while (i>0 && A[i-1]>key) {
//    A[i] = A[i-1];
//    i--;
//  }
//  A[i] = key;
}

/* Removes a key.  It is an error if key isn't in the set */
void Intset::remove(int key)
{

  assert (find(key));

  if (this->head->data == key){
    Node* del = this->head;
    this->head = this->head->next;
    delete del;  	
  } else{
	for (Node* head = this->head; head->next; head = head->next) {
	  if ((head->next)->data == key) {
	    Node* del = head->next;
	    head->next = del->next;
	    delete del;
	    break; 
	  }
	}
  }


//  int i;
//
//  assert (find(key));
//
//  /* Shift down elements in array to plug gap left by deleted element... */
//  for (i=size-1; key!=A[i]; i--);
//  while (i<size-1) {
//    A[i] = A[i+1];
//    i++;
//  }
//  size--;
}

void Intset::print(void)
{

  Node* head = this->head;
  int i = 1;
  while(head){
    cout << head->data << "\n";
    head = head->next;
    i++;
  }

//  int i;
//  for (i=0; i<size; i++)
//    cout << A[i] << "\n";
}
