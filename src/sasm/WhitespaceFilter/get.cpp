/**
 * \file sasm/WhitespaceFilter/get.cpp
 *
 * Implementation of WhitespaceFilter::get()
 *
 * Copyright (C) 2016 Justin Handville - All Rights Reserved.
 *
 * This file is part of the SIMEX Virtual Machine which is released under the
 * MIT License.  See LICENSE.txt in the root of this distribution for further
 * information.
 */

#include <simex/sasm/PreprocessorLexer.h>
#include "WhitespaceFilterImplementation.h"

using namespace simex::sasm;
using namespace std;

//forward declarations
static bool actionBeginWhitespace(WhitespaceFilterImplementation*, int*);
static bool actionBlockComment(WhitespaceFilterImplementation*, int*);
static bool actionLineComment(WhitespaceFilterImplementation*, int*);
static bool actionMaybeComment(WhitespaceFilterImplementation*, int*);
static bool actionMaybeLineEnd(WhitespaceFilterImplementation*, int*);
static bool actionMaybeEndComment(WhitespaceFilterImplementation*, int*);
static bool actionMaybeEscapeNewline(WhitespaceFilterImplementation*, int*);
static bool actionEscapeToMaybeComment(WhitespaceFilterImplementation*, int*);
static bool actionEscapeToInSpace(WhitespaceFilterImplementation*, int*);
static bool actionEscapeToInit(WhitespaceFilterImplementation*, int*);
static bool actionEscapeToEOFNewline(WhitespaceFilterImplementation*, int*);
static bool dropCharacter(WhitespaceFilterImplementation*, int*);
static bool passCharacter(WhitespaceFilterImplementation*, int*);
static bool passCharacterToInit(WhitespaceFilterImplementation*, int*);
static bool passSlashToEndOfFile(WhitespaceFilterImplementation*, int*);
static bool passSlashToInit(WhitespaceFilterImplementation*, int*);
static bool passSpaceToEndOfFile(WhitespaceFilterImplementation*, int*);
static bool passSpaceToInit(WhitespaceFilterImplementation*, int*);
static bool passSlashToInSpace(WhitespaceFilterImplementation*, int*);

/**
 * Filter state machine.
 */
filter_method_t filterMachine[8][31] = {
    //Init State
    {
      //Whitespace --> InSpace
      &actionBeginWhitespace,
      //LineEnd --> Init (pass character)
      &passCharacter,
      //FSlash --> MaybeComment
      &actionMaybeComment,
      //BSlash --> Init (pass character)
      &actionMaybeEscapeNewline,
      //Star --> Init (pass character)
      &passCharacter,
      //Bang --> Init (pass character)
      &passCharacter,
      //DoubleQuote --> Init (pass character)
      &passCharacter,
      //Exp --> Init (pass character)
      &passCharacter,
      //Alpha --> Init (pass character)
      &passCharacter,
      //Underscore --> Init (pass character)
      &passCharacter,
      //HexX --> Init (pass character)
      &passCharacter,
      //BinB --> Init (pass character)
      &passCharacter,
      //BNum --> Init (pass character)
      &passCharacter,
      //ONum --> Init (pass character)
      &passCharacter,
      //DNum --> Init (pass character)
      &passCharacter,
      //HNum --> Init (pass character)
      &passCharacter,
      //Dot --> Init (pass character)
      &passCharacter,
      //Plus --> Init (pass character)
      &passCharacter,
      //Minus --> Init (pass character)
      &passCharacter,
      //OParen --> Init (pass character)
      &passCharacter,
      //CParen --> Init (pass character)
      &passCharacter,
      //Lt -- Init (pass character)
      &passCharacter,
      //Eq -- Init (pass character)
      &passCharacter,
      //Gt -- Init (pass character)
      &passCharacter,
      //Comma --> Init (pass character)
      &passCharacter,
      //Hash --> Init (pass character)
      &passCharacter,
      //Ampersand --> Init (pass character)
      &passCharacter,
      //Pipe --> Init (pass character)
      &passCharacter,
      //HighBit --> Init (pass character)
      &passCharacter,
      //EOF --> Init (pass character)
      &actionMaybeLineEnd,
      //Unknown --> Init (pass character)
      &passCharacter
    },
    //InSpace State
    {
      //Whitespace --> InSpace (drop character)
      &dropCharacter,
      //LineEnd --> Init (pass newline)
      &passCharacterToInit,
      //FSlash --> MaybeComment
      &actionMaybeComment,
      //BSlash --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Star --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Bang --> Init (pass space; cache current character)
      &passSpaceToInit,
      //DoubleQuote --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Exp --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Alpha --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Underscore --> Init (pass space; cache current character)
      &passSpaceToInit,
      //HexX --> Init (pass space; cache current character)
      &passSpaceToInit,
      //BinB --> Init (pass space; cache current character)
      &passSpaceToInit,
      //BNum --> Init (pass space; cache current character)
      &passSpaceToInit,
      //ONum --> Init (pass space; cache current character)
      &passSpaceToInit,
      //DNum --> Init (pass space; cache current character)
      &passSpaceToInit,
      //HNum --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Dot --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Plus --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Minus --> Init (pass space; cache current character)
      &passSpaceToInit,
      //OParen --> Init (pass space; cache current character)
      &passSpaceToInit,
      //CParen --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Lt -- Init (pass space; cache current character)
      &passSpaceToInit,
      //Eq -- Init (pass space; cache current character)
      &passSpaceToInit,
      //Gt -- Init (pass space; cache current character)
      &passSpaceToInit,
      //Comma --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Hash --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Ampersand --> Init (pass space; cache current character)
      &passSpaceToInit,
      //Pipe --> Init (pass space; cache current character)
      &passSpaceToInit,
      //HighBit --> Init (pass space; cache current character)
      &passSpaceToInit,
      //EOF --> EndOfFile (pass space; cache EOF)
      &passSpaceToEndOfFile,
      //Unknown --> Init (pass space; cache current character)
      &passSpaceToInit
    },
    //MaybeComment State
    {
      //Whitespace --> InSpace (pass slash)
      &passSlashToInSpace,
      //LineEnd --> Init (pass slash; cache character)
      &passSlashToInit,
      //FSlash --> LineComment
      &actionLineComment,
      //BSlash --> Init (pass slash; cache character)
      &passSlashToInit,
      //Star --> BlockComment
      &actionBlockComment,
      //Bang --> Init (pass slash; cache character)
      &passSlashToInit,
      //DoubleQuote --> Init (pass slash; cache character)
      &passSlashToInit,
      //Exp --> Init (pass slash; cache character)
      &passSlashToInit,
      //Alpha --> Init (pass slash; cache character)
      &passSlashToInit,
      //Underscore --> Init (pass slash; cache character)
      &passSlashToInit,
      //HexX --> Init (pass slash; cache character)
      &passSlashToInit,
      //BinB --> Init (pass slash; cache character)
      &passSlashToInit,
      //BNum --> Init (pass slash; cache character)
      &passSlashToInit,
      //ONum --> Init (pass slash; cache character)
      &passSlashToInit,
      //DNum --> Init (pass slash; cache character)
      &passSlashToInit,
      //HNum --> Init (pass slash; cache character)
      &passSlashToInit,
      //Dot --> Init (pass slash; cache character)
      &passSlashToInit,
      //Plus --> Init (pass slash; cache character)
      &passSlashToInit,
      //Minus --> Init (pass slash; cache character)
      &passSlashToInit,
      //OParen --> Init (pass slash; cache character)
      &passSlashToInit,
      //CParen --> Init (pass slash; cache character)
      &passSlashToInit,
      //Lt -- Init (pass slash; cache character)
      &passSlashToInit,
      //Eq -- Init (pass slash; cache character)
      &passSlashToInit,
      //Gt -- Init (pass slash; cache character)
      &passSlashToInit,
      //Comma --> Init (pass slash; cache character)
      &passSlashToInit,
      //Hash --> Init (pass slash; cache character)
      &passSlashToInit,
      //Ampersand --> Init (pass slash; cache character)
      &passSlashToInit,
      //Pipe --> Init (pass slash; cache character)
      &passSlashToInit,
      //HighBit --> Init (pass slash; cache character)
      &passSlashToInit,
      //EOF --> EndOfFile (pass slash; cache EOF)
      &passSlashToEndOfFile,
      //Unknown --> Init (pass slash; cache character)
      &passSlashToInit
    },
    //LineComment State
    {
      //Whitespace --> LineComment (drop character)
      &dropCharacter,
      //LineEnd --> Init (pass character)
      &passCharacterToInit,
      //FSlash --> LineComment (drop character)
      &dropCharacter,
      //BSlash --> LineComment (drop character)
      &dropCharacter,
      //Star --> LineComment (drop character)
      &dropCharacter,
      //Bang --> LineComment (drop character)
      &dropCharacter,
      //DoubleQuote --> LineComment (drop character)
      &dropCharacter,
      //Exp --> LineComment (drop character)
      &dropCharacter,
      //Alpha --> LineComment (drop character)
      &dropCharacter,
      //Underscore --> LineComment (drop character)
      &dropCharacter,
      //HexX --> LineComment (drop character)
      &dropCharacter,
      //BinB --> LineComment (drop character)
      &dropCharacter,
      //BNum --> LineComment (drop character)
      &dropCharacter,
      //ONum --> LineComment (drop character)
      &dropCharacter,
      //DNum --> LineComment (drop character)
      &dropCharacter,
      //HNum --> LineComment (drop character)
      &dropCharacter,
      //Dot --> LineComment (drop character)
      &dropCharacter,
      //Plus --> LineComment (drop character)
      &dropCharacter,
      //Minus --> LineComment (drop character)
      &dropCharacter,
      //OParen --> LineComment (drop character)
      &dropCharacter,
      //CParen --> LineComment (drop character)
      &dropCharacter,
      //Lt -- LineComment (drop character)
      &dropCharacter,
      //Eq -- LineComment (drop character)
      &dropCharacter,
      //Gt -- LineComment (drop character)
      &dropCharacter,
      //Comma --> LineComment (drop character)
      &dropCharacter,
      //Hash --> LineComment (drop character)
      &dropCharacter,
      //Ampersand --> LineComment (drop character)
      &dropCharacter,
      //Pipe --> LineComment (drop character)
      &dropCharacter,
      //HighBit --> LineComment (drop character)
      &dropCharacter,
      //EOF --> EndOfFile (pass EOF)
      &actionMaybeLineEnd,
      //Unknown --> LineComment (drop character)
      &dropCharacter
    },
    //BlockComment State
    {
      //Whitespace --> BlockComment (drop character)
      &dropCharacter,
      //LineEnd --> BlockComment (drop character)
      &dropCharacter,
      //FSlash --> BlockComment (drop character)
      &dropCharacter,
      //BSlash --> BlockComment (drop character)
      &dropCharacter,
      //Star --> MaybeEndComment (drop character)
      &actionMaybeEndComment,
      //Bang --> BlockComment (drop character)
      &dropCharacter,
      //DoubleQuote --> BlockComment (drop character)
      &dropCharacter,
      //Exp --> BlockComment (drop character)
      &dropCharacter,
      //Alpha --> BlockComment (drop character)
      &dropCharacter,
      //Underscore --> BlockComment (drop character)
      &dropCharacter,
      //HexX --> BlockComment (drop character)
      &dropCharacter,
      //BinB --> BlockComment (drop character)
      &dropCharacter,
      //BNum --> BlockComment (drop character)
      &dropCharacter,
      //ONum --> BlockComment (drop character)
      &dropCharacter,
      //DNum --> BlockComment (drop character)
      &dropCharacter,
      //HNum --> BlockComment (drop character)
      &dropCharacter,
      //Dot --> BlockComment (drop character)
      &dropCharacter,
      //Plus --> BlockComment (drop character)
      &dropCharacter,
      //Minus --> BlockComment (drop character)
      &dropCharacter,
      //OParen --> BlockComment (drop character)
      &dropCharacter,
      //CParen --> BlockComment (drop character)
      &dropCharacter,
      //Lt -- BlockComment (drop character)
      &dropCharacter,
      //Eq -- BlockComment (drop character)
      &dropCharacter,
      //Gt -- BlockComment (drop character)
      &dropCharacter,
      //Comma --> BlockComment (drop character)
      &dropCharacter,
      //Hash --> BlockComment (drop character)
      &dropCharacter,
      //Ampersand --> BlockComment (drop character)
      &dropCharacter,
      //Pipe --> BlockComment (drop character)
      &dropCharacter,
      //HighBit --> BlockComment (drop character)
      &dropCharacter,
      //EOF --> EndOfFile (pass EOF)
      &actionMaybeLineEnd,
      //Unknown --> BlockComment (drop character)
      &dropCharacter
    },
    //MaybeEndComment State
    {
      //Whitespace --> BlockComment (drop character)
      &actionBlockComment,
      //LineEnd --> BlockComment (drop character)
      &actionBlockComment,
      //FSlash --> InSpace (drop character)
      &actionBeginWhitespace,
      //BSlash --> BlockComment (drop character)
      &actionBlockComment,
      //Star --> BlockComment (drop character)
      &dropCharacter,
      //Bang --> BlockComment (drop character)
      &actionBlockComment,
      //DoubleQuote --> BlockComment (drop character)
      &actionBlockComment,
      //Exp --> BlockComment (drop character)
      &actionBlockComment,
      //Alpha --> BlockComment (drop character)
      &actionBlockComment,
      //Underscore --> BlockComment (drop character)
      &actionBlockComment,
      //HexX --> BlockComment (drop character)
      &actionBlockComment,
      //BinB --> BlockComment (drop character)
      &actionBlockComment,
      //BNum --> BlockComment (drop character)
      &actionBlockComment,
      //ONum --> BlockComment (drop character)
      &actionBlockComment,
      //DNum --> BlockComment (drop character)
      &actionBlockComment,
      //HNum --> BlockComment (drop character)
      &actionBlockComment,
      //Dot --> BlockComment (drop character)
      &actionBlockComment,
      //Plus --> BlockComment (drop character)
      &actionBlockComment,
      //Minus --> BlockComment (drop character)
      &actionBlockComment,
      //OParen --> BlockComment (drop character)
      &actionBlockComment,
      //CParen --> BlockComment (drop character)
      &actionBlockComment,
      //Lt -- BlockComment (drop character)
      &actionBlockComment,
      //Eq -- BlockComment (drop character)
      &actionBlockComment,
      //Gt -- BlockComment (drop character)
      &actionBlockComment,
      //Comma --> BlockComment (drop character)
      &actionBlockComment,
      //Hash --> BlockComment (drop character)
      &actionBlockComment,
      //Ampersand --> BlockComment (drop character)
      &actionBlockComment,
      //Pipe --> BlockComment (drop character)
      &actionBlockComment,
      //HighBit --> BlockComment (drop character)
      &actionBlockComment,
      //EOF --> EndOfFile (pass EOF)
      &actionMaybeLineEnd,
      //Unknown --> BlockComment (drop character)
      &actionBlockComment
    },
    //MaybeEscapeNewline State
    {
      //Whitespace --> InSpace (pass backslash and enter InSpace)
      &actionEscapeToInSpace,
      //LineEnd --> InSpace (drop backslash and newline)
      &actionBeginWhitespace,
      //FSlash --> Init (pass backslash and character)
      &actionEscapeToMaybeComment,
      //BSlash --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Star --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Bang --> Init (pass backslash and character)
      &actionEscapeToInit,
      //DoubleQuote --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Exp --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Alpha --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Underscore --> Init (pass backslash and character)
      &actionEscapeToInit,
      //HexX --> Init (pass backslash and character)
      &actionEscapeToInit,
      //BinB --> Init (pass backslash and character)
      &actionEscapeToInit,
      //BNum --> Init (pass backslash and character)
      &actionEscapeToInit,
      //ONum --> Init (pass backslash and character)
      &actionEscapeToInit,
      //DNum --> Init (pass backslash and character)
      &actionEscapeToInit,
      //HNum --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Dot --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Plus --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Minus --> Init (pass backslash and character)
      &actionEscapeToInit,
      //OParen --> Init (pass backslash and character)
      &actionEscapeToInit,
      //CParen --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Lt -- Init (pass backslash and character)
      &actionEscapeToInit,
      //Eq -- Init (pass backslash and character)
      &actionEscapeToInit,
      //Gt -- Init (pass backslash and character)
      &actionEscapeToInit,
      //Comma --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Hash --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Ampersand --> Init (pass backslash and character)
      &actionEscapeToInit,
      //Ampersand --> Init (pass backslash and character)
      &actionEscapeToInit,
      //HighBit --> Init (pass backslash and character)
      &actionEscapeToInit,
      //EOF --> Init (pass backslash and character)
      &actionEscapeToEOFNewline,
      //Unknown --> Init (pass backslash and character)
      &actionEscapeToInit
    },
    //EndOfFile State
    {
      //Whitespace --> EndOfFile (EOF)
      &passCharacter,
      //LineEnd --> EndOfFile (EOF)
      &passCharacter,
      //FSlash --> EndOfFile (EOF)
      &passCharacter,
      //BSlash --> EndOfFile (EOF)
      &passCharacter,
      //Star --> EndOfFile (EOF)
      &passCharacter,
      //Bang --> EndOfFile (EOF)
      &passCharacter,
      //DoubleQuote --> EndOfFile (EOF)
      &passCharacter,
      //Exp --> EndOfFile (EOF)
      &passCharacter,
      //Alpha --> EndOfFile (EOF)
      &passCharacter,
      //Underscore --> EndOfFile (EOF)
      &passCharacter,
      //HexX --> EndOfFile (EOF)
      &passCharacter,
      //BinB --> EndOfFile (EOF)
      &passCharacter,
      //BNum --> EndOfFile (EOF)
      &passCharacter,
      //ONum --> EndOfFile (EOF)
      &passCharacter,
      //DNum --> EndOfFile (EOF)
      &passCharacter,
      //HNum --> EndOfFile (EOF)
      &passCharacter,
      //Dot --> EndOfFile (EOF)
      &passCharacter,
      //Plus --> EndOfFile (EOF)
      &passCharacter,
      //Minus --> EndOfFile (EOF)
      &passCharacter,
      //OParen --> EndOfFile (EOF)
      &passCharacter,
      //CParen --> EndOfFile (EOF)
      &passCharacter,
      //Lt -- EndOfFile (EOF)
      &passCharacter,
      //Eq -- EndOfFile (EOF)
      &passCharacter,
      //Gt -- EndOfFile (EOF)
      &passCharacter,
      //Comma --> EndOfFile (EOF)
      &passCharacter,
      //Hash --> EndOfFile (EOF)
      &passCharacter,
      //Ampersand --> EndOfFile (EOF)
      &passCharacter,
      //Ampersand --> EndOfFile (EOF)
      &passCharacter,
      //HighBit --> EndOfFile (EOF)
      &passCharacter,
      //EOF --> EndOfFile (EOF)
      &passCharacter,
      //Unknown --> EndOfFile (EOF)
      &passCharacter
    }
};

/**
 * Filter a character using the current input state.
 *
 * \param impl      The whitespace filter impl structure.
 * \param ch        The character pointer, containing the initial character to
 *                  filter.  This will be updated with the output character.
 *
 * \note This method may overwrite the cached character in the impl structure.
 * The caller should ensure that no characters are cached before continuing.
 */
static bool filter(WhitespaceFilterImplementation* impl, int* ch)
{
    //get the input category for this byte.
    PCat cat = character2PCat(*ch);

    return (filterMachine[impl->inputState][pcat2index(cat)])(impl, ch);
}

/**
 * Compute the line/column for a cached character.
 */
static void
computeCachedCharLineCol(WhitespaceFilterImplementation* impl, int ch)
{
    if (impl->cachedChar == '\n')
    {
        ++impl->line;
        impl->column = 0;
    }
    else if (impl->cachedChar != EOF)
    {
        ++impl->column;
    }
}

/**
 * Cache the previous line / column information.
 */
static void
cacheCharLineCol(WhitespaceFilterImplementation* impl, int ch)
{
    impl->column = impl->in_.columnNumber();
    impl->line   = impl->in_.lineNumber();
}

/**
 * The get method works like istream::get(), except that white space
 * characters and comments are condensed to a single space.
 *
 * \returns a filtered input byte, or EOF on EOF.
 */
int WhitespaceFilter::get()
{
    int ch;

    do {
        //if we have a cached character, return this first
        if (impl_->haveCachedChar)
        {
            impl_->haveCachedChar = false;

            computeCachedCharLineCol(impl_.get(), ch);

            return impl_->cachedChar;
        }
        else
        {
            //save the previous line / column information
            cacheCharLineCol(impl_.get(), ch);

            //get the next character
            ch = impl_->in_.get();
        }
    } while (filter(impl_.get(), &ch));

    return ch;
}

/**
 * Transition to the InSpace state.
 */
static bool actionBeginWhitespace(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::InSpace);

    return true;
}

/**
 * Transition to BlockComment.
 */
static bool actionBlockComment(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::BlockComment);

    return true;
}

/**
 * Transition to LineComment.
 */
static bool actionLineComment(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::LineComment);

    return true;
}

/**
 * Transition to the MaybeComment state.
 */
static bool actionMaybeComment(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::MaybeComment);

    return true;
}

/**
 * EOF has been encountered.  Check to see whether we should pass end-of-line,
 * and if so, do so.  Either way, transition to EndOfFile.
 */
static bool actionMaybeLineEnd(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->haveCachedChar = true;
    impl->cachedChar = *ch;
    impl->isEof = true;
    *ch = '\n';

    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::EndOfFile);

    //update line / column
    impl->line++;
    impl->column = 0;

    return false;
}

/**
 * Transition to the MaybeEndComment state.
 */
static bool actionMaybeEndComment(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::MaybeEndComment);

    return true;
}

/**
 * Transition to the MaybeEscapeNewline state.
 */
static bool actionMaybeEscapeNewline(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(
            WhitespaceFilterState::MaybeEscapeNewline);

    return true;
}

/**
 * Pass the backslash character and enter the MaybeComment state.
 */
static bool actionEscapeToMaybeComment(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(
            WhitespaceFilterState::MaybeComment);
    *ch = '\\';

    return false;
}

/**
 * Pass the backslash character and enter the InSpace state.
 */
static bool actionEscapeToInSpace(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(
            WhitespaceFilterState::InSpace);
    *ch = '\\';

    return false;
}

/**
 * Pass the backslash character, cache the current character, and enter the Init
 * state.
 */
static bool actionEscapeToInit(
    WhitespaceFilterImplementation* impl, int* ch)
{
    //cache the current character
    impl->cachedChar = *ch;
    impl->haveCachedChar = true;
    *ch = '\\';

    impl->inputState =
        whitespaceFilterState2index(
            WhitespaceFilterState::Init);

    return false;
}

/**
 * Pass the backslash character and go back to Init.
 */
static bool actionEscapeToEOFNewline(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(
            WhitespaceFilterState::Init);
    *ch = '\\';

    return false;
}

/**
 * Drop the current character.
 */
static bool dropCharacter(WhitespaceFilterImplementation*, int*)
{
    return true;
}

/**
 * Return the current character to the caller.
 */
static bool passCharacter(WhitespaceFilterImplementation* impl, int*)
{
    impl->line   = impl->in_.lineNumber();
    impl->column = impl->in_.columnNumber();

    return false;
}

/**
 * Return the current character to the caller and switch back to Init.
 */
static bool passCharacterToInit(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::Init);

    return false;
}

/**
 * Set up for end-of-file.
 */
static bool passSpaceToEndOfFile(
    WhitespaceFilterImplementation* impl, int* ch)
{
    //we can omit the final space.
    impl->cachedChar = EOF;
    impl->haveCachedChar = true;
    impl->isEof = true;
    *ch = '\n';

    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::EndOfFile);

    return false;
}

/**
 * Return a space character to the caller and transition to Init.
 */
static bool passSpaceToInit(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->cachedChar = *ch;
    impl->haveCachedChar = true;
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::Init);
    *ch = ' ';

    //fix up line / column
    impl->line   = impl->in_.lineNumber();
    impl->column = impl->in_.columnNumber()-1;

    return false;
}

/**
 * Return a slash character to the caller and set up end-of-file state.
 */
static bool passSlashToEndOfFile(
    WhitespaceFilterImplementation* impl, int* ch)
{
    impl->haveCachedChar = true;
    impl->cachedChar = '\n';
    *ch = '/';
    impl->isEof = true;

    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::EndOfFile);

    return false;
}

/**
 * Return a slash character to the caller and transition to Init.
 */
static bool passSlashToInit(WhitespaceFilterImplementation* impl, int* ch)
{
    impl->cachedChar = *ch;
    impl->haveCachedChar = true;
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::Init);
    *ch = '/';

    return false;
}

/**
 * Return a slash character to the caller and transition to InSpace.
 */
static bool passSlashToInSpace(WhitespaceFilterImplementation* impl, int* ch)
{
    *ch = '/';
    impl->inputState =
        whitespaceFilterState2index(WhitespaceFilterState::InSpace);

    return false;
}
