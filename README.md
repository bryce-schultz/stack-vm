# svm

## Description

stack-vm is a set of projects including a compiler, stack machine and a vscode extension.
The compiler is for a simple language I am working on called Lithium with file extension .li for source and .lsm for assembly.
The stack machine reads the Lithium assembly and performs the simple instructions.

## Documentation

[Grammar](https://github.com/bryce-schultz/stack-vm/blob/master/grammar)
[Instruction List](https://github.com/bryce-schultz/stack-vm/blob/master/instructions.md)

## Building

### What gets build

Building will build the compiler located in src/lic. The executable is called lic and can be run like so.

`lic <source.li>`

It will also build the stack machine located in src/vm. The executable is called vm and can be run like so.

`vm <source.lsm>`

Lastly it builds the vscode extension in src/extension. The vsix file is in the build folder.

`lithium-language-support.vsix`

### How to build

`build`
