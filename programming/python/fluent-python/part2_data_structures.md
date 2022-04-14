
# Part II Data Structures

> Sep 8. 2020

## An Array of Sequences

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


Keeping in mind these common traits -- mutable versus immutable; container versus flat -- is helpful to extrapolate what you know about one sequence type to others.

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

Two-dimensional `numpy.ndarray` can be fetched using the syntx `a[i, j]` and a two-dimensional slice `a[m:n, k:l]`. To evaluate `a[i, j]`, Python calls `a.__getitem__((i, j))`.


### list.sort and the sorted Built-In Function

An important Python API convention: function or methods that change an object in place should return `None` to make it clear to the caller that the object itself was changed, and no new object was created. Such as, `list.sort`, `random.shuffle`.

In contrast, the built-in function `sorted` creates a new list and returns it.


## Dictionaries and Sets

Get the list for `key`, or set it to `[]` if not found; `setdefault` returns the value, so it can be updated without requiring a second search.

```python
my_dict = {}
my_dict.setdefault(key, []).append(new_value)
```

is the same as running

```python
if key not in my_dict: 
	my_dict[key] = []
my_dict[key].append(new_value)
```

Except that the latter code performs at least two searches for key - three if it's not found - while `setdefault` does it all with a single lookup.

> defaultdict: another take on missing keys

```python
import collections

my_dict = collections.defaultdict(list)
my_dict[key].append(new_value)
```

1. Create a `defaultdict` with the `list` constructor as `default_factory`.

2. If `key` is not initially in the `my_dict`, the `default_factory` is called to produce an empty list that is then assigned to `my_dict[key]` and returned.

The `default_factory` of a `defaultdict` is only invoked to provide default values for `__getitem__` calls, i.e., `my_dict[key]` operator, and not for the other methods.

> Immutable mappings

The `types` module provides a wrapper class called `MappingProxyType`, which, given a mapping, return a `mappingproxy` instance that is a read-only but dynamic view of the original mapping.


## Text versus Bytes

> Handling text files

The best practice for handling text is the "Unicode sandwich". 

- This means that bytes should be decoded to `str` as early as possible on input (e.g., when opening a file for reading). (`bytes -> str`)

- The business logic, where text handling is done exclusively on `str` objects. You should never be encoding or decoding in the middle of other processing. (`100% str`)

- On output, the `str` are encoded to bytes as late as possible. For example, in Django, your views should output Unicode `str`, Django itself takes care of encoding the response to bytes, using `UTF-8` by default. (`str -> bytes`)
