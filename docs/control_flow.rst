============
Control flow
============

Modules and top-level expressions
=================================

The top-level of a module is described by the following syntax grammar. Parsing always starts at the ```module`` symbol::

    module              → line* EOF
    line                → expression NEWLINE
    block               → NEWLINE INDENT line+ DEDENT
    expression          → jump


Jump expressions
================

Jump expressions, which redirect evaluation of the module to a different point, are described by the following syntax grammar::

    jump                → return | break | continue | save | declaration
    return              → 'return' assignment?
    break               → 'break' assignment?
    continue            → 'continue' assignment?
    save                → 'save' assignment


Declaration expressions
=======================

Declaration expressions, which assign an evaluated expression to a name, are described by the following syntax grammar::

    declaration         → var_declaration | func_declaration | assignment
    var_declaration     → IDENTIFIER '=' assignment
    func_declaration    → IDENTIFIER '(' parameters? ')' '=' expression
    assignment          → call '=' call_assignment | control


Control flow expressions
========================

Control flow expressions, like conditional evaluation and looping, are described by the following syntax grammar::

    control             → conditional | for_do | for_yield | while_do | while_yield | until_do | until_yield | block | operation
    conditional         → 'if' operation 'then' expression ('else' expression)?
    for_do              → 'for' IDENTIFIER 'in' operation 'do' expression
    for_yield           → 'for' IDENTIFIER 'in' operation 'yield' expression
    while_do            → 'while' call_assignment 'do' expression
    while_yield         → 'while' call_assignment 'yield' expression
    until_do            → 'until' call_assignment 'do' expression
    until_yield         → 'until' call_assignment 'yield' expression
