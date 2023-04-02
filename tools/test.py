#!/usr/bin/env python3

import colorama
import os
import os.path
import re
import subprocess
import sys
import time

from argparse import ArgumentParser
from colorama import Fore, Back, Style

from utils import s, ansi_unescape, divider, divider_dashed


# Class that defines a test expectation
class Expectation:
  # Constructor
  def __init__(self, type, message, line):
    self.type = type
    self.message = message
    self.line = line

  # Return a string repesentation for the expectation
  def __str__(self):
    return f"\"{self.message}\" for {self.type} at line {self.line + 1}"

  # Match the expectation against a line
  def __call__(self, line):
    if self.type == "output":
      return self.message == line
    elif self.type == "error":
      return bool(re.search(self.message, line))
    else:
      return False


# Class that defines a test result
class Result:
  # Constructor
  def __init__(self, type, message):
    self.type = type
    self.message = message

  # Return a string repesentation for the result
  def __str__(self):
    if self.type == "info":
      return Style.DIM + "[✓] " + Style.NORMAL + self.message + Style.RESET_ALL
    elif self.type == "pass":
      return Style.DIM + "[✓] " + Style.NORMAL + Fore.GREEN + self.message + Style.RESET_ALL
    elif self.type == "warn":
      return Style.DIM + "[ ] " + Style.NORMAL + Fore.YELLOW + self.message + Style.RESET_ALL
    elif self.type == "fail":
      return Style.DIM + "[ ] " + Style.NORMAL + Fore.RED + self.message + Style.RESET_ALL
    else:
      return Style.DIM + "[ ] " + Style.NORMAL + self.message + Style.RESET_ALL


# Class that defines a test
class Test:
  # Patterns for test expectations
  _expect_output_pattern = re.compile("-->\s*(.+)$")
  _expect_error_pattern = re.compile("-- expect error:\s*(.+)$")


  # Constructor
  def __init__(self, path):
    self.path = path

    self.expectations = []
    self.duration = None
    self.code = None
    self.lines = None

    # Read the file at the test path
    with open(self.path, mode = "r", encoding = "utf-8") as file:
      lines = file.readlines()

    # Iterate over the lines
    for i, line in enumerate(lines):
      # Check if the line contains an expected output pattern
      if match := self._expect_output_pattern.search(line):
        self.expectations.append(Expectation("output", match.group(1), i))

      # Check if the line contains an expected error pattern
      if match := self._expect_error_pattern.search(line):
        self.expectations.append(Expectation("error", match.group(1), i))

  # Return a string representation for the test
  def __str__(self):
    expectations = len(self.expectations)
    return f"{expectations} test{s(expectations)} in file " + Style.BRIGHT + self.path + Style.NORMAL;

  # Run the test and validate the expectations
  def __call__(self, interpreter_path):
    # If there are no expectations, then don't run
    if not self.expectations:
      yield Result("warn", "The test contains no expectations")
      return

    # Run the test
    self.duration, self.code, self.lines = self.run(interpreter_path)
    yield Result("info", f"Tests were run in {self.duration * 1000:.0f} ms resulting in exit code {self.code}")

    # Iterate over the expectations
    line_num = 0
    for expectation in self.expectations:
      # Check if there is a line to match
      if line_num >= len(self.lines):
        yield Result("fail", f"Missing expected {expectation}")
        continue

      line = self.lines[line_num]

      # Validate the expectation
      if expectation(line):
        yield Result("pass", f"Got expected output {expectation}")
      else:
        yield Result("fail", f"Expected {expectation} but got \"{line}\"")

      # Increase the line pointer
      if expectation.type == "error":
        line_num += 4
      else:
        line_num += 1

    # Check abundant lines
    if line_num < len(self.lines):
      yield Result("warn", f"Abundant output where none was expected")

  # Run the test
  def run(self, interpreter_path):
    # Run the subprocess and measure the time it takes to execute
    start = time.time()
    result = subprocess.run([interpreter_path, self.path], stdout = subprocess.PIPE, stderr = subprocess.STDOUT, text = True)
    end = time.time()

    # Accumulate the result
    duration = end - start
    code = result.returncode
    lines = [ansi_unescape(line) for line in result.stdout.splitlines()]
    if len(lines) > 0 and lines[-1] == "":
      del lines[-1]

    return duration, code, lines


# Class that runs a test suite
class Runner:
  # Constructor
  def __init__(self, interpreter_path, less = False):
    self.interpreter_path = interpreter_path
    self.less = less

    self.passes = 0
    self.fails = 0
    self.warnings = 0

  # Test the file at the specified path
  def test_file(self, path):
    # Create the test from the file
    test = Test(os.path.abspath(path))
    print(Style.BRIGHT + f"Running {test}:")

    # Run the test
    results = test(self.interpreter_path)

    # Print and accumulate the results
    test_passed = True
    for result in results:
      if result.type != "pass" or not self.less:
        print(result)

      if result.type == "fail" or result.type == "warn":
        test_passed = False

      self.passes += 1 if result.type == "pass" else 0
      self.fails += 1 if result.type == "fail" else 0
      self.warnings += 1 if result.type == "warn" else 0

    if not test_passed and test.lines:
      divider_dashed()
      print(Style.BRIGHT + f"Console output for {os.path.abspath(path)}:")
      for i, line in enumerate(test.lines):
        print(f"{i + 1:>3d} │ " + Style.DIM + line)

    divider()

    # Return self for chainability
    return self

  # Test the directory at the specified path
  def test_directory(self, path):
    # Iterate over the the directory and test them
    for entry in os.listdir(path):
      self.test(os.path.join(path, entry))

    # Return self for chainability
    return self

  # Test the file or directory at the specified path
  def test(self, path):
    path = os.path.abspath(path)
    if os.path.isdir(path):
      return self.test_directory(path)
    elif os.path.isfile(path) and path.endswith(".dauw"):
      return self.test_file(path)


# Main function
def main(args):
  # Initialize colorama
  colorama.init(autoreset = True)

  # Parse the command line arguments
  parser = ArgumentParser(prog = "test.py", description = "Run the test suite for the Dauw compiler and interpreter.")
  parser.add_argument("suite", action = "store", help = "path to the test suite")
  parser.add_argument("-p", "--path", action = "store", default = "./dauw", help = "path to the intepreter executable (defaults to './dauw')")
  parser.add_argument("-l", "--less", action = "store_true", help = "do not display passed test results")

  args = parser.parse_args(args)

  # Check the arguments
  if not os.path.exists(args.suite):
    print(f"Cannot run the test suite: the test suite path \"{args.suite}\" does not exist")
    sys.exit(1)
  if not os.path.exists(args.path) or not os.path.isfile(args.path):
    print(f"Cannot run the test suite: the interpreter path \"{args.path}\" does not exist or is not a file")
    sys.exit(1)

  # Create a test runner and run the tests in the suite
  runner_path = os.path.abspath(args.suite)
  runner = Runner(args.path, args.less).test(runner_path)

  print(f"Test results for {runner_path}:")
  print(Fore.GREEN + Style.BRIGHT + f"{runner.passes}" + Style.RESET_ALL + f" test{s(runner.passes)} passed", end = ", ")
  print(Fore.RED + Style.BRIGHT + f"{runner.fails}" + Style.RESET_ALL + f" test{s(runner.fails)} failed", end = ", ")
  print(Fore.YELLOW + Style.BRIGHT + f"{runner.warnings}" + Style.RESET_ALL + f" warning{s(runner.warnings)}")

  if runner.fails > 0:
    sys.exit(1)


# Execute the main function
if __name__ == "__main__":
  main(sys.argv[1:])
