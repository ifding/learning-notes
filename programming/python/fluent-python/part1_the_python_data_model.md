
## Chapter 1 The Python Data Model

> Aug 31. 2020


### Special methods

One of the best qualities of Python is its consistency. 

You can think of the Data Model as a description of Python as a framework. It formalizes the interfaces of the building blocks of the language itself, such as sequences, iterators, functions, classes, context managers and so on.

The Python interpreter invokes special methods to perform basic object operations, often triggered by special syntax. The special method names are always spelled with leading and trailing double underscores, i.e. `__getitem__`. For example, the syntax `obj[key]` is supported by the `__getitem__` special method. To evaluate it, the interpreter calls `obj.__getitem__(key)`.

By implementing the special methods `__len__` and `__getitem__`, `obj` behaves like a standard Python sequence, allowing it to benefit core language features -- like iteration and slicing -- and from the standard library, `random.choice`, `reversed` and `sorted`.

More often than not, the special method call is implicit. For example, the statement `for i in x`: actually causes the invocation of `iter(x)` which in turn may call `x.__iter__()` if that is available.

Normally, your code should not have many direct calls to special methods, unless metaprogramming or `__init__`. It's better to call the related built-in function, such as len, iter, str etc. These built-in call often provide other services for built-in types, are faster than method calls.  

The Python interpreter is the only frequent caller of most special methods.

Table: Special method names (operators excluded)

| Category                          | Method names                             |
| --------------------------------- |:----------------------------------------:|
| String/bytes representation       | __repr__, __str__, __format__, __bytes__ |
| Conversion to number              | __abs__, __bool__, __complex__, __int__, __float__, __hash__, __index__ |
| Emulating collections             | __len__, __getitem__, __setitem__, __delitem__, __contains__ |
| Iteration                         | __iter__, __reversed__, __next__ |
| Emulating callables               | __call__ |
| Context management                | __enter__, __exit__ |
| Instance creation and destruction | __new__, __init__, __del__ |
| Attribute management              | __getattr__, __getattribute__, __setattr__, __delattr__, __dir__ |
| Attribute descriptors             | __get__, __set__, __delete__ |
| Classs services                   | __prepare__, __instancecheck__, __subclasscheck__ |


### Why len is not a method

> the Zen of Python: practicality beats purity.

len(x) runs very fast when x is an instance of a built-in type. No method is called for the built-in objects of CPython: the length is simply read from a field in a C struct.

> For built-in types like list, str, bytearray etc., the interpreter takes a shortcut: the CPython implementation of len() actually returns the value of the `ob_size` field in the `PyVarObject` C struct that represents any variable-sized built-in object in memory. This is much faster than calling a method.

In other words, len is not called as a method because it gets special treatment as part of the Python Data Model, just like `abs`. But thanks to the special method `__len__` you can also make `len` work with your own custom objects. The Zen of Python: Special cases aren't special enough to break the rules.


### Summary

By implementing special methods, your objects can behave like the built-in types, enabling the expressive coding style the community considers Pythonic.

A basic requirement for a Python object is to provide usable string representations of itself, one used for debugging and logging, another for presentation to end users. That is why the special methods `__repr__` and `__str__` exist in the Data Model.
