![Hero](https://user-images.githubusercontent.com/19797697/69917292-ae06da00-14a7-11ea-9eb6-b06898bfffef.png)


**Flexon** is a code inspector implemented using flex & bison. It reads source codes written in a specific language with simple grammar, similar to Pascal, then handles many errors on compile time.

## Grammar

Download and read this [documentation](https://github.com/ihooni/flexon/files/3908466/Term.Project.in.PL.final.pdf) file to see more about the project.

### Resolving ambiguities

We found some ambiguities in given grammer. So before getting started, we resolved them by doing

- Use operator precedence
- Use fake token for precedence

## Error Handling

- Duplicate variables & parameters (reflect variable scope)
- Duplicate functions & procedures
- Undefined variables
- Undefined functions
- Too many arguments
- Too few arguments
- Invalid function argument types
- Use float or array value as array subscript
- Assign array value to non-array type variable
- Assign non-array value to array type variable
- Use procedure statement in the expression
- Return at the mainprog
- Return at the procedures
- Return as array value at the function
- Not return at the function
- Use array type variable in the arithmetic operations

## Build Instruction

1. Make

```bash
$ Make
```

2. Run executable with test data set

```bash
$ ./flexon test/demo.pas
```

## Contributors

- 김민규
- 우승진
- 장해민
- 황치훈

## LICENSE

MIT License

Copyright (c) 2019 [PAYW](https://payw.org)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
