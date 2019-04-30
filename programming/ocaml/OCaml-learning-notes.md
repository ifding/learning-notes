
OCaml learning notes
=========
>11-30-2016

#### 1. CAML syntax and Pragmatics

* Typing ocaml at the command prompt. Input is case-sensitive.
* Comments begin with (*  and end with *).
* All variable name must begin with a lowercase letter. Names beginning with a capital letter are reserved for constructors for user-defined data structures.
* For multi-argument functions, both tuples and currying ar supported.
* CAML allows the use of if-then constructs as well as pattern matchers for conditional statements.
* Integer and floating-point operations are distinguished by separate symbols.
* The interactive prompt is the # character, and a pair of semicolons is the expression terminator.
* CAML phrases are either simple expressions or let definitions of identifiers that may be either values or functions.
* CAML will try to infer the type from usage in the function body.
* Recursive functions must be defined with the let rec binding.
* Typing a function name without arguments at the command line causes ocaml to return the function signature.

#### 2. The Interactive (Top-Level) CAML System

* The pervasives library contains the functions float_of_int, used to convert an int to a float,  int_of_float used for float to int conversion, string_of_int for int to string conversion as well as int_of_string for string to int conversion.

#### 3. Building a Custom Top-Level System
* ocamlmktop could be used to build a custom top-level system where this library was included. To build this custom top-level ocaml system:

```
ocamlmktop  -o myocaml unix.cma
```

* This top-level system is started via the command ./mytop (as opposed to ocaml)

#### 4. ocaml Top-Level Directives

	* cd:  Change the current working directory.
	* load:  Load a bytecode object file.
	* use: Read, compile, and execute CAML source statements from a file.
	* trace:  Trace a designated function ( #untrace stops tracing).

#### 5. CAML Programming Environment Structure

* Data, data structures, and associated functions are bundled into modules. Libraries contain one or more modules.
* The ocaml core library provides a number of elementary data types, including the types int, float, bool, list, and string, as well as predefined exceptions, also including the pervasives module, including min, max, not, or, +, sqrt, log, and so on.

#### 6. CAML Extensions

* A CAML-based lexer and parser generators ( appropriately named ocamllex and ocamlyacc ).
* A built-in documentation generator (ocamldoc), which makes special use of comments delineated by (** and **).
* A language module browser ocamlbrowser, which provides a quick, on-line reference to the modules and libraries as well as an IDE.
* OO programming extensions (via ocaml).

#### 7. Defining Functions in CAML
* Pattern-Match version

```
	function pattern_1  -> expr_1
		| …
		| pattern_n  -> expr_n
	# (function 1 -> “one” | 2 -> “two”  | 3 -> “three”) ;;
	Warning P:
: int -> string = <fun>
```

* Using fun

```
	fun parameter_1  … parameter_n  -> expr
	# (fun  a  b  c  d  ->  a+b+c+d);;
- : int -> int -> int -> int -> int = <fun>
```

* Naming Functions
The let keyword is used to associate a variable (name) with a function.

```
# let afun = fun  x -> x * x;;
val afun : int -> int = <fun>
```

Function Definitions and let

```
let identifier = fun  parameter_1  … parameter_m  ->  expr
```

Is equivalent to the simpler form

```
let identifier parameter_1  … parameter_m = expr
```

* Currying Versus Tuples

```
# let calc input1 input2  = List.hd input1 * List.hd input2;;
val calc :  int list ->  int list  -> int list  -> int = <fun>

# let calc2 (input1, input2)  = List.hd input1 * List.hd input2;;
val calc2 : int list * int list  ->  int = <fun>
```

* Polymorphic Function Behavior

A user-defined function will be polymorphic if the body only  contains polymorphic functions and operators.

#### 8. An Introduction to Lists

```
# let calc input = List.hd(input);;
val cal : 'a list -> 'a = <fun>

# calc([1;2;3]);;
- : int = 1

# List.hd [1;2;3];;
- : int = 1

# 1 :: [2;3;4];;
- : int list = [1;2;3;4]

# List.append [1;2;3] [4;5;6];;
- : int list = [1; 2; 3; 4; 5; 6]

# [1;2;3]@[4;5;6];;
- : int list = [1; 2; 3; 4; 5; 6]

# List.length [1; 2; 3; 4; 5; 6];;
- : int = 6

# List.nth [1; 2; 3; 4; 5; 6] 3;;
- : int = 4

# let alist = [3;6;2;1;5;4];;
val alist : int list = [3; 6; 2; 1; 5; 4]

# List.nth alist 0;;
- : int = 3

# nth  alist 0;;
Ubound value nth

# open List;;

# nth alist 0;;
- :  int  = 3

# List.sort  compare alist;;
-: int list = [1;  2;  3; 4;  5;  6]

# List.nth  alist 0;;
-:  int  =  3
 
# let alistsorted = List.sort compare alist;;
# let maxalist  = List.nth  alistsorted ((List.length alist) - 1);;
```

Another example:

```
(* list reversal function in ocaml *)
(* file: rev.caml *)

let rec rev = fun alist ->
if ((List.tl aList) == []) then aList
 else (rev (List.tl aList)) @ [List.hd aList];;
 ```

 ```
 # #use "rev.caml";;
 val rev : 'a list -> 'a list = <fun>
 # rev [1; 2; 3];;
 - : int list = [3; 2; 1]
 # rev ["three"; "two"; "one"];;
 - : string list = ["one"; "two"; "three"]
 ```

 #### 9. Example: Ways to find the minimum element of a list

 ```
 let rec listMin alist =
   match alist with
   [] -> failwith "listMin should not be used on an empty list"
   | [x] -> x
   | x :: t -> min x (listMin t);;
  ```

```
  let rec listMinP = function
    [] -> failwith "listMinP should not be used on an empty list"
    | [x] -> x
    | x::t -> min x (listMinP t);;
```

```
let rec listMinrev2 = function x ->
if x == [] then
    failwith "listMinrev2 should not be used on an empty list"
        else
            if List.tl(x) == [] then List.hd(x)
            else min (List.hd x) (listMinrev2 (List.tl x));;
```

```
let my_max = function x,y ->
if x < y then y else x ;;
```

#### 10. CAML I/O

* Input and output are specified by input and output channels in_channel and out_channel, the defaults are stdin and stdout.
*Input in the Pervasives module include read_line, read_int, and read_float
* Output in the Pervasives module and include print_string with signature string -> unit. This function prints an argument string on the standard output and returns type unit. Similarly, function print_int, print_float,  and print_newline are also provided.
* The Printf module  contains a number of printing functions intended for formatted printing, including function printf. A formatted string (called a format) is used.

```
# Printf.fprintf stdout “Hi Bob\n”;;
Hi Bob
-: unit = ()

# let my_chan = open_out "camlTest.out";;
val my_chan : out_channel = <abstr>

# Printf.fprintf my_chan "Hi Bob\n";;
- : unit = ()

# flush my_chan;;
- : unit = ()

# Printf.fprintf my_chan "\n %d    %d  %s\n" 10 20 "done";;
- : unit = ()

# flush my_chan;;
- : unit = ()

# let my_in = open_in "camlTest.in";;
try
  while true do
    print_string (input_line my_in ^ "\n")
  done
with
| End_of_file -> ()
```

#### 11. Recursion, Pattern Matching, Lists, and trace

* CAML trace

```
# trace <function>
let rec member = function
    (x, []) -> false
  | (x, h::t) ->
      if (h = x)
          then true
          else member (x,t);;
```

#### 12. Overview of Objects in CAML

```
(* vehicle.caml *)

class vehicle =
object
val mutable name = "batmobile"
method print_name = name
end;;

class boat = 
object
val mutable name = "leaky"
val mutable capacity = 4
method how_big = capacity
end;;
```

```
# #use “vehicle.caml”;;

# let my_vehicle = new vehicle;;
val my_vehicle : vehicle = <obj>

# my_vehicle#print_name;;
-: string = “batmobile”	

# let my_boat = new boat;;
-: my_boat : boat = <obj>

# my_boat#how_big;;	
-: int = 4
```

```
(* vehicle2.caml employs inheritence *)

class vehicle =
object
val mutable name = "batmobile"
method print_name = name
end;;

class boat = 
object
inherit vehicle
val mutable capacity = 4
method how_big = capacity
end;;
```	

```
# #use “vehicle2.caml”;;

# let my_vehicle = new vehicle;;
val my_vehicle : vehicle = <obj>

# let my_boat = new boat;;
-: my_boat : boat = <obj>

# my_vehicle#print_name;;
-: string = “batmobile”

# my_boat#print_name;;
-: string = “batmobile”
```

#### 13. Scanning and Parsing in CAML

* In a manner similar to flex and bison, CAML provides ocamllex and ocamlyacc
* Lexer input using one of a family of functions from the Lexing module. This includes Lexing.from_string and Lexing.from_channel.
* ocamllex input file for valid license plates, the input file license1.mll

```
rule license =
parse ['a'-'z']['a'-'z']['a'-'z']['1'-'9']['0'-'9']['0'-'9']
    {print_string "valid license plate"}
```

* CAML lexing or scanning function, license, is created in file license1.ml

```
	$ ocamllex license1.mll	
	# #use “license1.ml”;;
	# let lexbuf = Lexing.from_string “abc123”;;
	# license lexbuf;;
	valid license plate-: unit = ()
```

* Obtaining Scanner Input from stdin

```
	# let lexbuf = Lexing.from_channel stdin;;
	# license lexbuf;;
	abc123
	valid license plate-: unit = ()
```

#### 14. CAML Compilation with ocamlc

* ocamlc compiler .ocaml produces CAML bytecode suitable for use with ocamlrun
Example

```
	(* file: randex.ml *)
	print_newline();;
	print_string(“ A random number between 0 and 51”);;
	print_newline();;
	print_string((string_of_int (Random.int52 )));;
	print_newline();;

	$ ocamlc -o randex randex.ml
	$ ocamlrun randex
OR   ./randex
```

#### 15. Compiling with Libraries

* To compile a program that uses functions in modules, for examle, using module Unix, which is contained in the unix library.

```
	$ ocamlc -o randex  unix.cma  randex.ml
```

#### 16. ocaml Compilation Extensions

* If the -custom option is used, ocamlc produces a stand-alone executable. This file can then be executed directly or on another machine ( using the same CPU/ OS)
* The Debugger (ocamldebug)
	If the source files are compiled with -g switch, debug information is added to bytecode

```
	$ ocamldebug randex
```

#### Reference

* [Schalkoff, Programming Languages and Methodologies, Jones and Bartlett Publishers, 2007.](https://www.amazon.com/Programming-Languages-Methodologies-Robert-Schalkoff/dp/0763740594)
