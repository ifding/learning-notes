# Java Encapsulation

>iFDING

>02/16/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE Encapsulation post.
</div>

### Encapsulation

A mechanism of wrapping data (variables) and code together as a single unit. The variables of a class will be hidden from other classes, and can be accessed only through the methods of their current class. 

To achieve encapsulation in Java:

* Declare the variables of a class as private.
* Provide public setter and getter methods to modify and view the variable values.
* By providing only setter or getter method, you can make a class read-only or write-only (control over the data).

#### Data Types & Wrapper class

| Primitive Data Types   | Wrapper class    |
| ---------------------- |:----------------:|
| byte (8-bit)           | Byte             |
| short (16-bit)         | Short            |
| int (32-bit)           | Integer          |
| long (64-bit)          | Long             |
| float (32-bit)         | Float            |
| double (64-bit)        | Double           |
| boolean                | Boolean          |
| char (16-bit)          | Character        |

Besides managing primitive values, wrapper classes contain a set of methods used to convert values to different types.

Wrapper objects are creating using:

* class contructor and new operator
* static method: valueOf()

for example:

```java
Integer intObject1 = new Integer(3);
Integer intObject2 = Integer.valueOf("5");
Integer intObject3 = Integer.valueOf("1000",2);  //8 in base 2
Character charObject = new Character('a');
```

#### Autoboxing and Unboxing

Autoboxing is the process without explicit call to constructors. Unboxing is the reverse process that allows extracting a value without an explicit call to byte, short, int, long, float, double methods.

```java
Integer intObj1 = 15;    //autoboxing
int intPrim = intObj1;   //unboxing
Boolean boolValue = true;
if(boolValue)    //unboxing
   System.out.println("True!");
```

#### Wrapper objects: == & equals

Managed by references:
* = does a shallow copy, copies values between references
* == compares references values
* equals() is used to compare objects values

All wrapper classes are immutable, changing the value of a wrapper object it is created a new object.

```java
Integer intObj1 = 100;      //autoboxing
Integer intObj2 = intObj2;  //shallow copy
Integer intObj3 = 100;
Integer intObj4 = 101;

if(intObj1 == intObj2)
    System.out.println("The same object!");

if(intObj1.equals(intObj3))
    System.out.println("Objects with same value!");

intObj1++;        //increment the value, create a new object

if(intObj1 == intObj4)    //different objects
    System.out.println("The same object!");
else
    System.out.println("Different objects!");

```

Strings are a sequence of characters. In Java, strings are treated as objects.

Like any other object, you can create String objects by using the new keyword and a constructor.

```java
char[] helloArr = {'h','e','l','l','o'};
String helloStr1 = new String(helloArr);

String helloStr2 = "hello world!";
```

String equals() method returns true if the String are equal; false otherwise.

String == returns true if the string are same objects.


### Reference

* [Java Documentation](https://docs.oracle.com/javase/tutorial/java/index.html)
