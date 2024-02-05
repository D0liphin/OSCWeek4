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