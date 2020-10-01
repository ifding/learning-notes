
## Chapter 2 An array of sequences

> Sep 8. 2020


### Overview of built-in sequences

The standard library offers a rich selection of sequence types implemented in C:

Container sequences

`list`, `tuple` and `collections.deque` can hold items of different types.

Flat sequences

`str`, `bytes`, `bytearray`, `memoryview` and `array.array` hold items of one type.

Container sequences hold references to the objects they contain, which may be of any type, while flat sequences physically store the value of each item within its own memory space, and not as distinct objects.

Another way of grouping sequence types is by mutability:

Mutable sequences

`list`, `bytearray`, `array.array`, `collections.deque` and `memoryview`

Immutable sequences

`tuple`, `str` and `bytes`


> In Python code, line breaks are ignored inside pairs of [], {} or (). So you can build multi-line lists, listcomps, genexps, dictionaries etc. without using the ugly \ line continuation escape.


### Tuples are not just immutable lists

Tuples do double-duty: they can be used as immutable lists and also as records with no field names.

Tuples work well as records because of the tuple unpacking mechanism.

The most visible form of tuple unpacking is _parallel assignment_, assigning items from an iterable to a tuple of variables.

```python
lax_coordinates = (33.9425, -118.408056)
latitude, longitude = lax_coordinates # tuple unpacking
```

An elegant application of tuple unpacking is swapping the values of variables without using a temporary variable:

```python
b, a = a, b
```

> Using * to grab excess items

Defining function parameters with `*args` to grab arbitrary excess arguments is a classic Python feature.

```python
>>> a, b, *rest = range(5)
>>> a, b, rest
(0, 1, [2, 3, 4])
```

### Slicing

A common feature of list, tuple, str and all sequence types in Python is the support of slicing operations.

Why slices and range exclude the last item?

- It's easy to see the length of a slice or range: range(3) and my_list[:3] both produce three items.
- It's easy to split a squence in two parts at any index x, without overlapping: my_list[:x] and my_list[x:].

> s[a:b:c], seq[start:stop:step]

```python
>>> s = 'bicycle' >>> s[::3]
'bye'
>>> s[::-1] 
'elcycib'
>>> s[::-2] 
'eccb'
```

