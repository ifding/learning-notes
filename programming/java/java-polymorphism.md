# Java Polymorphism

>iFDING

>02/19/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE Polymorhism post.
</div>


### Polymorphism

Subclasses of a class can define their own unique behaviors and yet share some of the same functionality of the parent class. In other words. Polymorphism allows you define one interface and have multiple implementations.

* Allowing objects having different internal structures to share the same external interface

* An operation may exhibit different behavior in different instances.

* The behavior depends on the types of data used in the operation.

* Polymorphism is extensively used in implementing inheritance.

Two types of polymorphism: method overriding and method overloading.

#### Method Overriding

Child class has the same method as of base class. In such cases child class overrides the parent class method without even touching the source code of the base class.

```java
public class BaseClass
{
	public void methodToOverride()  //Base class method
	{
        System.out.println("I'm the method of BaseClass");
    }
}
public class ChildClass extends BaseClass
{
	public void methodToOverride()  //Child class method
	{
        System.out.println("I'm the method of ChildClass");
    }
}

public class TestMethod
{
	public static void main(String args []) {

	    BaseClass obj1 = new BaseClass();  //BaseClass reference and object

	    BaseClass obj2 = new ChildClass();  //BaseClass reference but ChildClass object

	    obj1.methodToOverride();  //Calls the method from BaseClass class

	    obj2.methodToOverride();  //Calls the method from ChildClass class
    }
}
```

Output:

```
I'm the method of BaseClass
I'm the method of ChildClass
```

**Rules for Method Overriding**

1. It is also known ad Runtime polymorphism
2. Applies only to inherited methods
3. Object type (NOT reference variable type) determines which overridden method will be used at runtime
4. Static and final methods cannot be overridden
5. Constructors cannot be overridden
6. Overriding method can have different return type
7. Overriding method must not have more restrictive access modifier
8. Abstract methods must be overridden

**Super keyword in Overriding**

When invoking a superclass version of an overridden method the super keyword is used.

```java
class Vehicle {
	public void move() {
	    System.out.println("Vehicle is moving!");
    }
}

class Car extends Vehicle {
	public void move() {
	    super.move();   //invokes the super class method
	    System.out.println("Car is moving!");
    }
}

public class TestCar {
	public static void main (String args []) {
	    Vehicle v = new Car();   // Vehicle reference but Car object
	    v. move();               // Calls the method in Car class
    }
}
```

Output:

```
Vehicle is moving!
Car is moving!
```

#### Method Overloading

In Java, it is possible to define two or more methods of same name in a class, provided that there argument list or parameters are different.

1. It must use the type and/or number of arguments to determine which version of the overload method to actually call.
2. It may have different return types; the return type alone is insufficient to distinguish two versions of a method.
3. When Java encounters a call to an overloaded method, it simply executes the version of the method whose parameters match the arguments used in the call.
4. It allows the user to achieve compile time exceptions.
5. An overloaded method can throw different exceptions.
6. It can have different access modifiers.

```java
class Calculation{  
    void sum(int a,int b){ System.out.println(a+b);}  
    void sum(int a,int b,int c){ System.out.println(a+b+c);}  

    public static void main(String args[])
    {  
       Calculation obj=new Calculation();  
       obj.sum(10,10,10);  
       obj.sum(20,20);  
    }  
}  
```

Output: 30, 40

**Rules for Method Overloading**

1. It is also known as compile time polymorphism
2. Overloading can take place in the same class or in its subclass
3. Constructor in java can be overloading
4. Overloaded methods must have a different argument list
5. The parameters may differ in their type or number, or in both
6. They may have the same or different return types

Source from: [Polymorphism in Java – Method Overloading and Overriding](http://beginnersbook.com/2013/03/polymorphism-in-java/)

### Types of polymorphism

There are two types of polymorphism in java:

* Runtime polymorphism (Dynamic polymorphism)

* Compile time polymorphism (Static polymorphism)

#### Runtime polymorphism

Method overriding is a perfect example of runtime polymorphism. In this kind of polymorphism, reference of `class X` can hold object `class X` or an object of any sub classes of `class X`. For example, if `class Y` extends `class X`, then both of the following statements are valid:

```
Y obj1 = new Y();
X obj2 = new Y();
//Parent class reference can be assigned to child object
```

Since in method overriding both the classes (base class and child class) have same method, compile doesn't figure out which method to call at compile time. In this case JVM decides which method to call at runtime that's why it is known as runtime or dynamic polymorphism.

#### Compile time polymorphism

Compile time polymorphism is nothing but the method overloading in java. In simple terms we can say that a class can have more than one methods with same name but with different number of arguments or differnt types of arguments or both.

```java
class X
{
   void methodA(int num)
   {
       System.out.println ("methodA:" + num);
   }
   void methodA(int num1, int num2)
   {
       System.out.println ("methodA:" + num1 + "," + num2);
   }
   double methodA(double num) {
       System.out.println("methodA:" + num);
       return num;
   }
}

class Y
{
   public static void main (String args [])
   {
       X Obj = new X();
       double result;
       Obj.methodA(20);
       Obj.methodA(20, 30);
       result = Obj.methodA(5.5);
       System.out.println("Answer is:" + result);
   }
}
```

Output:

```
methodA:20
methodA:20,30
methodA:5.5
Answer is:5.5
```

In such scenario, compiler is able to figure out the method call at compile time that is the reason it is known as compile time polymorphism.

Source from: [Types of polymorphism in java- Runtime and Compile time polymorphism](http://beginnersbook.com/2013/04/runtime-compile-time-polymorphism/)

### Polymorphism Implementation Mechanism

A normal (non-polymorphic) method's address is determined at compile time, and the bytecode instruction to invoke it can call the method directly. This is sometimes called early binding (or static binding) because a method name is bound to a memory address at compile time. This is efficient but it isn't always convenient! Sometimes it isn't clear what the type of some variable should be until we run the program, because it might depend on user input, random numbers, or other external data such as from a file.

Consider this code:

```java
public class PolyTest
{
    public static void main ( String [] args ) throws Exception
    {
        Number num = NumberFormat.getInstance().parse(args[0]);

        System.out.println( "The number " + num.toString() + " has class: " + num.getClass() );
    }
}
```

```
$ javac PolyTest
$ java PolyTest 100
The number 100 has class: java.lang.Long
$ java PolyTest 100.00
The number 100.00 has class: java.lang.Double
```

So what can the compiler do? When compiling the main method it can't bind the method name toString to an address. Instead the compiler defers the binding until run time, by using bytecode that will look up the address of the correct toString  method. This is why polymorphism is also known as late binding, or dynamic binding. (In some languages such as C++ polymorphic methods are known as virtual methods or functions)

Without polymorphism this program would be more complicated. You'd have to use some custom method to convert the args[0] String to a number, that also somehow returns a type name. You would then need a switch statement or an if chain to test the type name and call the right method yourself. This sort of code is very ugly, easily broken, and hard to maintain or enhance. That is the main reason why Java supports polymorphic methods.

#### How It Works

The addresses of an object's polymorphic methods is stored in a `method table` in the object. When invoking some polymorphic method at runtime the method name is looked up in this table to get the address. A method table contains the names and addresses of the object's dynamically bound (polymorphic) methods. The method table is the same for all objects belonging to tha same class, So it stored in the Class object (for the object's type, here Integer or Double). (In other languages method tables are called vtables).

It is illustrative to see how the system constructs a method table for some class such as Integer. Initially the method table is empty. Then the method table is filled with the polymorphic methods in the most distant ancestor class, usually the Object class:

Integer Method Table part 1

| Method Name        | Address    | Commment                        |
| ------------------ |:----------:|:-------------------------------:|
| Object.toString    | 111        | Object.toString method address  |
| ...                | ...        | 10 Additional methods           |

This list is added to (and existing entries modified) by the polymorphic methods in the next most-distant ancestor class,here its the Number class. If you look at the API you will find the Number class doesn't override any methods but does add six new ones to the table. So the toString entry in the method table is left unmodified:

Integer Method Table part 2

| Method Name        | Address    | Commment                        |
| ------------------ |:----------:|:-------------------------------:|
| Object.toString    | 111        | Object.toString method address  |
| Number.intValue    | 222        | Number.intValue method address  |
| ...                | ...        | 15 Additional methods           |

This continues until the method tables of all parent classes have been merged. Finally the method table is updated again with the Integer class polymorphic methods. Now toString is overridden:

Integer Method Table part 3

| Method Name        | Address    | Commment                        |
| ------------------ |:----------:|:-------------------------------:|
| Object.toString    | 333        | Object.toString method address  |
| Number.intValue    | 444        | Number.intValue method address  |
| Integer.parseInt   | 555        | Number.longValue method address |
| ...                | ...        | Additional methods              |

The "internal" method names in the table includes the orginal class name. Overriding some method merely changes the address in the method table slot but doesn't change the name. This is why the javap output shows the polymorphic method name as `Object.toString` and not merely `toString` or `Integer.toString`.

Take  a look at the bytecode for this method call (use "javap -verbose PolyTest"), which reads:

```
40:  invokevirtual   #11; //Method java/lang/Object.toString:()Ljava/lang/String;
```

The "#11" refers to a method name in the method table (as mentioned above this is an index into the "constant pool"), in this case the method named `Object.toString` that returns a String. The invokevirtual bytecode instruction causes the JVM to treat the value at #11, not as an address (as it would be for early binding), but the name of a method to look up in the method table for the current project. And that's how it works!

Source from: [A Detailed Look at Java's Polymorphism Implementation](http://wpollock.com/Java/PolyMorphism.htm)



