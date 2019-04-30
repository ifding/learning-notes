# 4 types of references in Java

>iFDING

>02/17/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE references types post.
</div>

### Types of References in Java

Java automatically manages the memory on the behalf of the programmers. We need not to worry about freeing the memory after the objects are no more used. **Garbage Collector Thread** does this for us. This thread is responsible for sweeping out unwanted objects from the memory.

Depending on how objects are garbage collected, referencs are grouped into 4 types: Strong, Soft, Weak, Phantom References.

#### Strong References

These type of references we use daily while writing Java code. Any object in the memory which has active strong reference is not eligible for garbage collection.

For example, when you create a class Employee, and you create a reference to a new employee object like this

```java
Employee emp = new Employee();
```

You care actually creating a strong reference. An object to which a reference is created in this way will be eligible for garbage collection only when the reference is to it is pointed to null. Like this:

```java
emp = null;
```

and there are no more references created to this object until the garbage collector runs again, this object will become eligible for garbage collection, and is most likely to be garbage collected.

The 3 remaining Reference types are subclasses of an abstract class Reference (java.lang.ref.Reference)

#### Weak References

A weak reference is reference that isn't strong enough to force an object to remain in memory. Weak references allow you to leverage the garbage collector's ability to determine rechability for you, so you don't have to do it yourself. What that means is that the garbage collector will make it eligible for garbage collection because object only has a week reference pointing to it.

For example, you need to maintain some metadata relaed to a user per database connection. In such a case you will be tempted to use a hashmap where you can store the database connection as the key and the user metadata as the value. However, this approach has one drawback. 

When the database connection has been cleaned up by some other part of the application, then you need to ensure the removal of the connection from the hashmap as well. If you forget to do such a thing, a reference to the connection will remain in the hashmap thereby preventing it from being garbage collected. Over a period of time, you are bound to end up with a very large hashmap, a clear indication of a memeory leak. And JVM will eventually spit out a OutOfMemoryError.

So, what do you do in such cases? Weak references to the rescue! Create a weak reference to the object:

````java
DBConnection emp = new Employee(); 
WeakReference<DBConnection> weakRef = new WeakReference<DBConnection>(emp);
````

If at any future point in of time dufing the execution of the program, if the garbage collector detects that the only reference to the actual DBConnection object is a Weak reference, then it will mark the object for garbage collection.

In our database example, we could effectively create a weak reference of the Database connection and store it in the WeakHashMap and the metadata of the user as the value in the hashmap. When the application no longer holds a strong reference to the Database connection, the only reference to the database connection object will be the one that we have via the WeakReference entry in the WeakHashMap. When the object is garbage collected, the entry from the WeakHashMap will be removed.

#### Soft References

A soft reference is exactly like a weak reference, except that it is less eager to throw away the object to which it refers. An object which is only weakly reachable (the strongest references to it are WeakReferences) will be discarded at the next garbage collection cycle, but an object which is softly reachable will generally stick around for a while.

In order to create a SoftReference to an object, you first need to create a strong reference to that object and then pass that object as a parameter to the constructor of the SoftReference object.

```java
Employee emp = new Employee();  //1
SoftReference<Employee> softRef = new SoftReference<employee>(emp);  //2
```

In line #1, an object(referent) is created and allocated memory on the heap that represents the Employee object, a stong reference is created to the newly created employee object. We call this reference 'emp'.

In line #2, a new SoftReference object is created and allocated memory on the heap. This is a special object because it contains an internal reference to the object(referent) passed to it in the constructor, the actual Employee object on the heap.

Again, in line #2, a strong reference is created to the SoftReference object on the heap. We call this reference 'softRef'.

In total, we have 2 strong references created. And the object that represents the soft reference, internally holds a reference to the actual employee object.

So what is it that makes soft references useful? Let us assume that at a later point of time, we nullify the 'emp' reference and no new strong references were created to this emp object.

```java
emp = null;
```

Now lets assume that the garbage collector decides to run at this point of time. What it will see is that the current employee object only has a soft reference pointing to it, and no strong references. The garbage collector may optionally choose to reclaim the memory occupied by the employee object. But what makes soft references even more special is the fact that the garbage collector will always reclaim the memory of objects that have only soft references pointing to them before it throws an OutOfMemory Error. This gives the garbage collector some sort of a last chance to save the life of the JVM.

At any given point of time, if the garbage collector has not yet reclaimed the memory of the actual referent object, you can get a strong reference to the referent via the get method.

```java
Employee resurrectedEmp = softRef.get();
```

The above code resurrects the employee object in the sense that the gerbage collector will not consider it a candidate for garbage collection because it now has a Strong Reference pointing to it.

This makes soft references highly useful in creating object pools, where the size of the pool needs to by dynamic. If you do not know the size of a pool when you begin, or choose not to set a minimum or a maximum size on the object pool, instead you want it to grow dynamically, and at the same time, you want to give the JVM a chance to cleanup unused objects, in that case SoftReferences are a perfect fit.

#### Phantom References

Unlike SoftReference and WeakReference, phantom references are not automatically cleaned by the garbage collector as they are enqueued. An object that is reachable via phantom references will remain so until all such references are cleared or themselves become unreachable.

A phantom is always associated with a reference queue during construction time. This is because phantom references are enqueued in the queue only when the object is about to be garbage collected, after the finalize method(if any) has been executed on it. 

Calling a get() on the Phantom reference always return null. And that is quite appropriate because the finalize function has already run on the referent object. So, there should be no 'legal' way of resurrecting the object (resurrecting i.e. creating a strong reference). 

At first this may seem to make no sense, since, what use is a phantom reference if we can't extract the referent from it? But on giving it a deep thought, you would realize that this is not the reason why phantom references are meant to be useful in the first place. It is the time at which the JVM puts a phantom reference in the reference queue that makes its use so intuitively amazing. 

### Reference

* [Understanding Weak References Blog](https://community.oracle.com/blogs/enicholas/2006/05/04/understanding-weak-references)

* [Reference Types In Java - Part 1](https://dzone.com/articles/reference-types-java-part-1)


