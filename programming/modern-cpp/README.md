
---
---

# Awesome Modern C++ Notes


### Best Practices

> From [Awesome Modern C++](https://github.com/rigtorp/awesome-modern-cpp)

- Consider the [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines).
- Never manually format code, use [clang-format](http://clang.llvm.org/docs/ClangFormat.html).
- Build your code using [CMake](https://cmake.org/).
- Test your code using [Catch](https://github.com/catchorg/Catch2).
- Also test using sanitized builds: [AddressSanitizer, ThreadSanitizer and MemorySanitizer](https://github.com/google/sanitizers).
- Use a fuzzer to test untrusted input: [afl](http://lcamtuf.coredump.cx/afl/), [libFuzzer](http://llvm.org/docs/LibFuzzer.html), [KLEE](http://klee.github.io/).


### C++ Crash Course 

- [Modern C++ Tutorial: C++11/14/17/20 On the Fly](https://github.com/changkun/modern-cpp-tutorial)
- [180+ Algorithm & Data Structure Problems using C++](https://github.com/mandliya/algorithms_and_data_structures)
- [Gibs Jose's C++ Cheat Sheet](https://github.com/gibsjose/cpp-cheat-sheet)
- [Crash Course by Nicolas P Rougie](https://www.labri.fr/perso/nrougier/teaching/c++-crash-course/index.html)
- [Learn C++ From Programiz](https://www.programiz.com/cpp-programming/return-reference)
- [C++ Notes by Fredosaurus](http://www.fredosaurus.com/notes-cpp/index.html), [Expanding an Array](http://www.fredosaurus.com/notes-cpp/newdelete/55dynexample.html)
- [The C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines)
- [A curated list of awesome C++ frameworks, libraries and software](https://github.com/pfultz2/awesome-cpp-1)
- [slides, discussion and exercices for the c++ co-learning in zurich](https://github.com/opentechschool-zurich/cpp-co-learning)
- [SweetLoverFT/CPlusPlus](https://github.com/SweetLoverFT/CPlusPlus)
- [Repository for those who learn C++14, STL and Boost](https://github.com/Evgeny-Bukovski/ForStudentsLearningCpp)


### Blogs

* [Andrzej's C++ blog](https://akrzemi1.wordpress.com/) - Guidelines and thoughts about C++.
* [C++ Hints](http://cpphints.com/) - Common mistakes and their solutions.
* [Embedded in Academia](http://blog.regehr.org/) - John Regehr, Professor of Computer Science, University of Utah, USA.
* [Eric Niebler](http://ericniebler.com/)
* [Guru of the Week (new posts)](http://herbsutter.com/gotw/) - A regular series of C++ programming problems created and written by Herb Sutter.
* [Guru of the Week (older posts)](http://www.gotw.ca/gotw/) - Older Guru of the Week posts.
* [Marshall's C++ Musings](https://cplusplusmusings.wordpress.com/)
* [Paul Fultz II's Blog](http://pfultz2.com/blog/)
* [Preshing on Programming](http://preshing.com/)
* [Simplify C++](http://arne-mertz.de/) - Write clean and maintainable C++.
* [Sticky Bits](https://blog.feabhas.com/) - A blog looking at developing software for real-time and embedded systems.
* [Sutter's Mill](http://herbsutter.com/) - Herb Sutter on software development.
* [The View from Aristeia](http://scottmeyers.blogspot.com/) -Scott Meyers' Professional Activities and Interests.
* [Visual C++ Team Blog](https://blogs.msdn.microsoft.com/vcblog/)
* [Why is a raven like a writing desk?](http://www.elbeno.com/blog/)
* [Anthony Williams' blog](https://www.justsoftwaresolutions.co.uk/blog/)
* [Fluent C++](https://www.fluentcpp.com/) - Expressive Code in C++.


### Libraries

> C++ Standard Library - including STL Containers, STL Aglorithm, STL Functional, etc.*

* [C++ Standard Library](https://en.wikipedia.org/wiki/C%2B%2B_Standard_Library) - A collection of classes and functions, which are written in the core language and part of the C++ ISO Standard itself.
* [Standard Template Library](https://en.wikipedia.org/wiki/Standard_Template_Library) - The Standard Template Library (STL).
* [C POSIX library](https://en.wikipedia.org/wiki/C_POSIX_library) - A specification of a C standard library for POSIX systems.
* [ISO C++ Standards Committee](https://github.com/cplusplus) - ISO/IEC JTC1/SC22/WG21 - The C++ Standards Committee. [website](http://www.open-std.org/JTC1/SC22/WG21/)
* [The GNU C Library](https://www.gnu.org/software/libc/manual) - The purpose of this manual is to tell you how to use the facilities of the GNU C Library.


> Artificial Intelligence

* [ANNetGPGPU](https://github.com/ANNetGPGPU/ANNetGPGPU) - A GPU (CUDA) based Artificial Neural Network library. [LGPL]
* [btsk](https://github.com/aigamedev/btsk) - Game Behavior Tree Starter Kit. [zlib]
* [Evolving Objects](http://eodev.sourceforge.net/) - A template-based, ANSI-C++ evolutionary computation library which helps you to write your own stochastic optimization algorithms insanely fast. [LGPL]
* [frugally-deep](https://github.com/Dobiasd/frugally-deep) - Header-only library for using Keras models in C++. [MIT]
* [Genann](https://github.com/codeplea/genann) - Simple neural network library in C. [zlib]
* [MXNet](https://github.com/apache/incubator-mxnet) - Lightweight, Portable, Flexible Distributed/Mobile Deep Learning with Dynamic, Mutation-aware Dataflow Dep Scheduler; for Python, R, Julia, Scala, Go, Javascript and more [website](https://mxnet.apache.org)
* [PyTorch](https://github.com/pytorch/pytorch) - Tensors and Dynamic neural networks in Python with strong GPU acceleration. [website](https://pytorch.org)
* [Recast/Detour](https://github.com/recastnavigation/recastnavigation) - (3D) Navigation mesh generator and pathfinder, mostly for games. [zlib]
* [TensorFlow](https://github.com/tensorflow/tensorflow) - An open source software library for numerical computation using data flow graphs [Apache]
* [CNTK](https://github.com/Microsoft/CNTK) - Microsoft Cognitive Toolkit (CNTK), an open source deep-learning toolkit. [Boost]
* [tiny-dnn](https://github.com/tiny-dnn/tiny-dnn) - A header only, dependency-free deep learning framework in C++11. [BSD]
* [Veles](https://github.com/Samsung/veles) - Distributed platform for rapid Deep learning application development. [Apache]
* [Kaldi](https://github.com/kaldi-asr/kaldi) - Toolkit for speech recognition. [Apache]
* [pytorch-cpp](https://github.com/warmspringwinds/pytorch-cpp)
* [An Introduction To Torch (Pytorch) C++ Front-End](https://radicalrafi.github.io/posts/pytorch-cpp-intro/)
* [A toolkit for making real world machine learning and data analysis applications in C++](https://github.com/davisking/dlib)


### Tools

> From [Awesome Modern C++](https://github.com/rigtorp/awesome-modern-cpp)

* [American fuzzy lop](http://lcamtuf.coredump.cx/afl/) - American fuzzy lop is a security-oriented fuzzer.
* [Buckaroo](https://github.com/LoopPerfect/buckaroo) - Fully Decentralized Polyglot Package Manager for C++ and Friends
* [cget](https://github.com/pfultz2/cget) - CMake package retrieval.
* [clang-format](http://clang.llvm.org/docs/ClangFormat.html) - A tool to format C++ code.
* [clang-tidy](http://clang.llvm.org/extra/clang-tidy/) - A clang-based C++ "linter" and static analysis tool.
* [CMake](https://cmake.org/) - Cross-platform family of tools designed to build, test and package software.
* [Compiler Explorer](https://gcc.godbolt.org/) - Interactively explore the assembly output of your C++ code.
* [conan](https://www.conan.io/) - C/C++ package manager.
* [cppcheck](http://cppcheck.sourceforge.net/) - Static analysis of C/C++ code.
* [C++ Archive Network](https://cppan.org/) - CPPAN - Cross-platform C/C++ package manager.
* [CPM](https://github.com/TheLartians/CPM) - A CMake script for setup-free cross-plattform dependency management.
* [Hunter](https://github.com/ruslo/hunter) - Cross-platform package manager for C++.
* [irony-mode](https://github.com/Sarcasm/irony-mode) -  A C/C++ minor mode for Emacs powered by libclang.
* [modern-cpp-font-lock-mode](https://github.com/ludwigpacifici/modern-cpp-font-lock) - Syntax highlighting support for Modern C++ with emacs
* [vcpkg](https://github.com/microsoft/vcpkg) - C++ Library Manager for Windows, Linux, and MacOS


### Websites

> From [Awesome Modern C++](https://github.com/rigtorp/awesome-modern-cpp)

* [C++ Questions Subreddit](https://www.reddit.com/r/cpp_questions) - A great place to get help.
* [C++ Patterns](https://cpppatterns.com/) - A repository of modern C++ patterns.
* [C++ Subreddit](https://www.reddit.com/r/cpp) - Discussions, articles, and news about the C++ programming language.
* [C++ Super-FAQ](https://isocpp.org/faq) - The C++ Super-FAQ.
* [C++ reference](http://en.cppreference.com/w/) - C++ reference.
* [C++11 FAQ](http://www.stroustrup.com/C++11FAQ.html)
* [CppCast](http://www.cppcast.com) - The only podcast for C++ developers by C++ developers.
* [Stackoverflow C++11](http://stackoverflow.com/questions/tagged/c%2b%2b11)
* [Stackoverflow C++14](http://stackoverflow.com/questions/tagged/c%2b%2b14)
* [Stackoverflow C++](http://stackoverflow.com/questions/tagged/c%2b%2b)
* [The C++ Programming Language](https://isocpp.org/) - News, Status & Discussion about Standard C++.