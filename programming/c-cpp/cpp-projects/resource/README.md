
# C++20/17/14/11

> modern cpp features


## Debug

* <https://github.com/sharkdp/dbg-macro>
* <https://cppinsights.io/>



## Type traits in modern C++

> https://www.internalpointers.com/post/quick-primer-type-traits-modern-cpp

Type traits are a clever technique used in C++ template metaprogramming that gives you the ability to inspect and transform the properties of *types*.

For example, given a generic type `T` — it could be `int`, `bool`, `std::vector` or whatever you want — with type traits you can ask the compiler some questions: is it an integer? Is it a function? Is it a pointer? Or maybe a class? Does it have a destructor? Can you copy it? Will it throw exceptions? ... and so on. This is extremely useful in **conditional compilation**, where you instruct the compiler to pick the right path according to the type in input. We will see an example shortly.

Type traits can also apply some transformation to a type. For example, given `T`, you can add/remove the `const` specifier, the reference or the pointer, or yet turn it into a signed/unsigned type and many other crazy operations. Extremely handy when writing libraries that make use of templates.

The beauty of these techniques is that everything takes place at *compile time* with no runtime penalties: it's template metaprogramming, after all. 




## Reference
- <https://github.com/AnthonyCalandra/modern-cpp-features>