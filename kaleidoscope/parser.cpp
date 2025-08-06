#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>
#include <vector>



std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}

std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}



static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();
    auto V = ParseExpression();
    if (!V) return nullptr;
    if (CurTok != Token::tok_rparen) return LogError("Expected )");
    getNextToken();
    return V;
}

static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = IdentifierStr;
    getNextToken();

    if (CurTok != Token::tok_lparen) return std::make_unique<VariableExpAST>(IdName);

    getNextToken();

    std::vector<std::unique_ptr<ExprAST>> Args;

    if (CurTok != Token::tok_rparen) {
        while (true) {
            if (auto Arg = ParseExpression()) {
                Args.push_back(std::move(Arg));
            } else {
                return nullptr;
            }

            if (CurTok == Token::tok_rparen) break;

            if (CurTok != Token::tok_comma) {
                return LogError("Expected ')' or ',' in argument list.");
            }
            getNextToken();
        }
    }

    getNextToken();

    return std::make_unique<CallExprAST>(IdName, Args);
}

static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
        default:
            return LogError("unknown token when expecting an expression.");
        case Token::tok_identifier:
            return ParseIdentifierExpr();
        case Token::tok_number:
            return ParseNumberExpr();
        case Token::tok_lparen:
            return ParseParenExpr();
    }
}

static std::unique_ptr<ExprAST> ParseExpression();