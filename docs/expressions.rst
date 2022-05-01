===========
Expressions
===========


This chapter explains the meaning of the elements of simple expressions in Dauw, along with their respective syntax grammars. The following sntax grammar descibes utility rules that are used throughout the other syntax grammars in this chapter and the following chapters::

    parameters          → IDENTIFIER (',' IDENTIFIER)*
    arguments           → expression (',' expression)*


Atoms
=====

::

    atom                → literal | name | list | record | lambda | parenthesized
    parenthesized       → '(' expression ')'

Atoms are the basic building blocks of expressions, which consist of literals, identifiers or parenthesized expressions. A parenthesized atom yields whatever the enclosed expression yields and is used to enclose expressions with lower precedence in a high-precedence context.

Literals
--------

::

    literal             → 'nothing' | 'false' | 'true' | INT | REAL | STRING

A literal atom specifies an immutable value of one of the ``Nothing``, ``Bool``, ``Int``, ``Real`` or ``String`` types. The former two types are defined with the keywords ``nothing``, ``false`` and ``true``, while the latter can take several forms. A literal atom yields an object corresponding to the matching literal type with the specified immutable value.

Names
-----

::

    name                → IDENTIFIER

An identifier occuring as an atom is a name. When the name is bound to an object in the current scope, the evaluation of the atom yields that object, otherwise a ``NameError`` is reported.

Lists
-----

::

    list                → '[' (list_item (',' list_item)*)? ']'
    list_item           → expression

A list atom yields a ``List`` object when it is evaluated. The supplied list items are each evaluated and added into the list object.

Records
-------

::

    record              → '{' (record_item (',' record_item)*)? '}'
    record_item         → IDENTIFIER ':' expression

A record atom yields a ``Record`` object when it is evaluated. The supplied record items are each evaluated and put into the record object at the key specified by the identifier.

Lambdas
-------

::

    lambda              → '\' '(' parameters? ')' '=' expression

A lambda atom yields an anonymous ``Function`` object, taking the specified parameters and evaluating the specified body when the function is invoked. The function object acts like a fully-fledged function, with the diffference that it is not bound to a name.


Primaries
=========

::

    primary             → atom (call_postfix | subscript_postfix | access_postfix)*
    call_postfix        → '(' arguments? ')'
    subscript_postfix   → '[' arguments? ']'
    access_postfix      → '.' IDENTIFIER

Primary exressions represent the most tightly bound operations in the language, which consists of calls, subscriptions, and accesses. Primaries yield the following values when they are evaluated:

- A call primary calls a ``Callable`` object specified by the evaluated primary expression, by invoking the ```()``` function with the specified arguments. When a call primary is used as an assignment target, the ```()=``` function is invoked instead with the assignment value as first argument.
- A subscript primary subscribes to a ``Subscriptable`` object specified by the evaluated primary expression, by invoking the ```[]``` function with the specified arguments. When a subscript primary is used as an assignment target, the ```[]=``` function is invoked instead with the assignment value as first argument.
- A access primary accesses a field specified by the identifier in an object specified by the evaluated primary expression.

Operators
=========

::

    operation           → logic_or

Simple compond expressions can be made by using primary expressions as operands with one or more operators. The following operators are defined, which are listed below from highest to lowest precedence along with their associativity:

======  ================================  ==================================================  ==========
Prec    Operator                          Description                                         Associates
======  ================================  ==================================================  ==========
1       ``()`` ``[]`` ``.``               Call, Subscript, Access (covered by primaries)      Left
2       ``-`` ``#`` ``$``                 Negate, Length, String                              Right
3       ``*`` ``/`` ``//`` ``%``          Multiply, Divide, Floor divide, Modulo              Left
4       ``+`` ``-``                       Add, Subtract                                       Left
5       ``..``                            Range                                               None
6       ``<=>``                           Three-way compare                                   None
7       ``<`` ``<=`` ``>`` ``>=``         Compare                                             None
8       ``==`` ``<>`` ``~``               Equals, Not equals, Match                           Left
9       ``not``                           Logical not                                         Right
10      ``and``                           Logical and                                         Left
11      ``or``                            Logical or                                          Left
======  ================================  ==================================================  ==========

Most operators, excluding the ``<`` ``<=`` ``>`` ``>=`` ``<>`` relational operators and logical operators, are overloadable by defining a function bound to the name that represents the operator symbol, and that takes one (for unary operators) or two (for binary ones) arguments that represent the operands. The ``<`` ``<=`` ``>`` ``>=`` (compare) operators delegate to the ``<=>`` (three-way compare) operator; the ``<>`` (not equals) operator delegates to the ``==`` (equals) operator.

Unary operators
---------------

::

    unary               → ('-' | '#' | '$') unary | primary

The following are the unary operators:

* The unary ``-`` (negate) operator yields the negation of its number operand.

* The ``#`` (length) operator yields the length of its sequence operand as an ``Int``.

* The unary ``$`` (string) operator yields a string representation of its operand as a ``String``.

Arithmetic operators
--------------------

::

    term                → factor (('+' | '-') factor)*
    factor              → unary (('*' | '/' | '//' | '%') unary)*

The following binary operators are used for arithmetic operations:

* The ``+`` (add) operator yields the sum of its operands. It is commonly used to add two numeric types or concatenate two sequences.

* The binary ``-`` (subtract) operator yields the difference of its operands. It is commonly used to subtract two numeric types.

* The ``*`` (multiply) operator yields the product of its operands. It is commonly used to multiply two numeric types.

* The ``/`` (divide) operator yields the quotient of its operands. It is commonly used to divide two numeric types.

* The ``//`` (floor divide) and ``%`` (modulo) operators respectively yield the floor division and modulo of its operands. For numeric types, the two operators are related such that ``a == (a // b) * b + (a % b)`` holds.


Range operator
--------------

::

    range               → term ('..' term)?

The ``..`` (range) operator yields a range of objects specified by its operands. For numeric types, the operator yields a ``Range`` object that uses its operands as respectively the minimum and exclusive maximum value of the range.

Relational operators
--------------------

::

    equality            → comparison (('==' | '<>' | '~') comparison)*
    comparison          → threeway (('<' | '<=' | '>' | '>=' | '%%') threeway)*
    threeway            → range ('<=>' range)*

The following binary operators are used for relational comparison and all yield a ``Bool`` result:

* The ``<=>`` (three-way compare) operator yields a negative integer, zero, or a positive integer if the first operand is respectively less than, equal or greater than the second operand. This operator can be used to lexicographically sort a sequence of objects.

* The ``<`` ``<=`` ``>`` ``>=`` (compare) operators delegate to the ``<=>`` operator to yield a boolean value that respresents if the first operand is less than, less than or equal, greater than, or greater than or equal to the second operand respectively. Those operators are not overloadable.

* The ``==`` (equals) operator yields a boolean result that represents if its operands are equal to each other.

* The ``<>`` (not equals) operator yields a boolean result that represents if its operands are not equal to each other, i.e. ``a <> b`` equals ``not a == b``. This operator is not overloadable as it delegates to the ``==`` operator.

* The ``~`` (match) operator yields a boolean result that represent if the first operand matches the second operand. The exact semantic behaviour of this operator can differ among types.

Logic operators
---------------

::

    logic_or            → logic_and ('or' logic_and)*
    logic_and           → logic_not ('and' logic_not)*
    logic_not           → 'not' logic_not | equality

The following binary operators are used for Boolean operations:

* The ``not`` (logic not) operator yields the inverse truth value of its operand.

* The ``and`` (logic and) operator yields the first operand if that evaluates to ``false``, or the second operand otherwise. Note that this operator short-circuits evaluation of the right operand.

* The ``or`` (logic or) operator yields the first operand if that evaluates to ``true``, or the second operand otherwise. Note that this operator short-circuits evaluation of the right operand.
