
# C and CPP

In general, computer languages deal with two concepts: data and algorithms. The *data* constitutes the information a program uses and processes. The *algorithms* are the methods the program uses.

```
         +-------+         +--------------------+
         | Data  |         |    Algorithms      | 
         +-------+         +--------------------+
  1/2 cup|butter |         |cream butter        |
         +-------+         +--------------------+
    1 cup|sugar  |         |gradually, add sugar|
         +-------+         +--------------------+
        2|eggs   |         |break eggs          | 
         +-------+         +--------------------+
           ...                ...
                      ||
                      ||
                     \||/
                      \/
                   Program
```

## C
- c is a *procedural* language. That means it emphasizes the algorithm side of programming.
- Conceptually, procedural programming consists of figuring out the actions a computer should take and then using the programming language to implement those actions.
- *Top-down* design: with c, the idea is to break a large program into smaller, more manageable tasks.
- If one of these tasks is still too broad, you divide it into yet smaller tasks.
- You continue with this process until the program is compartmentalized into small, easily programmed modules.
- C's design facilitates this approach, encouraging you to develop program units called *functions* to represent individual task modules.
- The C language incorporated features such as control structures and functions to better control the flow of a program and to enable a more structured, modular approach.

## C++
- C++ adds object-oriented concepts to the C language.
- Unlike procedural programming, which emphasizes algorithms, OOP emphasizes the data.
- Rather than try to fit a problem to the procedural approach of a language, OOP attempts to fit the language to the problem.
- The idea is to design data forms that correspond to the essential features of a problem.
- In C++, a *class* is a specification describing such a new data form, and an *object* is a particular data structure constructed according to that plan.
- The process of going from a lower level of organization, such as classes, to a higher level, such as program design, is called *bottom-up* programming.
- OOP facilitates creating reusable code, and that can eventually save a lot of work.
- Information hiding safeguards data from improper access.
- Polymorphism lets you create multiple definitions for operators and functions, with the programming context determining which definition is used.
- Inheritance lets you derive new classes from old ones.
- Instead of concentrating on tasks, you concentrate on representing concepts.
- Instead of taking a top-down programming approach, you sometimes take a bottom-up approach.
- A computer program translates a real-life problem into a series of actions to be taken by a computer.
- The OOP aspect of C++ gives the language the ability to relate to concepts involved in the problem, and the C part of C++ gives the language the ability to get close to the hardware.

## Programming steps

```
                   +-----------+
                   |source code|
                   +-----------+
                         |
                         v
                   +-----------+
                   |  COMPILER |
                   +-----------+
                         |
                         v
                   +-----------+
                   |object code|
                   +-----------+
                         |
 +------------+          |
 |startup code|          |
 +------------+          |
           |             v
           |-----> +-----------+
                   |   LINKER  |
           |-----> +-----------+
 +------------+          |
 |library code|          |
 +------------+          |
                         v
                   +---------------+
                   |executable code|
                   +---------------+
```

### Compilation and Linking

- Originally, Bjarne Stroustrup implemented C++ with a C++-to-C compiler program instead of developing a direct C++-to-object code compiler. 
- This program, called *cfront* (for C front end), translated C++ source code to C source code, which could then be compiled by a standard C compiler.
- As C++ has developed adn grown in popularity, more and more implementers have turned to creating C++ compliers that generate object code directly from C++ source code.
- This direct approach speeds up the compilation process and emphasizes that C++ is a separate, if similar, language.

## Reference

- C++ Primer Plus (6th Edition) (Developer's Library) 6th Edition by Stephen Prata

