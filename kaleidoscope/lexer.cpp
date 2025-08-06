#include <lexer.hpp>
#include <iostream>


static Token getNextToken() {
  return CurTok = gettok();
}


Token gettok() {
    static int LastChar = ' ';

    while (isspace(LastChar))
        LastChar = getchar();

    if (isalpha(LastChar = getchar())) {
        IdentifierStr = LastChar;
        while (isalnum(LastChar = getchar()))
            IdentifierStr += LastChar;


        if (IdentifierStr == "def")
            return Token::tok_def;
        if (IdentifierStr == "extern")
            return Token::tok_extern;
        return Token::tok_identifier;
    }

    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return Token::tok_number;
    }

    if (LastChar == '#') {
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    if (LastChar == EOF)
        return Token::tok_eof;
    
    int ThisChar = LastChar;
    LastChar = getchar();

    switch (LastChar) {
    case '+':
        return Token::tok_plus;
    case '-':
        return Token::tok_minus;
    case '*':
        return Token::tok_mul;
    case '/':
        return Token::tok_div;
    case '=':
        return Token::tok_equal;
    case '<':
        return Token::tok_less_than;
    case '(':
        return Token::tok_lparen;
    case ')':
        return Token::tok_rparen;
    case ',':
        return Token::tok_comma;
    }
    
    
    
    throw std::runtime_error("Couild not parse token.");
    
}