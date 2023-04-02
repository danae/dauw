import re


# Function that prints a plural s if applicable
def s(num):
  return "s" if num != 1 else ""


# Function that removes ANSI escape codes from a string
def ansi_unescape(string):
  return re.sub(r"\x1B(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])", "", string)


# Function that prints a horizontal divider
def divider():
  print("─" * 79)

# Function that prints a dashed horizontal divider
def divider_dashed():
  print("─ " * 39 + "─")
