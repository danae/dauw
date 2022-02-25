Syntax
======

The syntax of Dauw is heavily inspired by [Lua](https://en.wikipedia.org/wiki/Lua_(programming_language)), [Nim](https://en.wikipedia.org/wiki/Nim_(programming_language)) and [Python](https://en.wikipedia.org/wiki/Python_(programming_language)).


Syntax grammar
--------------

The following block shows the (context-free) grammar of the syntax of Dauw. Parsing always starts at the `module` symbol.

The syntax used to describe the grammars is an extended form of the grammar notation used in [*Crafting Interpreters*](https://craftinginterpreters.com/representing-code.html#rules-for-grammars), with the addition of the `?` and `?!` symbols, which treats the following token as positive and negative lookahead respectively::

    module              → expr (NEWLINE expr)* EOF

    expr                →

    assignment_expr     → (primary_expr '.')? IDENTIFIER '=' assignment_expr | func_expr

    comp_expr           → comp_do_expr | comp_namespace_expr | func_expr
    comp_do_expr        → 'do' block_expr
    comp_namespace_expr → 'namespace' block_expr

    func_expr           → '(' func_parameters? ')' '=' func_body | block_expr
    func_parameters     → variable (',' variable)*
    func_body           → block_expr

    block_expr          → (NEWLINE expr)+ ((?!block_implicit_end) 'end')? | ctrl_expr
    block_implicit_end  → 'do' | 'else' | 'then' | 'yield'

    ctrl_expr           → ctrl_if_expr | ctrl_while_expr | ctrl_until_expr | ctrl_for_expr | simple_expr
    ctrl_if_expr        → 'if' expr 'then' expr ('else' expr)?
    ctrl_while_expr     → 'while' expr 'do' expr
    ctrl_until_expr     → 'until' expr 'do' expr
    ctrl_for_expr       → 'for' variable 'in' expr 'do' expr

    simple_expr         → logic_or_expr
    logic_or_expr       → logic_and_expr ('or' logic_and_expr)*
    logic_and_expr      → logic_not_expr ('and' logic_not_expr)*
    logic_not_expr      → 'not' logic_not_expr | equals_expr
    equals_expr         → compare_expr (equals_op compare_expr)*
    equals_op           → '==' | '<>' | '~'
    compare_expr        → threeway_expr (compare_op threeway_expr)*
    compare_op          → '<' | '<=' | '>' | '>='
    threeway_expr       → range_expr (threeway_op range_expr)*
    threeway_op         → '<=>'
    range_expr          → add_expr (range_op add_expr)?
    range_op            → '..'
    add_expr            → multiply_expr (add_op multiply_expr)*
    add_op              → '+' | '-'
    multiply_expr       → unary_expr (multiply_op primary)*
    multiply_op         → '*' | '/' | '//' | '%'
    unary_expr          → unary_op unary_expr | primary_expr
    unary_op            → '#' | '$'

    primary_expr        → atom (primary_call | primary_subscript | primary_access)*
    primary_call        → '(' arguments? ')'
    primary_subscript   → '[' arguments? ']'
    primary_access      → '.' variable

    atom                → literal | variable | list | record | '(' expr ')'

    literal             → literal_nothing | literal_bool | literal_int | literal_float | literal_regex | literal_string
    literal_nothing     → 'nothing'
    literal_bool        → 'false' | 'true'
    literal_int         → INT
    literal_float       → FLOAT
    literal_regex       → REGEX
    literal_string      → STRING

    variable            → IDENTIFIER
    list                → '[' (list_item (',' list_item)*)? ']'
    list_item           → expr
    record              → '{' (record_item (',' record_item)*)? '}'
    record_item         → IDENTIFIER '=' expr

    parameters          → IDENTIFIER (',' IDENTIFIER)*
    arguments           → expr (',' expr)*



Operators
---------

Dauw uses the following operators, which are listed from highest to lowest precedence:

Prec | Operator           | Description                               | Associates
---- | ------------------ | ----------------------------------------- | ----------
1    | `()` `[]` `.`      | Call, Subscript, Access                   | Left
2    | `#` `$`            | Length, String                            | Right
3    | `*` `/` `//` `%`   | Multiply, Divide, Floor divide, Modulo    | Left
4    | `+` `-`            | Add, Subtract                             | Left
5    | `..`               | Range                                     | None
6    | `<=>`              | Three-way compare                         | None
7    | `<` `<=` `>` `>=`  | Compare                                   | None
8    | `==` `<>` `~`      | Equals, Not equals, Match                 | Left
9    | `not`              | Logical not                               | Right
10   | `and`              | Logical and                               | Left
11   | `or`               | Logical or                                | Left
12   | `=`                | Assignment                                | Right

Correspondence between operators and their function names (**to be decided**):

Operator    | Corresponding method
----------- | --------------------
`x(a)`      | `x.call(a)`
`x[i]`      | `x.index(i)`
`x[i] = a`  | `x.indexAssign(i, a)`
`x.y`       | `x.field(y)`
`x.y = a`   | `x.fieldAssign(y, a)`
`#x`        | `x.length()`
`$x`        | `x.string()`
