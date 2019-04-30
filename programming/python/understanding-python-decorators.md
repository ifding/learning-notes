# Understanding Python Decorators in 12 Easy Steps

### Introduction

Getting decorators requires understanding several functional programming concepts as well as feeling comfortable with some unique features of Python's function definition and function calling syntax.

I promise to make each piece as simple as possible, and if you understand each piece, you'll understand how decorators work!

I used Python's doctest modules to run the Python code samples, the code looks like an interactive Python console session (>>> and ... indicate Python statements while output has its own line).

#### 1.Functions

Functions in Python are created with the `def` keyword and take a name and an optional list of parameters. They can return values with the `return` values with the `return` keyword.

```
>>> def foo():
...     return 1
>>> foo()
1
```

The body of the function (as with all multi-line statements in Python) is mandatory and indicated by indentation.

#### 2.Scope

In Python functions create a new scope. This means Python looks first in the namespace of the function to find variable names when it encounters them in the function body. Python includes a couple of functions that let us look at our namespaces.

```
>>> a_string = "This is a global variable"
>>> def foo():
...     print locals()
... 
>>> print globals()
{'__builtins__': <module '__builtin__' (built-in)>, 'a_string': 'This is a global variable', '__package__': None, '__name__': '__main__', 'foo': <function foo at 0x10d9cc758>, '__doc__': None}
>>> foo()
{}
```

The builtin `globals` function returns a dictionary containing all the variable names Python knows about. At point #2 I called my function `foo` which prints the contents of the local namespace inside the function. As we can see the function `foo` has its own separate namespace which is currently empty.

#### 3.Variable resolution rules

Python's scope rule is that variable creation always creates a new local variable but variables (including modification) looks in the local scope and then searches all the enclosing scopes to find a match. So if we modify our function `foo` to print our global variable things work as we would expect:

```
>>> a_string = "This is a global variable"
>>> def foo():
...     print a_string # 1
>>> foo()
This is a global variable
```

At point #1 Python looks for a local variable in our function and not finding one, looks for a global variable of the same name.

On the other hand if we try to assign to the gobal variable inside our function it doesn't do what we want:

```
>>> a_string = "This is a global variable"
>>> def foo():
...     a_string = "test" # 1
...     print locals()
>>> foo()
{'a_string': 'test'}
>>> a_string # 2
'This is a global variable'
```

As we can see, global variables can be accessed (even changed if they are mutable data types) but not (by default) assigned to. At point #1 inside our function we are actually creating a new local variable that "shadows" the global variable with the same name. At point #2 that when we check the value of the variable `a_string` it hasn't been changed at all.

#### 4.Variable lifetime

It's also important to note that not only do variables live inside a namespace, they also have lifetimes.

```
>>> def foo():
...     x = 1
>>> foo()
>>> print x # 1
Traceback (most recent call last):
  ...
NameError: name 'x' is not defined
```

The namespace created for our function `foo` is created from scratch each time the function is called and it is destroyed when the function ends.

#### 5.Function arguments and parameters

Python does allow us to pass arguments to functions. The parameter names become local variables in our function.

```
>>> def foo(x):
...     print locals()
>>> foo(1)
{'x': 1}
```

Python has a variety of ways to define function parameters and pass arguments to them, short version: function parameters can be either positional parameters that are mandatory or named, default value parameters that are optional.

```
>>> def foo(x, y=0):  #1
...     return x - y
... 
>>> foo(3,1)  #2
2
>>> foo(3)  #3
3
>>> foo()  #4
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
TypeError: foo() takes at least 1 argument (0 given)
>>> foo(y=1, x=3)  #5
2
```

Python supports named arguments at function call time. Look at point #5 - here we calling a function with two named arguments even though it was defined with one named and one positional parameter.

Simple concept: function parameters can have names or positions.

#### 6.Nested functions

Python allows the creation of nested functions. This means we can declare functions inside of functions and all the scoping and lifetime rules still apply normally.

```
>>> def outer():
...     x = 1
...     def inner():
...         print x  #1
...     inner()  #2
... 
>>> outer()
1
```

At point #1, Python looks for a local variable named x, failing it then looks in the enclosing scope which is another function! The variable x is a local variable to our function `outer` but as before our function `inner` has access to the enclosing scope. At point #2, call inner function. It's important to remember that `inner` is also just a variable name that follows Python's variable rules.

#### 7.Functions are first class objects in Python

Functions are objects, just like everything else.

```
>>> issubclass(int, object) # all objects in Python inherit from a common baseclass
True
>>> def foo():
...     pass
>>> foo.__class__ # 1
<type 'function'>
>>> issubclass(foo.__class__, object)
True
```

Functions are just regular value like any other kind of value in Python. That means you can pass functions to functions as arguments or return functions from functions as return values! 

```
>>> def add(x, y):
...     return x + y
>>> def sub(x, y):
...     return x - y
>>> def apply(func, x, y): # 1
...     return func(x, y) # 2
>>> apply(add, 2, 1) # 3
3
>>> apply(sub, 2, 1)
1
```

At point #1, the variable intended to receive a function is just a normal variable like any other. At point #2, we are calling the function passed into `apply` - parentheses in Python are the call operator and call the value the variable name contains. And at point #3, function names are just variable labels like any other variable.

Python uses functions as arguments for frequently used operations like customizing the `sorted` builtin by providing a function to the key parameter. But what about returning functions as values? Consider:

```
>>> def outer():
...     def inner():
...         print "Inside inner"
...     return inner #1
... 
>>> foo = outer() #2
>>> foo
<function inner at 0x10d9cac80>
>>> foo()
Inside inner
```

At point #1, it returns the variable `inner` which happens to be a function label. There's no special syntax here - our function is returning the inner function which otherwise couldn't be called. Remember variable lifetime? The function `inner` is freshly redefined each time the function `outer` is called but if `inner` wasn't returned from the function it would simply cease to exist when it went out of scope.

At point #2, we can catch the return value which is our function `inner` and store it in a new variable `foo`. We can see that if we evaluate `foo` it really does contain our function `inner` and we can call it by using the call operator (parentheses, remember?).

#### 8.Closures

Let's start with another code sample.

```
>>> def outer():
...     x = 1
...     def inner():
...         print x #1
...     return inner
... 
>>> foo = outer()
>>> foo.func_closure
(<cell at 0x10d9e4750: int object at 0x7fcd4a405858>,)
```

From our last example we can see that `inner` is a function returned by `outer`, stored in a variable named `foo` and we could call it with `foo()`. But will it run? let's consider scoping rules first.

Everything works according to Python's scoping rules - x is a local variable in our function `outer`. When `inner` prints x at point #1, Python looks for a local variable to `inner` and not finding it looks in the enclosing scope which is the function `outer`, finding it there.Our variable x is local to the function `outer` which means it only exists while the function `outer` is running.  

Python supports a feature called **function closures** which means that inner functions defined in non-global scope remember what their enclosing namespaces looked like at defintion time. This can be seen by looking at the `func_closure` attribute of our inner function which contains the variables in the enclosing scopes.

Remember - the function inner is being newly defined each time the function outer is called.

```
>>> def outer(x):
...     def inner():
...         print x # 1
...     return inner
>>> print1 = outer(1)
>>> print2 = outer(2)
>>> print1()
1
>>> print2()
2
```

**Closures** - the fact that functions remember their enclosing scope - can be used to build custom functions that have, essentially, a hard coded argument. We aren't passing the numbers 1 or 2 to our `inner` function but are building custom versions of our inner functions that "remembers" what number it should print.

This alone is a powerful technique - you might even think of it as similar to object oriented techniques in some ways: `outer` is a constructor for `inner` with x acting like a private member variable. And the uses are numerous - if you are familiar with the `key` parameter in Python's `sorted` function you have probably written a lambda function to sort a list of lists by the second item instead of the first. You might now be able to write an `itemgetter` function that accepts the index to retrieve and returns a function that could suitably by passed to the key parameter.

#### 9.Decorators!

A decorator is just a callable that takes a function as an argument and returns a replacement function.

```
>>> def outer(some_func):
...     def inner():
...         print "before some_func"
...         ret = some_func() # 1
...         return ret + 1
...     return inner
>>> def foo():
...     return 1
>>> decorated = outer(foo) # 2
>>> decorated()
before some_func
2
```

We defined a function named `outer` that has a single parameter `some_func`. Inside `outer` we define an nested function named `inner`. The `inner` function will print a string then call `some_func`, catching its return value at point #1. The value of `some_func` might be different each time `outer` is called, but whatever function it is we'll call it. Finally `inner` returns the return value of `some_func() + 1`. When we call our returned function stored in `decorated` at point #2 we get the results of the print and also a return value of 2 instead of the original return value 1 we would expect to get by calling `foo`.

We could say that the variable `decorated` is a decorated version of `foo` - it's `foo` plus something. In fact if we wrote a useful decorator we might want to replace `foo` with the decorated version altogether so we always got our "plus something" version of foo.

Imagine we have a library that gives us coordinate objects. Perhaps they are primarily made up of x and y coordinate pairs.

```
>>> class Coordinate(object):
...     def __init__(self, x, y):
...         self.x = x
...         self.y = y
...     def __repr__(self):
...         return "Coord: " + str(self.__dict__)
>>> def add(a, b):
...     return Coordinate(a.x + b.x, a.y + b.y)
>>> def sub(a, b):
...     return Coordinate(a.x - b.x, a.y - b.y)
>>> one = Coordinate(100, 200)
>>> two = Coordinate(300, 200)
>>> three = Coordinate(-100, -100)
>>> add(one, two)
Coord: {'y': 400, 'x': 400}
>>> sub(one, two)
Coord: {'y': 0, 'x': -200}
>>> add(one, three)
Coord: {'y': 100, 'x': 0}
```

Adding bounds checking to the input arguments of each function and the return value of each function.

```
>>> def wrapper(func):
...     def checker(a, b): # 1
...         if a.x < 0 or a.y < 0:
...             a = Coordinate(a.x if a.x > 0 else 0, a.y if a.y > 0 else 0)
...         if b.x < 0 or b.y < 0:
...             b = Coordinate(b.x if b.x > 0 else 0, b.y if b.y > 0 else 0)
...         ret = func(a, b)
...         if ret.x < 0 or ret.y < 0:
...             ret = Coordinate(ret.x if ret.x > 0 else 0, ret.y if ret.y > 0 else 0)
...         return ret
...     return checker
>>> add = wrapper(add)
>>> sub = wrapper(sub)
>>> sub(one, two)
Coord: {'y': 0, 'x': 0}
>>> add(one, three)
Coord: {'y': 200, 'x': 100}
```

This decorator works just as before - returns a modified version of a function but in this case it does something useful by checking and normalizing the input parameters and the return value, substituting 0 for any negative x or y values.

Advantage: makes our code cleaner, isolating the bounds checking in its own function and applying it to all the functions we care to by wrapping them with a decocrator.

The alternative would be a function call on each input argument and on the resulting output before returning inside each math function and it is undeniable that using the decorator is at least less repetitious in terms of the amount of code needed to apply bounds checking to a function.


##### 10.The @ symbol

```
>>> add = wrapper(add)
```

This pattern can be used at any time, to warp any function. But if we are defining a function we "decorate" it with the @ symbol like:

```
>>> @wrapper
... def add(a, b):
...     return Coordinate(a.x + b.x, a.y + b.y)
```

Python just adds some syntactic sugar to make what is going on very explicit. Using decorators is easy. Even if writing useful decorators like `staticmethod` or `classmethod` would be difficult, using them is just a matter of prepending your function with `@decoratorname`!

#### 11.*args and **kwargs

Let's write a decorator that increments a counter for every function call of every decorated function without changing any of it's decorated functions. This means it would have to accept the calling signature of any of the functions that it decorates and also call the functions it decorates passing on whatever arguments were passed to it.

```
>>> def one(*args):
...     print args #1
... 
>>> one()
()
>>> one(1,2,3)
(1, 2, 3)
>>> def two(x,y,*args): #2
...     print x,y,args
... 
>>> two('a','b','c')
a b ('c',)
```

At point #1, we simply refer to the variable `args` inside the function - `*args` is only used in the function definition to indicate that positional arguments should be stored in the variable `args`. Python also allows us to specify some variables and catch any additional parameters in `args` as we can see at point #2.

The * operator can also be used when calling functions and here it means the analogous thing. A variable prefaced by * when calling a function means that the variable contents should be extracted and used as positional arguments.

```
>>> def add(x, y):
...     return x + y
>>> lst = [1,2]
>>> add(lst[0], lst[1]) # 1
3
>>> add(*lst) # 2
3
```

The code at point #1 does exactly the same thing as the code at point #2 - Python is doing automatically for us at point #2 what we could do manually for ourselves. `*args` means either extract positional variables from an iterable if calling a function or when defining a function accept any extra positional variables.

Things get only slightly more complicated when we introduce ** which does for dictionaries & key/value paris exactly what * does for iterables and positional parameters.

```
>>> def foo(**kwargs):
...     print kwargs
... 
>>> foo()
{}
>>> foo(x=1,y=2)
{'y': 2, 'x': 1}
```

When we define a function we can use `**kwargs` to indicate that all uncaptured keyword arguments should be stored in a dictionary called `kwargs`. As before neither the name `args` nor `kwargs` is part of Python syntax but it is convention to use these variable names when declaring functions. Just like * we can use ** when calling a function as well as when defining it.

```
>>> dct = {'x':1, 'y':2}
>>> def bar(x,y):
...     return x + y
... 
>>> bar(**dct)
3
```

#### 12.More generic decorators

Given our new power we can write a decorator that "log" the arguements to functions.

```
>>> def logger(func):
...     def inner(*args, **kwargs): #1
...         print "Arguments were: %s, %s" % (args, kwargs)
...         return func(*args, **kwargs) #2
...     return inner
```

Notice `inner` function takes any arbitrary number and type of parameters at point #1 and passes them along as arguments to the wrapped function at point #2. This allows us to wrap or decorate any function, no matter it's signature.

```
>>> @logger
... def foo1(x, y=1):
...     return x * y
>>> @logger
... def foo2():
...     return 2
>>> foo1(5, 4)
Arguments were: (5, 4), {}
20
>>> foo1(1)
Arguments were: (1,), {}
1
>>> foo2()
Arguments were: (), {}
2
```

You might also consider a little further study: [Bruce Eckel has an excellent essay on decorators](http://www.artima.com/weblogs/viewpost.jsp?thread=240808)


### Reference

* [Understanding Python Decorators in 12 Easy Steps!](http://simeonfranklin.com/blog/2012/jul/1/python-decorators-in-12-steps/)

