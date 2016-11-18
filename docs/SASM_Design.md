SASM Design Document
====================

The SASM Assembler is a simple macro assembler for SIMEX.  Input is parsed in
two stages.  In the first stage, preprocessing is performed, providing basic
macro expansion, conditional compilation, and include directive expansion.  In
this stage, comments are transformed into whitespace.  In the second stage,
assembler directives are parsed, including section definitions, labels, and
assembler code.  In order to reduce dependencies, this assembler is hand-coded
using two lexers and two parsers.  One lexer/parser combination manages
preprocessing, and the output of this is fed into the second lexer/parser
combination, which parses assembler directives.

After input is parsed into an abstract syntax tree, two analysis stages are
performed.  In the first analysis stage, a symbol table is built with all
symbols defined in the source file.  In the second analysis stage, semantic
analysis is performed on the instructions, and all symbol references are
resolved.

After analysis, the code generation phase executes.  This is performed in two
stages.  In the first stage, data and code is laid out in the output file
according to assembler directives.  In the second stage, relative addresses are
backpatched and the relocation table is created.  This relocation table is used
by the linker to resolve absolute addresses and external addresses.

Preprocessor Lexical Analysis
-----------------------------

The preprocessor lexer examines input looking for comments, identifiers,
strings, integers, decimal numbers, parentheses, commas, hash symbols, and basic
arithmetic symbols.  The lexer also passes unknown sequences verbatim to the
preprocessor parser.  Finally, the lexer tracks the raw line number and file
name.  These are used by the preprocessor to compute the logical line number and
file name.

A comment can take two forms: a C-style comment and a C++ style comment.  The
regular language markup for a C-style comment is:

    /\*!(\*/)*\*/


    ------ '/'  ----------  '*'   -----------  '*'   -----------  '/'   --------
    |init|----->|needStar|------->|inComment|------->|needSlash|------->|Accept|
    ------      ----------        -----------        -----------        --------
      ^             |             ^ ^     |                   |
      |--------------             | |------                   |
          not '*'                 | not '*'                   |
                                  |----------------------------
                                              not '/'

This matches against any sequence that starts with a `/*` and ends with a `*/`.
The regular language markup for a C++-style comment is:

    //.*$

    ------ '/'  -----------  '/'  -----------  EOL   --------
    |init|----->|needSlash|------>|inComment|------->|Accept|
    ------      -----------       -----------        --------
                                    ^      |
                                    |-------
                                     not EOL

This matches any sequence that starts with a `//` up to the end of a line.

The regular language markup for a string is:

    "(([^"])|(\\.))*"

    ------ '"'  ----------------  '"'  --------
    |init|----->|   inString   |------>|Accept|
    ------      ----------------       --------
                 ^      |  ^ |    '\'
                 |-------  | ---------------|
                  not '"'  |                V
                           |           --------
                           ------------|escape|
                               any     --------

At this point, the relationship between the regular language markup and the
state machine descriptions of automata which accept this input should be clear,
so the state machine descriptions will be omitted from the following expressions
for brevity.

The regular language markup for an identifier is:

    [A-Za-z_][A-Za-z_0-9]*

The regular language markup for an integer is:

    (0x[0-9A-Fa-f]*)|([0-1]*b)|(0[0-7])|([0-9]*)

Note that this expression does not include a unary negative sign, as
interpreting this sign is context-sensitive. For instance, `5 - 2` represents
the arithmetic expression, five minus two, but `-7` represents the integer
value, negative seven.  Therefore, the parser is needed to fully resolve integer
expressions.

The regular language markup for a decimal number is:

    ([0-9]*(\.[0-9]+)?(e(\+|-)*[0-9]+)?<< len > 0 >>

The expression starts with an optional sequence of digits, followed by an
optional decimal part that starts with a dot, followed by an optional
exponentiation part that starts with an `e` and may include a positive or
negative exponent.  Finally, the resulting expression must have a length greater
than 0.

Finally, the preprocessor lexer accepts an open parethethesis, a close
parenthesis, a plus, a minus, a star, a double-star, a slash, a backslash, a
comma, and a hash.  A backslash at the end of a line joins that line to the next
line for the purpose of evaluation, and is treated as a special character.  All
other characters encountered are passed to the parser as `PTok::Unknown` values,
which are treated verbatim by the parser except where they would cause syntax
errors in the preprocessor language.  In order to ensure that we can read
multibyte files, the preprocessor language also groups high-bit bytes together
into a single `PTok::Unknown` token value, which ensures that these tokens are
not treated as separate characters by the parser.

For input categories, we have the following:

    PCat::Whitespace
    PCat::LineEnd
    PCat::FSlash
    PCat::BSlash
    PCat::Star
    PCat::Bang
    PCat::DoubleQuote
    PCat::Exp
    PCat::Alpha
    PCat::Underscore
    PCat::HexX
    PCat::BinB
    PCat::BNum
    PCat::ONum
    PCat::DNum
    PCat::HNum
    PCat::Dot
    PCat::Plus
    PCat::Minus
    PCat::OParen
    PCat::CParen
    PCat::Lt
    PCat::Eq
    PCat::Gt
    PCat::Comma
    PCat::Hash
    PCat::HighBit
    PCat::EndOfFile
    PCat::Unknown

To transform this regular language markup into a lexical analyzer, we build a
finite state machine which operates on a table of input categories, indexed by
the current state.  The category for an input byte is looked up from a 256-entry
input category table, which maps each input byte into one of the above
categories.  Then, the current state of the finite state machine is used as an
index into the current input category table, which is used to look up the action
for the given character.  Actions can change the machine state, pass tokens to
the parser, drop characters (such as comments or whitespace), or perform some
combination of these actions.

In order to compute the states required for the lexical analyzer, we first
compute states for each regular language expression above, and then we combine
these states into a single automaton.  Ambiguous input sequences follow the same
state graph up until the point that they diverge.  Non-divergent paths, such as
a non-decimal number which may be either an integer or a floating point number,
are resolved by the parser.  We pick a general token that works in both cases,
in this case, a `PTok::Integer` token, and allow the parser to treat this
sequence differently if needed by context.

Because the preprocessor must also track lines, it runs input through two
state machines.  The first simply checks for white space or line ends and its
actions update the physical line number.  For simplicity sake, this state
machine also filters comments.  Comments and whitespace are reduced to a minimal
amount, which makes subsequent lexing and parsing easier.  The second state
machine performs lexical analysis on the input filtered by the first state
machine.

The preprocessor lexer tokenizes input, which simplifies the preprocessor
parser.  The list of tokens supported by the preprocessor lexer are below.

    PTok::Newline
    PTok::String
    PTok::SystemString
    PTok::Include
    PTok::IfDef
    PTok::If
    PTok::ElIf
    PTok::Else
    PTok::Pragma
    PTok::Warning
    PTok::Error
    PTok::And
    PTok::Or
    PTok::Not
    PTok::OParen
    PTok::CParen
    PTok::Comma
    PTok::Identifier
    PTok::Integer
    PTok::Number
    PTok::Concat
    PTok::Stringify
    PTok::Unknown

The preprocessor uses newlines to delimit preprocessor directives.  The
preprocessor lexer reduces a hash token followed by an identifier into either
one of the built-in directives, such as `#include` or `#if`, or into a
stringification operation on a given identifier.  The `||` and `&&` operators
are reduced to `PTok::Or` and `PTok::And` respectively.  Open and close
parentheses are treated specially, as these can be used to group expressions.
Integers and decimal numbers are tokenized specially so these can be used in
pragma definitions and for future constant folding.
