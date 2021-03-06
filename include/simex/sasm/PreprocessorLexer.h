/**
 * \file PreprocessorLexer.h
 *
 * Lexer for the Preprocessor.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#ifndef  SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD
# define SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <type_traits>

//this header is C++ specific
#ifdef __cplusplus

namespace simex { namespace sasm {

/**
 * Preprocessor Input Category.
 *
 * The preprocessor lexer categorizes input bytes as belonging to one of the
 * following categories.  These categories are then used by the lexer automaton
 * to create tokens for the preprocessor's parser.
 */
enum class PCat
{
    //all whitespace characters
    Whitespace                      = 0,
    //line end character
    LineEnd,
    //forward slash
    FSlash,
    //backslash
    BSlash,
    //*
    Star,
    //!
    Bang,
    //"
    DoubleQuote,
    //e, E
    Exp,
    //alphabetical characters, other than e, E, A-F, and a-f
    Alpha,
    //_
    Underscore,
    //x
    HexX,
    //b
    BinB,
    //0, 1
    BNum,
    //2, 3, 4, 5, 6, 7
    ONum,
    //8, 9
    DNum,
    //A-F, a-f
    HNum,
    //.
    Dot,
    //+
    Plus,
    //-
    Minus,
    //(
    OParen,
    //)
    CParen,
    //<
    Lt,
    //=
    Eq,
    //>
    Gt,
    //,
    Comma,
    //#
    Hash,
    //&
    Ampersand,
    //|
    Pipe,
    //a byte with the high bit set
    HighBit,
    //EOF
    EndOfFile,
    //all other bytes
    Unknown
};

/**
 * Convert a character to a preprocesser input category.
 * This is a lossy conversion.
 *
 * \param ch        The character to convert.
 *
 * \returns the preprocessor input category for this character.
 */
PCat character2PCat(int by);

/**
 * Convert a preprocessor input category to an index.
 *
 * \param cat       The input category to convert.
 *
 * \returns a 0-based index representing the input category.
 */
inline int pcat2index(PCat cat)
{
    //explicit conversion to int
    return static_cast<std::underlying_type<PCat>::type>(cat);
}

/**
 * Preprocessor Token.
 *
 * The preprocessor lexer creates tokens with the given enumerated values.
 * These tokens are then parsed by the preprocessor parser.
 */
enum class PTok
{
    //Newline
    Newline                         = 0,
    //String with all valid escapes
    String,
    //System string (used for system includes)
    SystemString,
    //#include preprocessing directive
    Include,
    //#ifdef preprocessing directive
    IfDef,
    //#ifndef
    IfNDef,
    //#if preprocessing directive
    If,
    //#elif preprocessing directive
    ElIf,
    //#else preprocessing directive
    Else,
    //#pragma preprocessing directive
    Pragma,
    //#warning preprocessing directive
    Warning,
    //#error preprocessing directive
    Error,
    //&&
    And,
    //||
    Or,
    //!
    Not,
    //(
    OParen,
    //)
    CParen,
    //,
    Comma,
    //a valid identifier
    Identifier,
    //a valid integer
    Integer,
    //a valid number
    Number,
    //##
    Concat,
    //#identifier -- also acts as intermediate for reducing directives above.
    Stringify,
    //All other input.  Typically passed to the downstream lexer as-is.
    Unknown,
    //indicates when the end of an input stream has been reached.
    EndOfFile,
    //Error token.  Indicate that the lexer encountered an invalid sequence.
    InputError
};

/**
 * Convert a preprocessor token into an index.
 *
 * \param tok       The preprocessor token to convert.
 *
 * \returns a 0-based index representing the preprocessor token.
 */
inline int ptok2index(PTok tok)
{
    //explicit conversion to int
    return static_cast<std::underlying_type<PTok>::type>(tok);
}

/**
 * The Filter interface is a source of filtered input.
 */
class Filter
{
public:

    /**
     * Filter destructor.
     */
    virtual ~Filter();

    /**
     * The get method works like istream::get(), except that the input is
     * filtered.
     *
     * \returns a filtered input byte, or EOF on EOF.
     */
    virtual int get() = 0;
};

/**
 * The LineFilter interface tracks the current line number.
 */
class LineFilter : public Filter
{
public:

    /**
     * Virtual destructor.
     */
    virtual ~LineFilter();

    /**
     * Get the current line number.
     */
    virtual int lineNumber() = 0;

    /**
     * Get the current column number.
     */
    virtual int columnNumber() = 0;
};

/**
 * The LineFilterImpl class tracks the current line number.
 */
class LineFilterImpl : public LineFilter
{
public:

    /**
     * Constructor.  Create a LineFilterImpl from an input stream.
     */
    LineFilterImpl(std::istream& in);

    /**
     * Virtual destructor.
     */
    virtual ~LineFilterImpl();

    /**
     * The get method works like istream::get(), except that the input is
     * filtered.
     *
     * \returns a filtered input byte, or EOF on EOF.
     */
    virtual int get();

    /**
     * Get the current line number.
     */
    virtual int lineNumber();

    /**
     * Get the current column number.
     */
    virtual int columnNumber();

private:
    std::istream& in_;
    int lineNumber_;
    int columnNumber_;
};

/**
 * Forward declaration to the private WhitespaceFilterImplementation.
 */
struct WhitespaceFilterImplementation;

/**
 * The WhitespaceFilter class reduces whitespace and comments down to a single
 * whitespace character, and keeps a physical line count.
 */
class WhitespaceFilter : virtual public LineFilter
{
public:

    /**
     * A WhitespaceFilter is created from an input stream.
     *
     * \param istream       The input stream used for this filter.
     */
    WhitespaceFilter(std::istream& in);

    /**
     * Destructor.  Clean up this instance.
     */
    virtual ~WhitespaceFilter();

    /**
     * The get method works like istream::get(), except that white space
     * characters and comments are condensed to a single space.
     *
     * \returns a filtered input byte, or EOF on EOF.
     */
    virtual int get();

    /**
     * Returns the current physical line number for this input stream.
     */
    virtual int lineNumber();

    /**
     * Get the current column number.
     */
    virtual int columnNumber();

private:
    std::unique_ptr<WhitespaceFilterImplementation> impl_;
};

/**
 * Forward declaration to the private PreprocessorLexerImplementation.
 */
struct PreprocessorLexerImplementation;

/**
 * The PreprocessorLexer class transforms an input stream to a series of tokens
 * that can be fed into the PreprocessorParser.  Methods to return the string,
 * integer, and double representations of tokens, where applicable, are
 * provided.
 */
class PreprocessorLexer
{
public:

    /**
     * A PreprocessorLexer is created from an input stream.
     *
     * \param istream       The input stream used for this filter.
     */
    PreprocessorLexer(std::istream& in);

    /**
     * Destructor.  Clean up this instance.
     */
    virtual ~PreprocessorLexer();

    /**
     * The get method returns a token from the input stream.
     *
     * \returns a token from the input stream, or PTok::EndOfFile if the end of
     * the input stream has been reached.
     */
    PTok get();

    /**
     * Returns the current physical line number for this input stream.
     */
    int lineNumber();

    /**
     * Get the current column number.
     */
    int columnNumber();

    /**
     * Returns the string representation of the current token.
     */
    std::string stringTok();

    /**
     * Returns the integer representation of the current token.
     */
    std::int64_t intTok();

    /**
     * Returns the unsigned integer representation of the current token.
     */
    std::uint64_t uintTok();

    /**
     * Returns the double representation of the current token.
     */
    double doubleTok();

    /**
     * Returns the line number where the token started.
     */
    int tokenStartLineNumber();

    /**
     * Returns the column number where the token started.
     */
    int tokenStartColumnNumber();

    /**
     * Returns the line number where the token ended.
     */
    int tokenEndLineNumber();

    /**
     * Returns the column number where the token ended.
     */
    int tokenEndColumnNumber();

private:
    std::unique_ptr<PreprocessorLexerImplementation> impl_;
};

/* namespace sasm */ } /* namespace simex */ }

//end of C++ code
#endif //__cplusplus

#endif //SIMEX_SASM_PREPROCESSOR_LEXER_HEADER_GUARD
