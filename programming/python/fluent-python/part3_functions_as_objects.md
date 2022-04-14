

## Part 3 Functions as Objects



A function that takes a function as argument or returns a function as the result is a `higher-order function`, such as, `map`, 	`sorted`.

> Packages for functional programming, like `operator` and `functools`


### Function Decorators and Closures

A decorator is a callable that takes another function as argument (the decorated function). In other words, assuming an existing decorator named decorate, this code:

```python
@decorate
def target():
	print('running target()')
```

Has the same effect as writing this:

```python
def target():
	print('running target()')

target = decorate(target)
```

The `target` name does not necessarily refer to the original `target` function, but to whatever function is returned by `decorate(target)`.

Decrators are just syntactic sugar, you can always simply call a decorator like any regular callable, passing another function. When doing `metaprogramming`, changing program behavior at runtime.

Two crucial facts: decorators have the power to replace the decorated function with a different one, they are executed immediately when a module is loaded.

A key feature of decorators is that they run right after the decorated function is defined. That is usually at `import time` (i.e., when a module is loaded by Python).

> Closures

A closure is a function with an extended scope that encompasses nonglobal variables referenced in the body of the function but not defined there.

```python
def make_averager():
	series = []

	def averager(new_value): 
		series.append(new_value) 
		total = sum(series) 
		return total/len(series)

	return averager
```

```python
>>> avg = make_averager()
>>> avg(10)
10.0
>>> avg(11)
10.5
>>> avg(12)
11.0
```

But where does the `avg` function in the second example find the `series`?

Note that `series` is a local variable of `make_averager` because the initialization `series = []` happens in the body of thagt function. But when `avg(10)` is called, `make_averager` has already returned, and its local scope is long gone.

Within `averager`, `series` is a `free variable`, it's not bound in the local scope.

A closure is a function that retains the bindings of the free variables that exist when the function is defined, so that they can be used later when the function is invoked and the defining scope is no longer available.

The `nonlocal` declaration lets you flag a variable as a free variable even when it is assigned a new value within the function.

Python has three built-in functions that are designed to decoreate methods: `property`, `classmethod`, and `staticmethod`.

