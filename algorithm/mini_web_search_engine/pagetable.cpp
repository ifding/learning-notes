#include<iostream>
#include<string>
#include "pagetable.h"

using namespace std;


Pagetable::Pagetable(int n, int m) : N(n), M(m)
{
  page_count = 0;
  array = new Page *[N];
  word_array = new Link *[M];
  for (int i=0; i<N; i++) {
    array[i] = NULL;
  }
  for (int j=0; j<M; j++) {
    word_array[j] = NULL;
  }
}

Pagetable::~Pagetable()
{
  for (int i=0; i<N; i++) {
    if (array[i] != NULL) delete array[i];
  }
  for (int j=0; j<M; j++) {
    if (word_array[j] != NULL ) delete word_array[j];
  }
  delete[] array;
  delete[] word_array;
}


void Pagetable::add_page(string url)
{
  if (page_count >= 0 && page_count < N) {
    array[page_count] = new Page(url);
  }
  page_count++;
}

void Pagetable::add_word_array(int id, string link, int n)
{
  word_array[id] = new Link(link, n, word_array[id]);
}

void Pagetable::insert_link(string str, int n)
{
  array[page_count-1]->link = new Link(str, n, array[page_count-1]->link);
}

void Pagetable::insert_word(string str)
{
  array[page_count-1]->word = new Word(str, array[page_count-1]->word);
}

void Pagetable::set_outgoing_num()
{
  int count = 0;
  for(int i=0; i<page_count; i++) {
    if(array[i] != NULL) {
      count = 0;
      for(Link *l=array[i]->link; l!=NULL; l=l->next) count++;
      array[i]->outgoing_num = count;
    }
  }
}

void Pagetable::page_rank()
{
  int N = 50;
  for(int i=0; i<page_count; i++) {
    if (array[i] != NULL)  array[i]->weight = 1.0/page_count;
  }
  for(int c=0; c<N; c++) {  
    for(int k=0; k<page_count; k++) {
      if (array[k] != NULL)  array[k]->new_weight = 0.1/page_count;
    }
    for (int i=0; i<page_count; i++) {
      if (array[i] != NULL) {
        //cout << array[i]->url << " " << array[i]->outgoing_num << "\n";
        for (Link *l=array[i]->link; l!=NULL; l=l->next)
          array[l->page_num]->new_weight += 0.9*array[i]->weight/(array[i]->outgoing_num+0.000000001);
      }
    }
    for (int i=0; i<page_count; i++) {
      array[i]->weight = array[i]->new_weight;
    }
  }
}


void Pagetable::search(int id)
{
  if( id >=0 && id < M) {
    cout << id << "\n";
    for (Link *l= word_array[id]; l!=NULL; l=l->next)
      cout << int(array[l->page_num]->weight *100*N) << " " << l->link << "\n";
  }
}


void Pagetable::print()
{
  for(int i=0; i<page_count; i++) {
    if (array[i] != NULL) {
      cout <<  array[i]->url << " " << array[i]->weight << "\n";
      //for (Word *w= array[i]->word; w!=NULL; w=w->next)
      //  cout << w->word << " ";
      //cout << "\n";
      //for (Link *l= array[i]->link; l!=NULL; l=l->next)
      //  cout << l->link << " " << l->page_num << " || ";
      //cout << "\n";
    }
  }
  for(int i=0; i<M; i++) {
    if(word_array[i] != NULL) {
      cout << i << "\n";
      for (Link *l= word_array[i]; l!=NULL; l=l->next)
        cout << l->link << " " << l->page_num << " || ";
      cout << "\n";
    }
  }
}
