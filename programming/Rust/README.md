# Rust

> Jan. 2022

### What Is Ownership?

Ownership is a set of rules that governs how a Rust program manages memory.

> The Stack and the Heap
> 1. Data with an unknown size at compile time or a size that might change must be stored on the heap instead.
> 2. allocating on the heap and return a pointer
> 3. the pointer to the heap is a known, fixed size, you can store the pointer on the stack.
> 4. the main purpose of ownership is to manage heap data


#### Ownership Rules

- Each value in Rust has a variable that’s called its owner.
- There can only be one owner at a time.
- When the owner goes out of scope, the value will be dropped.

> Rust will never automatically create “deep” copies of your data. 
> If a type implements the `Copy` trait, a variable is still valid after assignment to another variable.


#### References and Borrowing

- A reference is like a pointer in that it’s an address we can follow to access data stored at that address that is owned by some other variable. 
- Unlike a pointer, a reference is guaranteed to point to a valid value of a particular type.
- The opposite of referencing by using & is dereferencing, which is accomplished with the dereference operator, `*`.
- We call the action of creating a reference `borrowing`.
- A slice is a kind of reference, so it does not have ownership.

> The Rules of References

- At any given time, you can have either one mutable reference or any number of immutable references.
- References must always be valid.



### Generic Types, Traits, and Lifetimes

#### Traits: Defining Shared Behavior

- A `trait` tells the Rust compiler about functionality a particular type has and can share with other types. 
- We can use `trait` bounds to specify that a generic type can be any type that has certain behavior.
- Traits are similar to a feature often called interfaces in other languages, although with some differences.

One restriction to note with trait implementations is that we can implement a trait on a type only if **at least** one of the trait or the type is local to our crate. 

This restriction is part of a property of programs called _coherence_, and more specifically the _orphan rule_, so named because the parent type is not present. 

This rule ensures that other people’s code can’t break your code and vice versa. Without the rule, two crates could implement the same trait for the same type, and Rust wouldn’t know which implementation to use.

> Using the Newtype Pattern to Implement External Traits on External Types

There is no runtime performance penalty for using this pattern, and the wrapper type is elided at compile time. _Newtype_ is a term that originates from the Haskell programming language. 

```rust
use std::fmt;

// Wrapper is a tuple struct and Vec<T> is the item at index 0 in the tuple
struct Wrapper(Vec<String>);

impl fmt::Display for Wrapper {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "[{}]", self.0.join(", "))
    }
}

fn main() {
    let w = Wrapper(vec![String::from("hello"), String::from("world")]);
    println!("w = {}", w);
}
```


#### Traits as Parameters

```rust
pub trait Summary {
    fn summarize(&self) -> String {
        String::from("(Read more...)")
    }
}
```

We can define a notify function that calls the `summarize` method on its `item` parameter, which is of some type that implements the `Summary` trait. To do this, we can use the `impl Trait` syntax, like this:

```rust
pub fn notify(item: &impl Summary) {
    println!("Breaking news! {}", item.summarize());
}
```

Instead of a concrete type for the `item` parameter, we specify the `impl` keyword and the trait name. This parameter accepts any type that implements the specified trait. 

> Trait Bound Syntax

The `impl Trait` syntax works for straightforward cases but is actually syntax sugar for a longer form, which is called a `trait bound`:

```rust
pub fn notify<T: Summary>(item: &T) {
    println!("Breaking news! {}", item.summarize());
}
```

The `impl Trait` syntax is convenient and makes for more concise code in simple cases. The `trait bound` syntax can express more complexity in other cases.

> Specifying Multiple Trait Bounds with the + Syntax

We can also specify more than one `trait bound`. Say we wanted `notify` to use `display` formatting on item as well as the `summarize` method:

```rust
pub fn notify(item: &(impl Summary + Display)) {
```

The `+` syntax is also valid with trait bounds on generic types:

```rust
pub fn notify<T: Summary + Display>(item: &T) {
```

> Clearer Trait Bounds with where Clauses

Using too many trait bounds makes the function signature hard to read. So instead of writing this:

```rust
fn some_function<T: Display + Clone, U: Clone + Debug>(t: &T, u: &U) -> i32 {
```

we can use a `where` clause, like this:

```rust
fn some_function<T, U>(t: &T, u: &U) -> i32
    where T: Display + Clone,
          U: Clone + Debug
{
```


#### Returning Types that Implement Traits

We can also use the `impl Trait` syntax in the return position to return a value of some type that implements a trait:

```rust
fn returns_summarizable() -> impl Summary {
    Tweet {
        username: String::from("horse_ebooks"),
        content: String::from(
            "of course, as you probably already know, people",
        ),
        reply: false,
        retweet: false,
    }
}
```

We specify that the `returns_summarizable` function returns some type that implements the `Summary` trait without naming the concrete type. In this case, `returns_summarizable` returns a `Tweet`, but the code calling this function doesn’t know that.

The ability to return a type that is only specified by the trait it implements is especially useful in the context of `closures` and `iterators`. The `impl Trait` syntax lets you concisely specify that a function returns some type that implements the `Iterator` trait without needing to write out a very long type.


#### Validating References with Lifetimes

Most of the time, lifetimes are implicit and inferred, just like most of the time, types are inferred. Rust requires us to annotate the relationships using generic lifetime parameters to ensure the actual references used at runtime will definitely be valid.

The main aim of lifetimes is to prevent dangling references.

> The Borrow Checker

The Rust compiler has a borrow checker that compares scopes to determine whether all borrows are valid. 

```rust
    {
        let r;                // ---------+-- 'a
                              //          |
        {                     //          |
            let x = 5;        // -+-- 'b  |
            r = &x;           //  |       |
        }                     // -+       |
                              //          |
        println!("r: {}", r); //          |
    }                         // ---------+
```

Note that we want the function to take string slices, which are references, because we don’t want the `longest` function to take ownership of its parameters.

We don’t know the concrete lifetimes of the references that will be passed in, so we can't look at the scopes as we determine whether the reference we return will always be valid. 

```rust
fn main() {
    let string1 = String::from("abcd");
    let string2 = "xyz";

    let result = longest(string1.as_str(), string2);
    println!("The longest string is {}", result);
}

fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
```

Lifetime annotations describe the relationships of the lifetimes of multiple references to each other without affecting the lifetimes.

Having function signatures contain the lifetime contract means the analysis the Rust compiler does can be simpler.

> Thinking in Terms of Lifetimes

When returning a reference from a function, the lifetime parameter for the return type needs to match the lifetime parameter for one of the parameters. 

If the reference returned does not refer to one of the parameters, it must refer to a value created within this function, which would be a dangling reference because the value will go out of scope at the end of the function.

Ultimately, lifetime syntax is about connecting the lifetimes of various parameters and return values of functions.


#### Generic Type Parameters, Trait Bounds, and Lifetimes Together

It has an extra parameter named `ann` of the generic type `T`, which can be filled in by any type that implements the `Display` trait as specified by the `where` clause. This extra parameter will be printed using `{}`, which is why the `Display` trait bound is necessary. Because lifetimes are a type of generic, the declarations of the lifetime parameter `'a` and the generic type parameter `T` go in the same list inside the angle brackets after the function name.

```rust
fn main() {
    let string1 = String::from("abcd");
    let string2 = "xyz";

    let result = longest_with_an_announcement(
        string1.as_str(),
        string2,
        "Today is someone's birthday!",
    );
    println!("The longest string is {}", result);
}

use std::fmt::Display;

fn longest_with_an_announcement<'a, T>(
    x: &'a str,
    y: &'a str,
    ann: T,
) -> &'a str
where
    T: Display,
{
    println!("Announcement! {}", ann);
    if x.len() > y.len() {
        x
    } else {
        y
    }
}
```

> An I/O project: building a command line program: [minigrep](./minigrep.rs)


### Functional Programming: Iterators and Closures

#### Closures

Closures: anonymous functions that can capture their environment and access variables from the scope in which they're defined. 

When a closure captures a value from its environment, it uses memory to store the values for use in the closure body. 

Closures can capture values from their environment in three ways: taking ownership, borrowing mutably, and borrowing immutably. These are encoded in the three `Fn` traits:

- `FnOnce`: take ownership of these variables and move them into the closure, `Once` means it can't take ownship of the same variables more than once, so it can be called only once.
- `FnMut`: can change the environment because it mutably borrows values.
- `Fn`: borrows values from the environment immutably.

If you want to force the closure to take ownership, you can use the `move` keyword. It's mostly useful when passing a closure to a new thread to move the data so it's owned by the new thread.

> Note: `move` closures may still implement `Fn` or `FnMut`, even though they capture variables by move. This is because the traits implemented by a closure type are determined by what the closure does with captured values, not how it captures them. The `move` keyword only specifies the latter.

#### Iterators

In Rust, iterators are _lazy_, meaning they have no effect until you call methods that consume the iterator to use it up.

> The Iterator Trait and the next Method

All iterators implement a trait named `Iterator` that is defined in the standard library.

```rust
pub trait Iterator {
    type Item;

    fn next(&mut self) -> Option<Self::Item>;

    // methods with default implementations elided
}
```

This code says implementing the `Iterator` trait requires that you also define an `Item` type, and this `Item` type is used in the return type of the `next` method. 

The `iter` method produces an iterator over immutable references. If we want to create an iterator that takes ownership and returns owned values, we can call `into_iter` instead of `iter`. Similarly, if we want to iterate over mutable references, we can call `iter_mut` instead of `iter`.

> Comparing Performance: Loops vs. Iterators

Iterators, although a high-level abstraction, get compiled down to roughly the same code as if you’d written the lower-level code yourself. Iterators are one of Rust’s `zero-cost abstractions`, by which we mean using the abstraction imposes no additional runtime overhead.

> In general, C++ implementations obey the zero-overhead principle: What you don’t use, you don’t pay for. And further: What you do use, you couldn’t hand code any better.

Closures and iterators are inspired by functional programming language. Their implementations are such that runtime performance is not affected. This is part of Rust’s goal (zero-cost abstractions).


### Smart Pointers

> The difference between references and smart pointers

1. Smart pointers have additional metadata and capabilities. One example is the `reference counting` smart pointer type. 
2. References are pointers that only borrow data; in contrast, in many cases, smart pointers own the data they point to.

The most common smart pointers in the standard library:

- `Box<T>` for allocating values on the heap
- `Rc<T>`, a reference counting type that enables multiple ownership
- `Ref<T>` and `RefMut<T>`, accessed through `RefCell<T>`, a type that enforces the borrowing rules at runtime instead of compile time


#### Treating Smart Pointers Like Regular References with the Deref Trait

> Implicit Deref Coercions with Functions and Methods

`Deref` coercion works only on types that implement the `Deref` trait. `Deref` coercion automatically converts such a type into a reference to another type for either references or smart pointers. For example, deref coercion can convert `&String` to `&str` because `String` implements the `Deref` trait such that it returns `&str`.

Deref coercion was added so that function and method calls don’t need to add as many explicit references and dereferences with `&` and `*`.

> How Deref Coercion Interacts with Mutability

You can use the `DerefMut` trait to override the `*` operator on mutable references. Rust does deref coercion when it finds types and trait implementations in three cases:

- From `&T` to `&U` when `T: Deref<Target=U>`
- From `&mut T` to `&mut U` when `T: DerefMut<Target=U>`
- From `&mut T` to `&U` when `T: Deref<Target=U>`

The first case states that if you have a `&T`, and `T` implements Deref to some type `U`, you can get a `&U` transparently. The second case states that the same deref coercion happens for mutable references. The third case is trickier: Rust will also coerce a mutable reference to an immutable one.

> Running Code on Cleanup with the Drop Trait


#### Rc<T>, the Reference Counted Smart Pointer

The `Rc<T>` type keeps track of the number of references to a value to determine whether or not the value is still in use. If there are zero references to a value, the value can be cleaned up without any references becoming invalid. Note that `Rc<T>` is only for use in single-threaded scenarios. 

The implementation of `Rc::clone` doesn’t make a deep copy of all the data like most types’ implementations of `clone` do. The call to `Rc::clone` only increments the reference count, which doesn’t take much time.

Via immutable references, `Rc<T>` allows you to share data between multiple parts of your program for reading only.


#### RefCell<T> and the Interior Mutability Pattern

_Interior mutability_ is a design pattern in Rust that allows you to mutate data even when there are immutable references to that data, the pattern uses `unsafe` code inside a data structure.

With references and `Box<T>`, the borrowing rules’ invariants are enforced at compile time. With `RefCell<T>`, these invariants are enforced at _runtime_. Checking the borrowing rules at compile time is the best choice in the majority of cases, which is why this is Rust’s default.

Static analysis, like the Rust compiler, is inherently conservative. The advantage of checking the borrowing rules at runtime instead is that certain memory-safe scenarios are then allowed, whereas they are disallowed by the compile-time checks. 

The `RefCell<T>`type is useful when you’re sure your code follows the borrowing rules but the compiler is unable to understand and guarantee that.

Here is a recap of the reasons to choose `Box<T>`, `Rc<T>`, or `RefCell<T>`:

- `Rc<T>` enables multiple owners of the same data; `Box<T>` and `RefCell<T>` have single owners.
- `Box<T>` allows immutable or mutable borrows checked at compile time; `Rc<T>` allows only immutable borrows checked at compile time; `RefCell<T>` allows immutable or mutable borrows checked at runtime.
- Because `RefCell<T>` allows mutable borrows checked at runtime, you can mutate the value inside the `RefCell<T>` even when the `RefCell<T>` is immutable.


> Keeping Track of Borrows at Runtime with RefCell<T>

The `borrow` and `borrow_mut` methods are part of the safe API that belongs to `RefCell<T>`. The `borrow` method returns the smart pointer type `Ref<T>`, and `borrow_mut` returns the smart pointer type `RefMut<T>`. Both types implement `Deref`, so we can treat them like regular references.

The `RefCell<T>` keeps track of how many `Ref<T>` and `RefMut<T>` smart pointers are currently active. Every time we call `borrow`, the `RefCell<T>` increases its count of how many immutable borrows are active. When a `Ref<T>` value goes out of scope, the count of immutable borrows goes down by one. Just like the compile-time borrowing rules, `RefCell<T>` lets us have **many** immutable borrows or **one** mutable borrow at any point in time.

Catching borrowing errors at runtime rather than compile time means that you would find a mistake in your code later in the development process and possibly not until your code was deployed to production. Also, your code would incur a small runtime performance penalty as a result of keeping track of the borrows at runtime rather than compile time. 

A common way to use `RefCell<T>` is in combination with `Rc<T>`. Recall that `Rc<T>` lets you have multiple owners of some data, but it only gives immutable access to that data. If you have an `Rc<T>` that holds a `RefCell<T>`, you can get a value that can have multiple owners and that you can mutate!


### Object Oriented Programming Features of Rust

#### Using Trait Objects That Allow for Values of Different Types

The `trait` objects are more like objects in other languages in the sense that they combine data and behavior. But we can’t add data to a trait object. Trait objects aren’t as generally useful as objects in other languages: their specific purpose is to allow abstraction across common behavior, such as `interface` in Java.

The `trait` objects allow for multiple concrete types to fill in for the trait object at runtime. 

The advantage of using `trait` objects to write code similar to code using `duck typing` is that we never have to check whether a value implements a particular method at runtime. Rust won’t compile our code if the values don’t implement the traits that the trait objects need.

> Trait Objects Perform Dynamic Dispatch

- Static dispatch: the compiler generates nongeneric implementations of functions and methods for each concrete type that we use in place of a generic type parameter. The compiler knows what method you’re calling at compile time.
- Dynamic dispatch: when the compiler can’t tell at compile time which method you’re calling. At runtime, Rust uses the pointers inside the trait object to know which method to call. There is a runtime cost when this lookup happens that doesn’t occur with static dispatch. 

Trait objects must be object safe because once you’ve used a trait object, Rust no longer knows the concrete type that’s implementing that trait. A trait is object safe if all the methods defined in the trait have the following properties:

- The return type isn’t `Self`.
- There are no generic type parameters.


### Advanced Features

> Unsafe Rust

By requiring these five unsafe operations to be inside blocks annotated with unsafe you’ll know that any errors related to memory safety must be within an unsafe block. Keep unsafe blocks small. To isolate unsafe code as much as possible, it’s best to enclose unsafe code within a safe abstraction and provide a safe API.

- Dereference a raw pointer
- Call an unsafe function or method
- Access or modify a mutable static variable
- Implement an unsafe trait
- Access fields of `union`s

> The Never Type that Never Returns

```rust
fn bar() -> ! {
    // --snip--
}
```

```rust
        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };
```

`continue` has a `!` value. That is, when Rust computes the type of `guess`, it looks at both match arms, the former with a value of `u32` and the latter with a `!` value. Because `!` can never have a value, Rust decides that the type of guess is `u32`.


> Dynamically Sized Types and the Sized Trait

_dynamically sized types_: Sometimes referred to as DSTs or unsized types, these types let us write code using values whose size we can know only at runtime.

For example, `str` is a DST, we can’t know how long the string is until runtime, meaning we can’t create a variable of type `str`, nor can we take an argument of type `str`. `&T` is a single value that stores the memory address of where the `T` is located.

Rust has a particular trait called the `Sized` trait to determine whether or not a type’s size is known at compile time. This trait is automatically implemented for everything whose size is known at compile time.

```rust
fn generic<T>(t: T) {
    // --snip--
}
```

is actually treated as though we had written this:

```rust
fn generic<T: Sized>(t: T) {
    // --snip--
}
```

> Advanced Functions and Closures

The `fn` type is called a `function pointer` vs. `Fn` closure trait.

> Macros

macros are a way of writing code that writes other code, which is known as `metaprogramming`, such as `println!`, `vec!`. All of these macros expand to produce more code than the code you’ve written manually.

