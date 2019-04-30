# Java Inheritance

>iFDING

>02/16/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE Inheritance post.
</div>

### Inheritance

In Java, classes can be derived from other classes, thereby inheriting fields and methods from those classes.

A class that is derived from another class is called s subclass(also a derived class, extended class, or child class). The class from which the subclass is derived is called a superclass(also a base class or a parent class).

A subclass inherits all the members(fields, methods, and nested classes) from its superclass. Constructors are not members, so they are not inherited by subclasses, but the constructor of the superclass can be invoked from the subclass.

#### Object as a Superclass

All classes in the Java Platform are Descendants of `Object`.

![classes-object.gif](../images/classes-object.gif)

At the top of the hierarchy, `Object` is the most general of all classes. Classes near the bottom of the hierarchy provide more specialized behavior.

* A subclass inherits all of the `public` and `protected` members of its parent, no matter what package the subclass is in. If the subclass is in the same package as its parent, it also inherits hte package-private members of the parent.
* A subclass does not inherit the `private` members of its parent class. 
* A nested class has access to all the private members of its enclosing class - both fields and methods.

The `Object` class sits at the top of the class hierarchy tree. Every class is a descendant, direct or indirect, of the Object class. The methods inherited from Object that are:

* protected Object clone() throws CloneNotSupportedException

      Creates and returns a copy of this object.

* public boolean equals(Object obj)

      Indicates whether some other object is "equal to" this one.

* protected void finalize() throws Throwable

      Called by the garbage collector on an object when garbage collection determines that there are no more references to the object

* public final Class getClass()

      Returns the runtime class of an object.

* public int hashCode()

      Returns a hash code value for the object.

* public String toString()

      Returns a string representation of the object.

The notify, notifyAll, and wait methods of Object all play a part in synchronizing the activities of independently running threads in a program. There are five of these methods:

```
public final void notify()
public final void notifyAll()
public final void wait()
public final void wait(long timeout)
public final void wait(long timeout, int nanos)
```

### Reference
* [Java Documentation](https://docs.oracle.com/javase/tutorial/java/index.html)

* [Java Tutorial](https://www.tutorialspoint.com/java/index.htm)
