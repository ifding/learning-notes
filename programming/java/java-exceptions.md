# Exceptions in Java

>iFDING

>02/19/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE exceptions post.
</div>

###  How to Throw Exceptions

The Java provides numberous exception classes. All the classes are descendats of the Throwable class.

All methods use the throw statement to throw an exception. The throw statement requires a single argument: a throwable object. Throwable objects are instances of any subclass of the Throwable class. Here's an example of a throw statement.

```
throw someThrowableObject;
```

Throwable class has two direct descendants: `Error` and `Exception`.

![exceptions-throwable.gif](../images/exceptions-throwable.gif)

Source from: [JavaDoc](https://docs.oracle.com/javase/tutorial/essential/exceptions/throwing.html)

#### Error Class

When a dynamic linking failure or other hard failure in the Java virtual machine occurs, the virtual machine throws an Error. Simple programs typically do not catch or throw Errors.

Error class represents the errors which are mainly caused by the environment in which application is running. For example, **OutOfMemoryError** occurs when JVM runs out of memory or **StackOverFlowError** occurs when stack overflows.

#### Exception Class

Most programs throw and catch objects that derive from the Exception class. An Exception indicates that a problem occured, but it is not a serious system problem. Most programs you write will throw and catch Exceptions as opposed to Errors.

Exception class represents the exceptions which are mainly caused by the application itself. For example, NullPointerException occurs when an application tries to access null object or ClassCastException occurs when an application tries to cast incompatible class types.

#### Error Vs Exception

1. Recovering from Error is not possible. The only solution to errors is to terminate the execution. Where as you can recover from Exception by using either try-catch blocks or throwing exception back to caller.

2. You will not be able to handle the Errors using try-catch blocks. Exceptions can be handled using try-catch blocks and can make program flow normal if they happen.

3. All Errors belongs to only one category i.e. unchecked. Where exceptions in java are divided into two categories - checked and unchecked.

4. Complier will not have any knowledge about unchecked exceptions which include Errors and sub classes of RunTimeException because they happen at run time. Where as compliler will have knowledge about checked Exceptions. Complier will force you to keep try-catch blocks if it sees any statements which may throw checked exceptions.

5. Exceptions are related to application where as Errors are related to environment in which application is running.

| Errors                             | Exceptions                                           |
| ---------------------------------- |:----------------------------------------------------:|
| type java.lang.Error               | type java.lang.Exception                             |
| All errors are unchecked type      | both checked and unchecked type                      |
| Errors happen at run time          | Unchecked not known to complier, occur at run time   |
| not be known to complier           | Checked are known to complier                        |
| Impossible to recover from errors  | recover from exceptions through try-catch blocks     |
| Mostly caused by environment       | mainly caused by the application itself              |
| Examples: StackOverflowError       | Checked Examples: SQLException, IOException          |
| OutOfMemoryError                   | Unchecked: ArrayIndexOutOfBoundException,            |
|                                    | ClassCastException, NullPointerException             |

Source from:[Difference Between Error Vs Exception In Java](http://javaconceptoftheday.com/difference-between-error-vs-exception-in-java/)

### Understanding the OutOfMemoryError

JavaDoc:

```
Thrown when the Java Virtual Machine cannot allocate an object because it is out of memory, and no more memory could be made available by the garbage collector. OutOfMemoryError objects may be constructed by the virtual machine as if suppression were disabled and/or the stack trace was not writable.
```

#### Java heap space

Quite common. This is the JVM's way to announce you that there is no more room in the virtual machine heap area. You are trying to create a new object, but the amount of memory this newly created structure is about to consume is more than the JVM has in the heap. The JVM has tried to free memory by calling GC before throwing in the towel, but without any success. 

The fastest way to get rid of the symptoms is to increase the heap via -Xmx parameter.

More options: [How to fix java.lang.OutOfMemoryError: Java heap space](http://opcodesolutions.com/tech/solve-java-lang-outofmemoryerror-java-heap-space/)

#### PermGen space

Quite common during redeploys. It is pretty much the same message as the first one, but instead of the heap you are now trying to allocate memory in the `Permanent Generation` area. And again, you do not have enough room, so the JVM native code is kind enough to let you know about it.

This message tends to disappear if you increase the -XX:MaxPermSize parameter.

#### GC overhead limit exceeded

A bit of a different beast. Instead of the missing heap / permgen the JVM is signaling that your application is spending too much time in garbage collection wiht litte to show for it. By default the JVM is configured to throw this error if you are spending more than 98% of the toal time in GC and less than 2% of the heap is recovered after the GC.

In the rare cases where it makes sense to diable it, add -XX:-UseGCOverheadLimit to your startup scripts.

The above three OutOfMemoryError messages make up to 98% of the cases Plumbr detects. 

Source from: [Understanding the OutOfMemoryError](https://plumbr.eu/blog/memory-leaks/understanding-java-lang-outofmemoryerror)

