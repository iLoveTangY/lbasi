#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <ostream>
#include <string>
#include <limits>

#define INVALID_CHAR 0
enum TokenType {
    BEGIN,        // "BEGIN"
    END,          // "END"
    DOT,          // "."
    ASSIGN,       // ":="
    SEMI,         // ";"
    ID,           // identifier
    INTEGER,      // 正整数
    PLUS,         // "+"
    MINUS,        // "-"
    MUL,          // "*"
    DIV,          // "/"
    END_OF_FILE,  // 结束
    LP,           // "("
    RP,           // ")"
};

std::ostream &operator<<(std::ostream &out, const TokenType &type);

class Token {
   public:
    TokenType type_;
    int value_ = std::numeric_limits<int>::infinity();
    std::string str_;

    Token() = default;
    Token(TokenType type, int value) : type_(type), value_(value) {}
    Token(TokenType type, std::string str) : type_(type), str_(std::move(str)) {}
};

std::ostream &operator<<(std::ostream &out, const Token &token);

#endif
