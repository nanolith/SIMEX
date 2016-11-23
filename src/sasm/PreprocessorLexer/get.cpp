/**
 * \file sasm/PreprocessorLexer/get.cpp
 *
 * Implementation of PreprocessorLexer::get() method.
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>
#include "PreprocessorLexerImplementation.h"

using namespace simex::sasm;
using namespace std;

static bool actionIgnore(PreprocessorLexerImplementation*, int, PTok*);
static bool actionAcceptEOF(PreprocessorLexerImplementation*, int, PTok*);
static bool actionAcceptNewline(PreprocessorLexerImplementation*, int, PTok*);

/**
 * Preprocessor state machine.
 */
preprocessor_method_t preprocessorMachine[2][31] = {
    //Init State
    {
      //Whitespace --> Init
      &actionIgnore,
      //LineEnd --> Init
      &actionAcceptNewline,
      //FSlash --> Init
      &actionIgnore,
      //BSlash --> Init
      &actionIgnore,
      //Star --> Init
      &actionIgnore,
      //Bang --> Init
      &actionIgnore,
      //DoubleQuote --> Init
      &actionIgnore,
      //Exp --> Init
      &actionIgnore,
      //Alpha --> Init
      &actionIgnore,
      //Underscore --> Init
      &actionIgnore,
      //HexX --> Init
      &actionIgnore,
      //BinB --> Init
      &actionIgnore,
      //BNum --> Init
      &actionIgnore,
      //ONum --> Init
      &actionIgnore,
      //DNum --> Init
      &actionIgnore,
      //HNum --> Init
      &actionIgnore,
      //Dot --> Init
      &actionIgnore,
      //Plus --> Init
      &actionIgnore,
      //Minus --> Init
      &actionIgnore,
      //OParen --> Init
      &actionIgnore,
      //CParen --> Init
      &actionIgnore,
      //Lt --> Init
      &actionIgnore,
      //Eq --> Init
      &actionIgnore,
      //Gt --> Init
      &actionIgnore,
      //Comma --> Init
      &actionIgnore,
      //Hash --> Init
      &actionIgnore,
      //Ampersand --> Init
      &actionIgnore,
      //Pipe --> Init
      &actionIgnore,
      //HighBit --> Init
      &actionIgnore,
      //EOF --> Init
      &actionAcceptEOF,
      //Unknown --> Init
      &actionIgnore
    },
    //Identifier State
    {
      //Whitespace --> Init
      &actionIgnore,
      //LineEnd --> Init
      &actionIgnore,
      //FSlash --> Init
      &actionIgnore,
      //BSlash --> Init
      &actionIgnore,
      //Star --> Init
      &actionIgnore,
      //Bang --> Init
      &actionIgnore,
      //DoubleQuote --> Init
      &actionIgnore,
      //Exp --> Init
      &actionIgnore,
      //Alpha --> Init
      &actionIgnore,
      //Underscore --> Init
      &actionIgnore,
      //HexX --> Init
      &actionIgnore,
      //BinB --> Init
      &actionIgnore,
      //BNum --> Init
      &actionIgnore,
      //ONum --> Init
      &actionIgnore,
      //DNum --> Init
      &actionIgnore,
      //HNum --> Init
      &actionIgnore,
      //Dot --> Init
      &actionIgnore,
      //Plus --> Init
      &actionIgnore,
      //Minus --> Init
      &actionIgnore,
      //OParen --> Init
      &actionIgnore,
      //CParen --> Init
      &actionIgnore,
      //Lt --> Init
      &actionIgnore,
      //Eq --> Init
      &actionIgnore,
      //Gt --> Init
      &actionIgnore,
      //Comma --> Init
      &actionIgnore,
      //Hash --> Init
      &actionIgnore,
      //Ampersand --> Init
      &actionIgnore,
      //Pipe --> Init
      &actionIgnore,
      //HighBit --> Init
      &actionIgnore,
      //EOF --> Init
      &actionAcceptEOF,
      //Unknown --> Init
      &actionIgnore
    }
};

/**
 * Analyze an input character based upon the current input state.
 */
static bool analyze(PreprocessorLexerImplementation* impl, int ch, PTok* tok)
{
    PCat cat = character2PCat(ch);

    return (preprocessorMachine[impl->inputState][pcat2index(cat)])(
                impl, ch, tok);
}

/**
 * Compute the line/column for a cached character.
 */
static void
computeCachedCharLineCol(PreprocessorLexerImplementation* impl, int ch)
{
    if (impl->cachedChar == '\n')
    {
        ++impl->line;
        impl->column = 0;
    }
    else if (impl->cachedChar != EOF && !impl->isEof)
    {
        ++impl->column;
    }
}

/**
 * Cache the previous line / column information.
 */
static void
cacheCharLineCol(PreprocessorLexerImplementation* impl, int ch)
{
    if (!impl->isEof)
    {
        impl->column = impl->filt_.columnNumber();
        impl->line   = impl->filt_.lineNumber();
    }
}

/**
 * The get method returns a token from the input stream.
 *
 * \returns a token from the input stream, or PTok::EndOfFile if the end of
 * the input stream has been reached.
 */
PTok PreprocessorLexer::get()
{
    PTok ret;
    int ch;

    do {
        //if we have a cached character, process this first
        if (impl_->haveCachedChar)
        {
            impl_->haveCachedChar = false;
            ch = impl_->cachedChar;
            computeCachedCharLineCol(impl_.get(), ch);
        }
        else
        {
            ch = impl_->filt_.get();
            cacheCharLineCol(impl_.get(), ch);
        }
    } while (!analyze(impl_.get(), ch, &ret));

    return ret;
}

/**
 * Ignore the current character and remain in the current state.
 */
static bool actionIgnore(PreprocessorLexerImplementation*, int, PTok*)
{
    return false;
}

/**
 * Accept an EOF token, and reset the state to Init.
 */
static bool
actionAcceptEOF(PreprocessorLexerImplementation* impl, int ch, PTok* tok)
{
    impl->inputState =
        preprocessorLexerState2index(PreprocessorLexerState::Init);

    impl->isEof = true;
    *tok = PTok::EndOfFile;

    return true;
}

/**
 * Accept a Newline token and return to the Init state.
 */
static bool
actionAcceptNewline(PreprocessorLexerImplementation* impl, int ch, PTok* tok)
{
    impl->inputState =
        preprocessorLexerState2index(PreprocessorLexerState::Init);
    *tok = PTok::Newline;

    return true;
}
