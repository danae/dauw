# The Dauw programming language

This repository contains the **interpreter and compiler for [Dauw](https://dauw.dev/)**, a high-level, statically typed, object-oriented, general-purpose scripting language, taking inspiration from [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language)) and [Python](https://en.wikipedia.org/wiki/Python_(programming_language)).

## Example source code

```
-- Show the awesome stropped identifiers and Unicode support
def foo = 42
def pi = 3.141_592_653_59
def piPlusTwo = pi + 2.0e+0
def `false` = false
def `妖精` = "I'm a fairy!"
def `✈️` = "Ready for take-off!"  -- This is a comment at the end of the line


-- Simple greeter program
def greeterProgram() =
  echo "Enter your name:"

  def name = console.gets()
  if name then
    echo "Hello, " + name + "!"
  else
    echo "Hello, World!"


-- Fibonacci sequence program
-- Calculates the fibonacci sequence for an index that is written by the user
def fibProgram() =
  -- Calculate the fibonacci number for index n
  def fib(n: Int): Int = if n < 2 then 1 else fib(n - 1) + fib(n - 2)

  echo "Enter a number: "
  def n = console.gets().toInt()

  echo "Fibonacci number: " + $fib(n)


-- Program that demonstrates lambdas and first-class functions
def lambdaProgram() =
  def ints = [1, 2, 3, 4, 5]
  for i in ints.map(\(i: Int) = i * i) do
    echo i


-- Program that demonstrates closures
def closureProgram() =
  def makeAdder(n: Int) = \(x: Int) = n + x

  def add5 = makeAdder(5)
  echo add5(10)

  def add7 = makeAdder(7)
  echo add7(10)


-- Execute the programs
greeterProgram()
fibProgram()
lambdaProgram()
closureProgram()
```

## License

Dauw and its associated repositories are distributed under the terms of the [GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html). See [LICENSE.txt](LICENSE.txt) for details.
