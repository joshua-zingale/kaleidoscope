#include "lexer.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>

int main() {
    // Write test input to a temporary file
    std::ofstream temp_file("temp_input.txt");
    temp_file << "hello 12.5 def foobar( 1, hello) extern +-*/";
    temp_file.close();

    // Redirect stdin to read from the temporary file
    FILE* fp = nullptr;
    if (freopen_s(&fp, "temp_input.txt", "r", stdin) != 0) {
        std::cerr << "Failed to redirect stdin." << std::endl;
        return 1;
    }

    Token token_sequence[] = {
        Token::tok_identifier,
        Token::tok_number,
        Token::tok_def,
        Token::tok_identifier,
        Token::tok_lparen,
        Token::tok_number,
        Token::tok_comma,
        Token::tok_identifier,
        Token::tok_rparen,
        Token::tok_extern,
        Token::tok_plus,
        Token::tok_minus,
        Token::tok_mul,
        Token::tok_div,
        Token::tok_eof
    };

    for (int i = 0; i < sizeof(token_sequence)/sizeof(token_sequence[0]); ++i) {
        auto received_token = gettok();
        if (received_token != token_sequence[i]) {
            std::cerr << "Token mismatch at index " << i << ". Test failed." << std::endl;
        }
    }

    std::cout << "Test completed." << std::endl;
    std::remove("temp_input.txt");
}