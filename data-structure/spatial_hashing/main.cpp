#include <string>
#include <fstream>
#include <iostream>
#include "stringset.h"

using namespace std;

void compute(void)
{
  Stringset S;
  string x, y;

  ifstream wordfile("points.txt");
  while (wordfile >> x >> y)
    S.insert(x, y);
  wordfile.close();

  //S.print();

  double smallest = S.compute();
  cout <<  smallest << " is the smallest. \n";

  //while (cin >> word) {
  //  cout << "Possible alternatives for word '" << word << "':\n";

    // TBD: print out all words that differ by exactly one character...

    //alternative = word;

    // for (char &w:alternative) {
    //  for (const char &c:alphabet) {
    //    w = c;
    //    if (S.find(alternative) && alternative != word)
    //      cout << alternative << "\n";
    //  }
    //  alternative = word;
    // }

  //}
}

void test(void)
{
  Stringset S;
  string x, y, command;

  while(cin >> command) {
    
    if (command == "insert") {
      
      cin >> x >> y;
      if (S.find(x, y))
	      cout << "Error!  (x,y) " << x << y << " already in structure!\n";
      else
	      S.insert(x, y);

    } else if (command == "find") {

      cin >> x >> y;
      if (S.find(x, y))
	      cout << "(x,y) " << x << y << " present.\n";
      else
	      cout << "(x,y) " << x << y<< " absent.\n";

    } else if (command == "print") {

      cout << "Contents of structure:\n";
      S.print();

    } else if (command == "quit") {

      break;

    } else {

      cout << "Error!  Unknown command '" << command << "'!\n";

    }
  }
}

int main(void)
{
  // string d = "0.98489102";
  // cout << d << " " << stod(d)<< "\n";
  // test();
  compute();
  return 0;
}
