================
Lexical analysis
================

Encoding
========

Dauw source code should be in UTF-8 encoding or its ASCII subset; other encodings are not supported. Source code can either use Linux-style line endings (``\n``), which is preferred, or Windows-style line endings (``\r\n``). If source code can't be decoded, a ``SyntaxError`` is reported.


Indentation
===========

Dauw is an indentation sensitive language, which means that control structures in the language are expressed by their indentation, using the `off-side rule <https://en.wikipedia.org/wiki/Off-side_rule>`_. Indentation should only be marked by spaces; horizontal tabs or other whitespace characters are not supported.

The lexical analyzer produces `INDENT` and `DEDENT` tokens based on the amount of spaces at the start of each line in the source code:

- If the amount of spaces is bigger than the amount in the previous line, then the lexer produces an `INDENT` token and pushes that amount on the indentation stack. Should the lexer encounter indentation on the first line, it reports a ``SyntaxError``.
- Similarly, if the amount of spaces is less than the amount in the previous line, the lexer produces a `DEDENT` token for every item on the indentation stack that is bigger than the amount on the current line. If the amount of spaces doesn't align with a previous amount, then the lexer reports a ``SyntaxError``.

Except at the start of a line or in string or character literals, the space and horizontal tab characters are used to separate tokens if their concatenation could otherwise be interpreted as a different token.


Comments
========

An inline comment starts with ``--`` and runs until the end of a line. Dauw has no support for block-style comments. Presumed comments in string or regex literals are not lexed as comments, but as part of those literals. Comments that appear directly above a declaration are lexed as documentation strings; comments in other places are ignored by the parser.

If the first line of source code starts with `#!`, it is perceived as an `shebang <https://en.wikipedia.org/wiki/Shebang_(Unix)>`_  and ignored by the parser.


Identifiers and keywords
========================

Identifiers are described by the following lexical grammar::

    IDENTIFIER          → ASCII_IDENTIFIER | STROPPED_IDENTIFIER
    ASCII_IDENTIFIER    → (ALPHA | '_') ( ALPHA | DIGIT | '_')*
    STROPPED_IDENTIFIER → '`' <any source character except '`' or newline> '`'
    ALPHA               → 'a'...'z' | 'A'...'Z'
    DIGIT               → NONZERODIGIT | '0'
    NONZERODIGIT        → '1'...'9'
    HEXDIGIT            → DIGIT | 'a'...'f' | 'A'...'F'

Identifiers must start with an ASCII letter or the underscore and can be followed by any ASCI letter, ASCII digit or underscore. To use identifiers outside of the valid range as valid identifiers, such as operator symbols, they can be `stropped <https://en.wikipedia.org/wiki/Stropping_(syntax)>`_` by writing them betweem backticks. This also works for reserved keywords, which can be transformed into an ordinary identifier this way.

Some examples of valid identifiers in context::

    let foo = 42
    let pi_plus_two = 5.14_15_92
    let `false` = 360
    let `妖精` = "I'm a fairy!"
    let `✈️` = "Ready for take-off!"

The following words are reserved keywords and cannot be used as ASCII identifiers::

    and         false       nothing     true
    break       for         or          until
    discard     if          return      while
    do          in          save        yield
    else        not         then


Integer literals
================

Integer literals are described by the following lexical grammar::

    INT                 → SIGNED_INT | HEX_INT
    SIGNED_INT          → '-'? UNSIGNED_INT
    UNSIGNED_INT        → '0' | NONZERODIGIT ('_' | DIGIT)*
    HEX_INT             → '0' ('x' | 'X') HEXDIGIT ('_' | HEXDIGIT)*

An integer literal can be either an optionally negative decimal number, or a hexadecimal number preceded by the characters ``0x`` or ``0X``. When determining the numerical value of the literal underscores are ignored; they can be inserted for readability of the number. Note that decimal numbers can't start with leading zeroes, to avoid confusion with (unsupported) octal numbers from other programming languages.


Real literals
=============

Real (or floating point) literals are described by the following lexical grammar::

    REAL                → SIGNED_INT (('.') UNSIGNED_INT EXPONENT?) | EXPONENT)
    EXPONENT            → ('e' | 'E') ('+' | '-')? UNSIGNED_INT

A real literal is an optionally negative decimal number, followed by a decimal fractional part, a optionally signed decimal exponent preceded by the character ``e`` or ``E``, or both. As in integer literals, leading zeroes are not permitted and underscores are supported for readability.


Character and string literals
=============================

Character and string literals are described by the following lexical grammar::

    CHAR                → "'" CHAR_CHAR | ESCAPE_SEQ "'"
    CHAR_CHAR           → <any source character except "'", '\' or newline>
    STRING              → '"' (STRING_CHAR | ESCAPE_SEQ)* '"'
    STRING_CHAR         → <any source character except '"', '\' or newline>
    ESCAPE_SEQ          → '\' ("'" | '"' | '\' | 'b' | 'f' | 'n' | 'r' | 't' | 'u' '{' HEXDIGIT+ '}')

Character literals denote a single Unicode code-point and are delimited by single quotes ``''``. String literals denote a sequence of Unicode code points and are delimited by double quotes ``""``.

The following escape sequences are supported in character and string literals, which are mostly the same as supported by the `JSON standard <https://www.json.org/json-en.html>`_:

===============  ===========
Escape sequence  Description
===============  ===========
``\'``           Single quote
``\"``           Double quote
``\\``           Backslash (in order to output a regular backslash)
``\b``           Backspace (ASCII ``BS``)
``\f``           Form feed (ASCII ``FF``)
``\n``           Line feed/newline (ASCII ``LF``)
``\r``           Carriage return (ASCII ``CR``)
``\t``           Horizontal tab (ASCII ``TAB``)
``\u{···}``      Unicode code point specified by 1 to 6 hex digits
===============  ===========


Operators and delimiters
========================

The following sequences of symbols are used as operators::

    =   ==  <>  ~   <   <=  >   >=  <=> ..
    +   -   *   /   //  %   ?   #   $

The following tokens serve as delimiters in the grammar or are otherwise significant to the lexical analyzer::

    (   )   {   }   [   ]   ,   .   :   \
