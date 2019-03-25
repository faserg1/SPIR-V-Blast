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