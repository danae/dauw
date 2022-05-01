==========
Data types
==========

Nothing
=======

The nothing value is a singleton value which represents the absence of a value. The only valid literal for this value is ``nothing``. This value is represented by the ``Nothing`` type (note the difference in capitalization in the type and literal).


Booleans
========

A boolean value represents a truth value. A boolean supports two literals, ``true`` and ``false``. A boolean value is represented by the ``Bool`` type.


Numbers
=======

There are two numeric types: integer values and double-precisioun floating point values. They are represented by the ``Int`` and ``Real`` types respectively, which both descend from the ``Number`` type.


Strings
=======

A string value is an array of Unicode code points, which are stored in UTF-8 encoding. Strings are represented by the ``String`` type.

Strings support interpolation of variables and arbitrary expressions too. A variable can be interpolated by prepending a dollar sign before it (``$foo``), while an expression can be interpolated by surrounding it with parentheses and prepending a dollar sign before it. The following examples demonstrate how string interpolation can be used:

::

  let foo = 42
  print("The value of foo is: $foo")  -- Outputs "The value of foo is: 42"
  print("Math $(3 + 4 * 5) is fun!")  -- Outputs "Math 23 is fun!"


Ranges
======

A range value specifies a consecutive range of integer values. Ranges are represented by the ``Range`` type and are created using the range literal, which specifies two integer numbers spearated by ``..``:

::

  let list = [1, 2, 3, 4]
  3 .. 8      -- Includes the integers 3 up to 7, but not 8 itself
  0 .. #list  -- Includes a range from 0 to the amount of items in the list, which is 4
