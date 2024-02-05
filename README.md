# Project Structure

You will need Make for this*. Each folder, `Task1` and `Task2` is a 
project. Each has their own `Makefile` for building the projects.

The structure of each is as follows:

```plaintext
.
├── include
├── Makefile
├── src
│   └── main.c
├── target
│   ├── main
│   └── src
│       ├── main.c.d
│       └── main.c.o
└── tests
```

You only need to worry about `./src/main.c`.

Use `make build` to build the project and `make run` to execute the 
produced binary, which will be in `./target/main`.

For more options, use `make help`.

## Changing the Compiler 

You will probably need to change the compiler from `clang-17` to 
something that weird people (like you) use e.g. `gcc`. Open up the 
`Makefile` and you will see a box that says `CHANGE THESE`. Of course,
you don't need to 'CHANGE THESE', but you can if you like. Changing the
compiler for example requires a single line change of the value of `CC`
(which stands for "C Compiler").

```make
CC=gcc
```

You might also want to use some sanitizers... or something else. Just 
ask me or figure it out yourself (I'm sure you can do it!).

## Installing Make

If you're on Linux, you probably already have Make.

If you're on Windows, I don't know how to install Make. Looks like you 
have to install it manually [from here](https://gnuwin32.sourceforge.net/packages/make.htm).

If you're on some other esoteric operating system (like FreeBSD or 
MacOS) you ought to figure it out for yourself.

## What Kind of Style Is This?

It's the same style that linux uses (I use the same `.clang-format`). 
There are a few exceptions that make my life easier.

1. 8-spaces instead of 8-width tabs
2. Aligned comments
3. Max-width of 100 characters for code and inline comments
4. Max-width of 80 characters otherwise

1, 3 and 4 are generally accepted for modern programming languages. 
Though there is some contention about using 80-width comments instead of
72... Aligned comments I just like. I don't really have an excuse. 

```c
int something = 5; // Here is a long description that spans over 
                   // multiple comments. In the kernel style guide, this
                   // is not allowed!
int somethingelse = 6;
```