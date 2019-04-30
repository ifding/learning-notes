# Why Functional Programming?

## Paradigm evolution

- Mathematics (lambda calculus)
- Computer Science
- Spawn of languages and paradigms
  - functional, procedural, imperative, declarative, object-oriented programming (OOP)
- OOP overload ...

## OOP limitations
```
[W]e’re going to be living in a multicore, distributed, concurrent — all the buzz words — world. 
The conventional models we’ve been doing, the OO stuff… is not going to survive in that 
kind of environment.” - Dave Thomas
```
```
“OOP promised a cure for the scourge of software complexity. …its weaknesses have become 
increasingly apparent. Spreading state all over the place leads to concurrency issues 
and unpredictable side effects.” - Dave Thomas
```

## Think in Functionas not Objects

- **Functions**
  - Easy to reason about
  - reliable
  - pure
    - don't modify variables outside of scope
    - no side effects
    - deterministic (reproduciable results)
- **Data transformation**
  - i.e. Unix pipes - `cat foo.log | grep bar | wc -l`
- **No side-effects**
  - Side effects are:
    - modifying state
    - has observable interaction with external functions
- **Higer-order functions**
  - Functions can receive functions as arguments and return functions
- **Where is my *for* loop?**
  - recursion
  - map, reduct, filter, reject, take, etc.

## Some (impure and pure) functional languages

* LISP, Scheme, Clojure, Erlang, Scala, OCaml, Haskell, F#, Elm, Elixir

## Functional Concepts

- **Anonymous/First-class functions**
- **Pattern Matching**
  - Not an assignment operator but an assertion
  - `=` is a match operator
- **Multi-bodied functions**
- **Higher-order functions**
  - Functions that can take functions in as arguments or functions that return functions
- **Side-effects & State**
  - take data, copy it, alter copy, return it
  - always return the same output for any given input

  ```
  Doing the maths:
  (value2 * (value1 + value3)) + value1 * value2

  if value1 = 4, value2 = 2, value3 = 0
  then result should be 16

  (2 * (4 + 0)) + 4 * 2
  8 + 8
  16
  ```

- **Composition**
  - Combining simple functions to make more complicated ones
  - The result of one function is passed to the next; the result of the last function call is the result of the whole
  - Piping
  - Transforming data

- **Enumerables**
- **Partial function applications**
  - Elixir does not have built-in **currying** like Haskell or Scala

- **Recursion**
  - "Recursion in computer science is a method where the solution to a problem depends on solutions to smaller instances of the same problem (as opposed to iteration). The approach can be applied to many types of problems, and recursion is one of the central ideas of computer science." - https://en.wikipedia.org/wiki/Recursion_(computer_science)
  - Looping in traditional languages often times mutate and change data in sometimes unsuspecting ways
  - There is a lot of CS material out there that goes into depth on why this is true. Also you'll find recursion mentioned with terms such as Binary Trees and Binary Search Trees

- **Tail-call Optimization** 
    - If the very last thing a function does is call itself, there is no need to make the call. The runtime can jump back to the start of the function. The recursive call MUST be the last function executed. You may need to accumulate the results as you go.
    - Technique that allows the compiler to call a function without using any additional stack space

- **Concurrency**

- **Examples**
  - Parallel Map
  - OOP to Functional

## Reference

* [A Taste of Functional Programming](https://github.com/nonis3/functional-programming)
