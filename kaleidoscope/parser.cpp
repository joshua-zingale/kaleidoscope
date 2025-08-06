#include "parser.hpp"
#include "lexer.hpp"
#include <iostream>
#include <vector>


#include <array>

static constexpr std::array<std::pair<Token, int>, 4> BinopPrecedence = {{
    { Token::tok_less_than, 10 },
    { Token::tok_plus, 20 },
    { Token::tok_minus, 20 },
    { Token::tok_mul, 40 }
}};

static int GetTokPrecedence() {
    for (const auto& entry : BinopPrecedence) {
        if (entry.first == CurTok) {
            return entry.second;
        }
    }
    return -1;
}


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

static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS) return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}


static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS) {
    while (true) {
        int TokPrec = GetTokPrecedence();

        if (TokPrec < ExprPrec) return LHS;
        Token BinOp = CurTok;
        getNextToken();
        auto RHS = ParsePrimary();
        if (!RHS) return nullptr;

        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS) return nullptr;
        }

        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));

    }
}

static std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != Token::tok_identifier) {
        return LogErrorP("Expected function name in prototype.");
    }

    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != Token::tok_lparen) {
        return LogErrorP("Exoected ')' in prototype");
    }

    std::vector<std::string> Parameters;
    while (getNextToken() == Token::tok_identifier) {
        Parameters.push_back(IdentifierStr);
        if (CurTok != Token::tok_rparen) {
            return LogErrorP("Expected ')' in prototype");
        }
    }
    return std::make_unique<PrototypeAST>(FnName, IdentifierStr);
}