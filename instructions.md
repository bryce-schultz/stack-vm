## Halt
### `HALT`
stops the vm

## Nop
### `NOP`
skips a clock cycle

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

## Greater Than
### `GT`
greater than with the last two stack values
result is stored on the stack

## Greater Than Or Equal
### `GE`
greater than or equal with the last two stack values
result is stored on the stack

## Less Than
### `LT`
less than with the last two stack values
result is stored on the stack

## Less Than Or Equal
### `LE`
less than or equal with the last two stack values
result is stored on the stack

## Equal
### `EQ`
check if the last two values on the stack are equal
result is stored on the stack

## Not Equal
### `NE`
check if the last two values on the stack are not equal
result is stored on the stack

## And
### `AND`
checks if the last two values on the stack are both true
result is stored on the stack

## Or
### `OR`
checks if either of the last two values on the stack are true
results is stored on the stack

## Print
### `PRINT`
pops an i64 off the stack and prints it

## Print String
### `PRINTS`
pops a string off the stack and prints it (see Strings section)

## Concat
### `CONCAT`
concatinates two strings on the stack

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
jump if greater than with the last two stack values

## Jump if Greater Than or Equal
### `JGE <label>`
jump if greater than or equal with the last two stack values

## Jump if Less Than
### `JLT <label>`
jump if less than with the last two stack values

## Jump if Less Than or Equal
### `JLE <label>`
jump if less than or equal with the last two stack values

## Store
### `STORE <address>`
stores the value on the stack in the specified address

## Param Store
### `PSTORE <address>`
stores the last value on the stack in the specified address skipping the return address

## Load
### `LOAD <address>`
loads the value from the specified address and pushes it on the stack

## Store String
### `STOSTR <address>`
stores a string in the sepcified address

## Load String
### `LOADST <address>`
loads a string from the specified address and pushes it on the stack

## Syscall
### `SYSCALL`
pops the syscall number the depending on syscall pops the args for it

## Call
### `CALL <function>`
calls the function by label pusing the return address on the stack

## Return
### `RET`
return from a function with no return value

## Return Value
### `RETVAL`
returns from a function with a return value
return value is placed on the stack
