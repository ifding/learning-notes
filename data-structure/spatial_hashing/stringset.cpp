#include <iostream>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "stringset.h"

using namespace std;

// Function to calculate distance 
double distance(string x1, string y1, string x2, string y2) 
{ 
  // Calculating distance 
  return sqrt(pow(stod(x2) - stod(x1), 2) + pow(stod(y2) - stod(y1), 2)); 
}


Stringset::Stringset()
{
  size = 1000; // initial size of array    
  //by me: Node **array = new Node * [size]
  array = new Node **[size]; // allocate array, initialize head ptrs all to NULL
  
  //cout << "after array = new Node *[size] \n";   
  for (int i=0; i<size; i++) {
    //cout << array[i] << "\n";
    array[i] = new Node *[size];

    for (int j=0; j<size; j++) {
      array[i][j] = NULL;
    }
    //cout << array[i] << "\n";
  }
}

Stringset::~Stringset()
{
  for (int i=0; i<size; i++) {
  	for (int j=0; j<size; j++) {
      while (array[i][j] != NULL) {
        Node *temp = array[i][j];
        array[i][j] = array[i][j]->next;
        delete temp;
      }
    }
  }
  delete[] array;
}

/* Return true if key is in the set */
bool Stringset::find(string x, string y)
{
  int j = int(stod(x)*size);
  int k = int(stod(y)*size);

  Node *n = array[j][k];
  while (n != NULL) {
    if (n->x == x && n->y == y) return true;
    n = n->next;
  }
  return false;
}

/* Inserts a new key.  It is an error if key is already in the set. */
void Stringset::insert(string x, string y)
{
  assert (!find(x, y));

  int j = int(stod(x)*size);
  int k = int(stod(y)*size);  

  array[j][k] = new Node(x, y, array[j][k]);
}

/* Removes a key.  It is an error if key isn't in the set */
//void Stringset::remove(string key)
//{
//  assert (find(key));
//  num_elems--;

  // TBD: Delete element
//}

double Stringset::compute(void)
{
  double smallest = 1.0;
  double dist = 0.0;  

  for (int i=0; i<size; i++) {
  	for (int j=0; j<size; j++) { 	  
	  for (Node *curr = array[i][j]; curr != NULL; curr = curr->next) {
		Node *next = curr->next;
		//cout << "(" << curr->x << "," << curr->y << ") -->";
		// while(next != NULL) {
		//   //cout << "(" << next->x << "," << next->y << ") ";
		//   dist = distance(curr->x, curr->y, next->x, next->y);
		//   if (dist < smallest) smallest = dist;
		//   next = next->next;
		// }

		for (int m = -1; m < 2; ++m)
		  {
		  	for (int n = -1; n < 2; ++n)
		  	{
		  		if (i+m >= 0 && i+m < size && j+n>= 0 && j+n < size) {
		  			//if ((m != 0 && n == 0) || (m == 0 && n != 0) || (m != 0 && n != 0)) {
			  			for (Node *temp = array[i+m][j+n]; temp != NULL; temp=temp->next) {
			  			  if (temp != curr) {
							  dist = distance(curr->x, curr->y, temp->x, temp->y);
							  if (dist < smallest) smallest = dist;	
							  //cout << dist << "\n";
						  }	  				
			  			}
			  		//}
		  		}
		  	}
		  }
		
		//cout << "\n";
	  }

    }

  }
  return smallest;
}


void Stringset::print(void)
{
  for (int i=0; i<size; i++) {
  	for (int j=0; j<size; j++) {
  	  Node *temp = array[i][j];
      while (temp != NULL) {
        cout << "(" << temp->x << "," << temp->y << ") ";
        temp = temp->next;
      }
      if (array[i][j] != NULL)
        cout << "||";
    }
    cout << "\n";
  }
}
