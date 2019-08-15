
## difference-between-emplace_back-and-push_back-function

### vector::push_back function

The vector push_back help us in many ways by allowing easy appending of data to the current existing vector in case prior memory reserve is not made. Vector in fact is a perfect substitution for an array.

```c++
int arr[3] ={2 ,34 , 900}; //arr can store only three integers
vector<int> vec={23};

vec.push_back( 34 );     //second element added

vec.push_back( 100 );    //third element added

vecc.push_back( 500 );   //fourth element added

/* ..you can keep adding data **/

for(auto elem:vec )       //accessing vector element using range-based for loop
{
    cout<< elem << ” ” ;   // 23 34 100 500
}
```

When the type is a user-defined type: class or structure, and if the constructor accepts only one argument then you can simply use push_back to add an object of the type or you can pass a data of the argument type of the constructor.

```c++
class Data
{
    int i;
public:
    Data(int ii):i(ii) {}

    ~Data ( ) { }
};

vector<Data> vec;

vec.push_back( Data(78) ) ;///adding an object of Data

vec.push_back( 89 ); ///work fine,Data constructor accept integer
```

Passing raw data is acceptable because the push_back function will create a temporary object of the type, this temporary object is again copied into the vector.

- void push_back(T&&x);

This function increase the capacity of the vector if the storage is not reserve previously. However, if the storage is reserve using the reserve() function, calling this function only increases it's size not it's capacity. The capacity will increase when you add data more than the reserved size.

- void push_back(const T&x);

This function is call when passing a **reference** as the argument. In calling this function also the capacity remains the same if the vector has reserve some storage else the capacity will increased along with it's size.

### vector::push_back function

The emplace_back function like the push_back function append data at the end of the vector.It is added in C++11.

```c++
vector<int> vec ={ 23 , 56 , 89 } ;

vec.emplace_back( 981 ) ;
vec.push_back( 1425 ) ;

for( auto elem:vec )
{
    cout<< elem << ” ” ;    // 23 56 89 981 1425
}
```

In the code above the push_back and emplace_back perform exactly the same operation:append data at the end of the vector.

### Difference

#### 1: push_back accepts only object of the type if the constructor accept more than one arguments,whereas emplace_back accept arguments of the constructor of the type

```c++
class Test
{
    int i;
public:
    Test(int ii):i(ii) { } //constructor
    int get const() { return i; }
    ~Test( ) { }
};

int main( )
{
    vector<int> vec={ 21 , 45 };
    vec.push_back( Test(34) ) ;///Appending Test object by passing Test object
    vec.push_back( 901 ) ; //Appending Test object but int data is passed,work fine
    vec.emplace_back( Test(7889) ); ///work fine
    vec.emplace_back( 4156 ) ; ///work fine

    for( auto elem:vec )
    {
        cout<< elem.get() << ” ” ;
    }
    cin.get( );
    return 0;
}
```

Class with constructor accepting two arguments:

```c++
class New
{
	int i;
	string st;
public:
	New(int ii, string s):i(ii) , st(s) { }
	int getInt const() { return i; }
	string getString const () { return st; }
	~New( ) { }
};

int main( )
{
	vector<int> vec={ {21,”String”} , New{45 , “tinger”} } ;
	vec.push_back( New(34 , “Happy” ) ) ; //Appending Test object
	vec.push_back( 901 , “Doer” ) ; //Error!
	vec.emplace_back( New(78 , “Gomu gomu” ) ); ///work fine
	vec.emplace_back( 41 , “Shanks” ) ; ///work fine
	for( auto elem:vec )
	{
		cout<< elem.getInt( ) << ” ” << elem.getString( ) << endl ;
	}

	cin.get( ) ;
	return 0 ;
}
```

Output:

```c++
21 String
45 Tinger
34 Happy
78 Gomu gomu
41 Shanks
```

The first push_back call suceeds because we have passed a New object, but the second push_back call fails because the push_back simply cannot accept two arguments. The emplace_back on the other hand happily accept the two arguments passed to it.

#### 2:Efficiency

- built-in type

If the type is a built-in type there is no difference in efficiency between push_back and emplace_back function.

- User-defined type

```c++
class Dat
{
	int i;
public:
	Dat(int ii):i(ii) { }
	~Dat( ) { }
};

vector<Dat> vec ;
vec.push_back( Dat(89) ); // efficiency lesser
vec.push_back( 67 ); // efficiency same as the above push_back
vec.emplace_back( Dat(67) ); // efficiency same as the above push_back
vec.emplace_back( 890 ); // efficiency is more than the above three
```

In the first push_back call a temporary object is created first by calling the constructor. But in the second push_back and emplace_back function call the object is created directly into the vector. And there is no need for copying the object into the vector.

If the constructor accept more than one argument and if push_back is used our only option is to pass an object of the type explicitly. This is turn will lead to the creation of temporary object which is then copied into the vector. This means using push_back to add object of class with more than one argument will always produce an overhead.

But note emplace_back does not have such limitation - whether the class has more than one argument or not it can always accept raw data. So it is better that you use emplace_back function which increase the efficiency of the code by constructing the object directly into the memory managed by the vector.

```c++
class Dat
{
	int i ;
	string ss ;
	char c ;
public:
	Dat(int ii , string s, char cc):i(ii) , ss(s) , c(cc) { }
	~Dat( ) { }
};

vector<Dat> vec ;
vec.reserve( 3 );
vec.push_back( Dat(89, “New” , ‘G’ ) ); //efficiency lesser
vec.push_back( 678 , “Newer” , ‘O’ ); //error,push_back can’t accept three arguments
vec.emplace_back( 890 , “Newest” , ‘D’ ); //work fine,efficiency is also more
```

![](https://corecplusplustutorial.com/image/vector_header/difference-between-push_back-and-emplace_back.png)

The copying process is omitted in emplace_back function call.

## Reference

- [C++ difference between emplace_back and push_back function](https://corecplusplustutorial.com/difference-between-emplace_back-and-push_back-function/)
