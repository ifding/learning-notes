
# Working with Classes

"Ease yourself into the language. Don't feel you have to use all of the features, and don't try to use them all on the first day." -- Bjarne Stroustrup (the creator of C++)

## Operator Overloading

To overload an operator, you use a special function form called an _operator function_. An operator funciton has the following form, where _op_ is the symbol for the operator being overloaded: 

`operator_op_(argument-list)`

For example, `operator+()` overloads the `+` operator and `operator*()` overloads the `*` operator. The `op` has to be a valid C++ operator. Suppose that you have a `Salesperson` class for which you define an `operator+()` member function to overload the `+` operator so that it adds sales figures of one salesperson object to another. Then, if `district2`, `sid`, and `sara` are all objects of the `Salesperson` class:
```c++
Salesperson sid, sara;
district2 = sid + sara;
```

The compiler, recognizing the operands as belonging to the `Salesperson` class, replaces the operator with the corresponding operator function:
```c++
district2 = sid.operator+(sara);
```

The function then uses the `sid` object implicitly (because it invoked the method) and the `sara` object explicitly (because it's passed as an argument) to calculate the sum, which it then returns.
```c++
Salesperson john;
district2 = sid + sara + john;		// use addition as defined for Salesperson objects
district2 = sid.operator+(sara + john);		// addition is a left-to-right operator
district2 = sid.operator+(sara.operator+(john));	// sara.operator+(john) returns a Salesperson object that represents the sum of sara and john.
```

## Introducing Friends

C++ provides another form of access: the _friend_. Friends come in three varieties:
- Friend functions
- Friend classes
- Friend member functions

By making a function a friend to a class, you allow the function the same access privileges that a member functions of the class has.

Remember, the left operand is the invoking object. That is,
```c++
A = B * 2.75;
```

translates to the following member function call:
```c++
A = B.operator*(2.75);
```

But what about the following statement?
```c++
A = 2.75 * B;	// cannot correspond to a member function
```

Because 2.75 is not a type `Salesperson` object, the compiler cannot replace the expression with a member function call.

However, there is another possiblity - using a nonmember function. (Remember, most operators can be overloaded using either member or nonmember functions.) A nonmember function is not invoked by an object; instead, any values it uses, including objects, are explicit arguments. Thus, the compiler could match the expression.
```c++
A = 2.75 * B;	// cannot correspond to a member function
```

to the following nonmember function call:
```c++
A = operator*(2.75, B);
```

The function would have this prototype:
```c++
Salesperson operator*(double m, const Salesperson & n);
```

With the nonmember overloaded operator function, the left operand of an operator expression corresponds to the first argument of the operator function, and the right operand corresponds to the second argument.

A new problem: nonmember functions can't directly access private data in a class. But there is a special category of nonmember functions, called `friends`.


### Creating Friends

```c++
friend Salesperson operator*(double m, const Salesperson & n); // goes in class declaration
```

This prototype has two implications"
  - Although the `operator*()` function is declared in the class declaration, it is not a member function.
  - Although the `operator*()` function is not a member function, it has the same access rights as a member function.

Next, write the function definition. Because it is not a member function, you don't use the `Salesperson::` qualifier. Also you don't use the `friend` keyword in the definition.
```c++
Salesperson operator*(double m, const Salesperson & n)  // friend not used in definition
{
	Salesperson result;
	result.total = m * n.total;
	return result;
}
```

In short, a friend function to a class is a nonmember function that has the same access rights as a member function. Class methods and friends are simply two different mechanisms for expressing a class interface.

Actually, you can write this particular friend function as a non-friend by altering the definition so that it switches which value comes first in the multiplication:
```c++
Salesperson operator*(double m, const Salesperson & n)
{
	return n * m; 	// use n.operator*(m)
}
```

### A common kind of friend: Overloading the << operator

In its most basic incarnation, the << operator is one of C and C++'s bit manipulation operators; it shifts bits left in a value. But the `ostream` class overloads the operator, converting it into an output tool. The `ostream` class declaration includes an overload `operator<<()` definition for each of the basic types, e.g. int, double and so on.

- The first version of Overloading <<

Using a friend function:
```c++
void operator<<(ostream & os, const Salesperson & s)
{
	os << s.total;
}
```

The `operator<<()` accesses private `Salesperson` object members directly, it has to be a friend to the `Salesperson` class; it  does not directly access private `ostream` object members, the function does not have to be a friend to the `ostream` class. It only uses the `ostream` object as a whole.

Note that the new `operator<<()` definition uses the `ostream` reference `os` as its first argument. Normally, `os` refers to the `cout` object, as it does in the expression `cout << sid;`

The call `cout << sid` should use the `cout` object itself, not a copy, so the function passes the object as a reference instead of by value. Thus, the expression `cout << sid` causes `os` to be an alias for `cout`. The `Salesperson` object can be passed by value or by reference. Passing by reference uses less memory and time than passing by value.

- The second version of Overloading <<

The first implementation doesn't allow you to combine the redefined << operator with the ones `cout` normally uses:
```c++
cout << " Total value: " << sid;	// can't do
```

Why? Consider the following statements:
```c++
int x = 5;
int y = 8;
cout << x << y;
```

C++ reads the output statement from left to right, meaning it is equivalent to the following: `(cout << x) << y;`

The << operator takes an `ostream` object to its left. The expression `cout << x` satisfies that requirement because `cout` is an `ostream` object. But the output statement also requires that the whole expression (cout << x) be a type `ostream` object because that expression is to the left of `<< y`.

You can take the same approach with the friend function:
```c++
ostream & operator<<(ostream & os, const Salesperson & s)
{
	os << " Total value: " << s.total;
	return os;
}
```

Note that the return type is `ostream &`. Recall that this means that the function returns a reference to an `ostream` object.

Let's break this into separate steps to see how it works. `cout << " Total value: "` displays the string and then is replaced by its return value, `cout`. This reduces the original statement to the following one:
```c++
cout << s.total;
```

> In general, to overload the << operator to display an object of class `c_name`, you use a friend function with a definition in this form:
```c++
ostream & operator<<(ostream & os, const c_name & obj)
{
	os << ... ; 	// display object contents
	return os;
}
```

> Caution
> You use the `friend` keyword only in the prototype found in the class declaration. You don't use it in the function definition unless the definition is also the prototype.


### Overloaded operators: Member versus Nonmember functions

For many operators, you have a choice between using member functions or nonmember functions to implement operator overloading. Typically, the nonmember version is a friend function so that it can directly access the private data for a class.
```c++
Salesperson operator+(const Salesperson & s) const;		// member version
friend Salesperson operator+(const Salesperson & s1, const Salesperson & s2);  // nonmember version
```

The addition operator requries two operands. For the member function version, one is passed implicitly via the `this` pointer and the second is passed explicitly as a function argument. For the friend version, both are passed as arguments.

The compiler can convert the statement:`S1 = S2 + S3;` to either of the following:
```c++
S1 = S2.operator(S3);	// member function
S1 = operator+(S2, S3);	// nonmember function
```

## Automatic conversions

C++ lets you establish conversions to and from class types. First, any class constructor that takes a single argument acts as a conversion function, converting values of the argument type to the class type. C++ invokes the constructor automatically if you assign a value of the argument type to an object.

For example, suppose you have a `String` class with a constructor that takes a `char *` value as its sole argument. Then, if `bean` is a `String` object, you can use the following statement:
```c++
bean = "pinto";		// converts type char * to type String
```

If, however, you precede the consturctor declaration with the keyword `explicit`, the constructor can be used only for explicit conversions:
```c++
bean = String("pinto");		// converts type char * to type String explicitly
```

To convert from a class to another type, you must define a conversion function and provide instruction about how to make the conversion. A conversion function must be a member function. If it is to convert to type `typeName`, it should have the following prototype:
```c++
operator typeName();
```

Note that it must have no declared return type, must have no arguments, and must (despite having noe declared return type) return the converted value. For example, a function to convert type `Vector` to type `double` would have this function form:
```c++
Vector::operator double()
{
	...
	return a_double_value;
}
```

Experience has shown that often it is better not to rely on such implicit conversion functions.


## Reference

- C++ Primer Plus (Developer's Library) 6th Edition by Stephen Prata
