## Push
### `PUSH <i64 value>`
pushes a the value onto the stack

## Pop
### `POP`
pops a value of the stack

## Add
### `ADD`
pops two values off the stack and adds them
result is left on the stack

## Subtract
### `SUB`
pops two values off the stack and subtracts them
result is left on the stack

## Multiply
### `MUL`
pops two values off the stack and multiplies them
result is left on the stack

## Divide
### `DIV`
pops two values off the stack and divides them
result is left on the stack

## Modulus
### `MOD`
pops two values off the stack and takes the modulo operator of them
result is left on the stack

## Exponentation
### `POW`
pops two values off the stack and exponentiates them
result is left on the stack

## Negate
### `NEG`
pops a value off the stack and negates it
result is left on the stack

## Factorial
### `FACT`
pops a value off the stack and factorializes it
result is left on the stack

## Increment
### `INC`
pops a value off the stack and increments it
result is left on the stack

## Decrement
### `DEC`
pops a value off the stack and decrements it
result is left on the stack

## Nop
### `NOP`
skips a clock cycle

## Print
### `PRINT`
pops an i64 off the stack and prints it

## Print String
### `PRINTS`
pops a string off the stack and prints it (see Strings section)

## Concat
### `CONCAT`
concatinates two strings on the stack

## Halt
### `HALT`
stops the vm

## Duplicate
### `DUP`
duplicates the value on the stack

## Jump
### `JMP <label>`
jumps to the specified label

## Jump if Zero
### `JZ <label>`
jump to the specified label if the value on the stack is 0

## Jump if not Zero
### `JNZ <label>`
jump to the specified label if the value on the stack is not 0

## Jump if Equal
### `JE <label>`
jump to the specified label if the last two values on the stack are equal

## Jump if not Equal
### `JNE <label>`
jump to the specified label if the last two values on the stack are not equal

## Jump if Greater Than
### `JGT <label>`


JGE
JLT
GT
GE
LT
LE
EQ
NE
JLE
AND
OR
STORE
PSTORE
LOAD
STOSTR
LOADST
SYSCAL
CALL
RET
RETVAL
