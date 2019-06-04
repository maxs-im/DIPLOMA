# DIPLOMA. Bachelor's degree.
Algorithm for solving systems of equations in Boolean algebra

## How to use this program?
Program is used to solve systems of (non-)linear (in-)homogeneous equations
For linear we'll use TSS method. Otherwise, Quine's method.

## Helper
For more info your could use such flags:
- __-h__ - to see this info;
- __-t__ - to detect the execution time of the main functions;
- __-l__ - something like logs (errors, parsed variables etc.);
- __-f/-o__ [path] - input/output file directories accordingly;
- __-rt__ [v:e] - start time testing. v,e - numbers of variables & equations (# - means useless ignoring test case);
- __-s__ [num] - helper for testing. Get number of step. If num > 0 -> add, if num < 0 - multiply each step;

## How to write correct input?
You should to know some rules:
1. Each line == equation. But empty line -> means stop for parsing;
2. There are two ways to write an equation:
+ In common boolean form.  
    Example: *'x1 x2 x3 10110010'*
    Read from left to right -> first __'1'__ -> means x = (x1 x2 x3) = (0, 0, 0), f(x) = 1;
    *Note:* n - variables means we should have table with 2^n elements in !one! string (only _'1'_, _'0'_ symbols);
    Also, do not repeat variables like *'x1 x1 0100'*.
+ In Zhegalkin form   
    Example: *'x1 x2 + x3 = 1'*
    __'+'__ - means adding modulo two, __' '__ between variables - means conjunction;
    Write each symbol with space!. You have two special symbols __'+'__ and __'='__;
    *Note:* Symbol __'='__ must be at the end. After it you can set either __'1'__ or __'0'__ (by default is nothing).

## Author & Link

- **Maxim Galchenko** (@maxs-im)

[Github Repository](https://github.com/maxs-im/DIPLOMA)
