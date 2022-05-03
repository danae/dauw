# The Dauw programming language

This repository contains the **interpreter and compiler for [Dauw](https://dauw.dev/)**, a high-level, statically typed, object-oriented, general-purpose scripting language, taking inspiration from [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language)) and [Python](https://en.wikipedia.org/wiki/Python_(programming_language)).

## Example source code

```
-- Show the awesome stropped identifiers and Unicode support
let foo = 42
let pi_plus_two = 5.14_15_92
let `false` = 360
let `妖精` = "I'm a fairy!"
let `✈️` = "Ready for take-off!"  -- This is a comment at the end of the line


-- Simple greeter program
func greeter_program() =
  write("Enter your name: ")
  if let name = read_line() then
    writeln("Hello, $name!")
  else
    writeln("Hello, World!")


-- Fibonacci sequence program
func fib_program() =
  func fib(n) = if n < 2 then n else fib(n - 1) + fib(n - 2)

  write("Enter a number: ")
  let n = parse_int(read_line())

  writeln("Fibonacci number: ${fib(n))}")


-- Program that demonstrates lambdas and first-class functions
func lambda_program(ints) =
  ints.map(\(i) = i * i).each(writeLine)


-- Execute the programs
greeter_program()
fib_program()
lambda_program([1, 2, 3, 4, 5])
```

## License

Dauw and its associated repositories are distributed under the terms of the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html). See [LICENSE.txt](LICENSE.txt) for details.
