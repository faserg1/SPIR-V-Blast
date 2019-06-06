# Summary
Blast (SPIR-V Blast) — is a C-like high-level language, supposed to use all abilities of SPIR-V.

# Usage and arguments

Blast [options] <input file>
Options:
 * -o|--output <output file> - set the outputfile
 * -D|--define <KEY=VALUE> - add the define for preprocessor
 * -I|--include <include dir> - add the include directory for file search

# Comments

You can use inline comments:
```
a = b + c; // Inline comment
```

And multiline comments:
```
/*
This
whole
thing
ignoring
by the compiler!
*/
```
# Preprocessor

## defines
You can use defines in code:
```
#define something
#define something2 yey
#undef something
```
You can check the defines:
```
#ifdef SOME_DEFINE
a = b + c;
#endif

#ifndef SOME_NOT_SETTED_DEFINE
a = b + c;
#endif
```
## includes

Include some files is shader! Reuse your code :)

```
#include "localfile.blast"
#include <globalfile.blast>
```

# Shader settings

## Capabilities

```
#capability <capability>
```

## Entry point

```
#entry <function_name> <execution_model>
```

## Memory model

```
#memory <addressing_model> <memory_model>
```

# The Blast language itself

## Basic types

There are some basic type, you can use:
 - `void`
 - `bool`
 - `int`
 - `float`

For int you can also change the size and signedness, and change size of float:
```
int<64> //as int64_t in C++
int<64, 0> //as uint64_t in C++
float<64> //as float64_t in C++
```

## Comples types

The shader cannot live without vectors and matricies. So how to define it?

```
// Syntax:
mat<base_type, rows, columns>
vec<base_type, component_types>

//Example
mat<float, 4, 4> // The 4x4 matrix of float32
vec<int<64>, 4> // The 4 component vector of int64
```

## Pointers

As in SPIR-V, there are pointers in language. The syntax is same as in C++.
```
void *
float<16> *
```

## Enumerations

You can define enumerations simple as in C++. The exception is that enumeration keys are always scoped.

```
enum MySuperEnum : int
{
	Nice,
	Dice = 50,
	Paradice // = 51
};
```

For now (but I wish to fix this in future) enum resolving is:

```
::MySuperEnum::Dice;
```

## Structures

Syntax to define structure same as in C++:

```
struct Struct
{
	int i, i1, i2;
	float f, f1, f2;
};
```

## Global variables

Simple. As in C++. And there — there are only one way to write shaders — USE global variables.

```
vec<int, 4> *posIn;
vec<int, 4> *posOut;
```

## Attributes

In this language there are NO predefined variables and constants. If you want to declare somthing as BuiltIn or say about layout, there is only one way — attributes. Syntax of attributes is mostly the same as in C++11. For example, let define the input and output position of vertex:

```
[[bind:0]]
[[in:0]]
vec<int, 4> *posIn;
[[bind:0]]
[[out:0]]
[[builtIn:Position]]
vec<int, 4> *posOut;
```
