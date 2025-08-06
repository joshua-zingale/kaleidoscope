#ifndef KALAEIDOSCOPE_LEXER_HPP
#define KALAEIDOSCOPE_LEXER_HPP

#include <iostream>

enum class Token {
    tok_eof,

    tok_def,
    tok_extern,

    tok_identifier,
    tok_number,

    tok_plus,
    tok_minus,
    tok_mul,
    tok_div,
    tok_equal,
    tok_less_than,

    tok_lparen,
    tok_rparen,
    
    tok_comma,
};

/// Gets the next token
Token gettok();

/// Updates CurTok to the next token
Token getNextToken();

extern std::string IdentifierStr;
extern double NumVal;
extern Token CurTok;


#endif