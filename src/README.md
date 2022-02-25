# Dauw

Dauw is a high-level, statically typed, object-oriented, general-purpose scripting language, taking inspiration from [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language)) and [Python](https://en.wikipedia.org/wiki/Python_(programming_language)).

## Example source code

```
-- Show the awesome stropped identifiers and Unicode support
foo = 42
pi_plus_two = 5.14_15_92
`妖精` = "I'm a fairy!"
`✈️` = "Ready for take-off!"


-- Simple greeter program
greeterProgram() =
  write("Enter your name: ")
  if name = readLine() then
    writeLine("Hello, $name!")
  else
    writeLine("Hello, World!")


-- Fibonacci sequence program
fibProgram() =
  fib(n: Int): Int =
    if n < 2 then n else fib(n - 1) + fib(n - 2)

  write("Enter a number: ")
  n = parseInt(readLine())

  write("Fibonacci number: ")
  write(fib(n))
  writeLine()


-- Execute the programs
greeterProgram()
fibProgram()
```
