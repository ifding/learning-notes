#ifndef PAGETABLE_H
#define PAGETABLE_H

#include<string>
using namespace std;

class Pagetable {
  
  private:

    struct Word {
      string word;
      Word *next;
      Word(string str, Word *w) { word=str; next=w; }
      Word() { word=""; next=NULL; }
    };

    struct Link {
      string link;
      int page_num;
      Link *next;
      Link(string str, int n, Link *l) { link=str; page_num=n; next=l; }
      Link() { link=""; page_num=0; next=NULL; }
    };

    struct Page {
      string url;
      int outgoing_num;
      double weight;
      double new_weight;
      Word *word;
      Link *link;
      Page(string str, Word *w, Link *l) { 
        url=str; word=w; link=l; weight=0.0; new_weight=0.0; outgoing_num=0; }
      Page(string str) { 
        url=str; word=NULL; link=NULL; weight=0.0; new_weight=0.0; outgoing_num=0; }
      ~Page() {
        while (word != NULL) { Word *w = word; word = word->next; delete w; }
        while (link != NULL) { Link *l = link; link = link->next; delete l; }
      }
    };

    Page **array;
    Link **word_array;
    int page_count;
    int N;
    int M; 

  public:
    Pagetable(int n, int m);
    ~Pagetable();
//    bool find(string link);
    void add_page(string url);
    void add_word_array(int id, string link, int n);
    void insert_link(string str, int n);
    void insert_word(string str);
    void set_outgoing_num();
    void page_rank();
    void print();
    void search(int id);
};

#endif
