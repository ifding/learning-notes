
# Learning Summary

## Basics

### 1.how to generate random letters from A-Z?
```c++
int main()
{
   int n = rand() % 26;
   char c = (char)(n+65);
   cout << c << "\n";
   return 0;
}
```
or
  
```c++    
const char* const a_to_z = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
```
generate a random integer ri in the range 0 to 25 (inclusive).
use a_to_z[ri] as the random letter.

### 2.Call-by-reference vs Call-by-value

The & before the parameter name indicates that the argument vec is passed by reference, which means that the value in the function is shared with the value in the caller. Call-by-reference is more efficient than C++’s default model of call-by-value, which would require copying every element in the vector.
```c++
void printVector(Vector<int> & vec) {
	cout << "[";
	for (int i = 0; i < vec.size(); i++) {
		if (i > 0) cout << ", ";
		cout << vec[i];
	}
	cout << "]" << endl;
}

void removeZeroElements(Vector<int> & vec) {
	for (int i = vec.size() - 1; i >= 0; i--) {
		if (vec[i] == 0) vec.removeAt(i);
	}
}
```

To sure that removing an element doesn’t change the positions of elements that have not yet been checked, the for loop starts at the end of the vector and runs backwards.

### 3.Letter Frequency
```c++  
letterCounts[toupper(ch) - ‘A’] ++;   // subtracting the ASCII value ‘A’
```

### 4.Conversion a string and a number
```c++
#include <sstream> //include this to use string streams
#incldue <string>
int main()
{
	std::string inputString = "1234 12.3 44";
	std::istringstream istr(inputString);
	int i1, i2;
	float f;
	istr >> i1 >> f >> i2;
	//i1 is 1234, f is 12.3, i2 is 44

	int number = 1234;
	std::ostringstream ostr;  //output string stream
	ostr << number;  //use the string stream just like cout,
	//except the stream prints not to stdout but to a string
	
	std::string theNumberString = ostr.str();  //the str() function of the strem
	//returns the string

	//now theNumberString is "1234"
}
```

### 5.Pointers: star(*)
```c++
 *p =20;
```

The star (*) is an indirection operator that forces the system to first retrieve to the contents of p, then access the location whose address has just been retrieved from p, and only afterward, assign 20 to this location. 
 
### 6.Struct

Like any type definition, a struct is a definition, not a declaration. That is, it defines only a data type; no memory is allocated.

Memory is allocated for the struct variables only when you declare them.
The dot (.) operator is called the member access operator.

### 7.Class

Scope resolution operator (::), the name of the class, followed by the scope resolution operator, followed by the function name.


--------------------Last edit was on April 16, 2016------------------------

























