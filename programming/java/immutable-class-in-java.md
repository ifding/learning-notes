# Immutable class in java

>iFDING

>02/18/2017


<div class="message">
<b>Reader beware:</b> this is a JavaSE immutable class post.
</div>


Immutable objects are instances whose state doesn't change after it has been initialized. For example, String is an immutable class and once instantiated its value never changes.

### Why String is immutable

One of the popular interview question. In java, string is immutable and final, java runtime maintains a `String Pool` that make it a special class.

1. **String Pool** is possible only because String is immutable in java.Because different String variables can refer to same String variable in the pool, Java Runtime saves a lot of java heap space. If String would not have been immutable, then String interning would not have been possible because if any variable would have changed the value, it would have been reflected to other variables also. String interning is a method of storing only one copy of each distinct string value, which must be immutable.

2. If String is not immutable then it would cause severe **security threat** to the application. For example, database username, password are passed as String to get database connection and in **socket progamming** host and port details passed as String. Since String is immutable it's value can't be changed otherwise any hacker could change the referenced value to cause security issues in the application.

3. Since String is immutable, it is **safe for multithreading**. A single String instance can be shared across different threads. This avoid the usage of synchronization for thread safety. Strings are implicitly thread safe.

4. Strings are used in **java classloader** and immutability provides security that correct class is getting loaded by Classloader. For example, think of an instance where you are trying to load java.sql.Connection class but the referenced value is changed to myhacked.Connection class that can do unwanted things to your database.

5. Since String is immutable, its **hashcode** is cached at the time of creation and it doesn't need to be calculated again.

This makes it a great candidate for key in a Map and it's processing is fast than other HashMap key objects. This is why String is mostly used Objects as HashMap keys.

More details, please Read: [Why String in immutable in Java](http://www.journaldev.com/802/string-immutable-final-java)

### Create immutable class

Immutable class is good for caching purpose, don't need to worry about the value changes. Other benefit of immutable class is that it is inherently thread-safe in case of multi-threaded environment.

#### Final class

To create immutable class in Java, you have to do following steps.

1. Declare the class as final so it can't be extended.

2. Make all fields private so that direct accesss is not allowed.

3. Don't provide setter methods for variables.

4. Make all **mutable fields final** so that it's value can be assigned only once.

5. Initialize all the fields via a constructor performing deep copy.

6. Perform cloning of objects in the getter methods to return a copy rather than returning the actual object reference.

[Read a Final class sample](http://www.journaldev.com/129/how-to-create-immutable-class-in-java) 

#### Builder pattern

If the immutable class has a lot of attributes and some of them are optional, we can use builder pattern to create immutable classes.

* The immutable class should have only getter methods.

* The immutable class will have a private constructor with Builder object as parameter that will be used to create the immutable class.

* If the immutable class attributes are not immutable, for example HashMap, we should perform deep copy or cloning to avoid modification of its attributes.

* Using builder pattern to create immutable class is a good approach when the number of arguments in the Constructor is more that can cause confusion in their ordering.

[Here is the implementation of an immutable class using Builder pattern.](http://www.journaldev.com/1432/how-to-create-immutable-class-in-java-using-builder-pattern)
