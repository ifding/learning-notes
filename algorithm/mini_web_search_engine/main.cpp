#include<iostream>
#include<fstream>
#include<string>
#include "pagetable.h"
#include "stringintmap.h"

using namespace std;

int main()
{
  ifstream fin;
  string s;
  string l;
  string command;
  int page_num = 0;
  int word_num = 0;
  int index = 0;

  StringIntMap url_map, word_map;

  fin.open("webpages.txt");
  while (fin >> s) {
    //cout << s << "\n";
    if (s.find("NEWPAGE") != string::npos) { 
      page_num++; 
      index = 0; 
    } else if (s.find("http://") == string::npos) {
      if (!word_map.find(s)) {
        word_map.insert(s, word_num);
        word_num++;
      }
    }
    if (index == 1) url_map.insert(s, page_num-1);
    index++;
  }
  fin.close();
 
  cout << page_num << "\n";
  cout << word_num << "\n";
  //url_map.print();
  //word_map.print();
  Pagetable pt(page_num, word_num);
  index = 0;

  fin.open("webpages.txt");
  while (fin >> s) {
    //cout << s << "\n";
    if (s.find("NEWPAGE") != string::npos) index  = 0;

    if (index == 1) {
      //cout << s << "\n";
      pt.add_page(s);
      l = s;
    } else if (index > 1) {
      // insert words
      if (s.find("http://") == string::npos) { 
        pt.insert_word(s);
        pt.add_word_array(word_map[s], l, url_map[l]);
      // insert links
      } else {
        if(url_map.find(s)) pt.insert_link(s, url_map[s]); // if s is not in url_map, ignore it
      }
    }

    index++;
  }
  fin.close();
  pt.set_outgoing_num();
  pt.page_rank();

  //pt.print();

  //while(cin >> s) {
  //  pt.search(word_map[s]);
  //} 

  return 0;
}
