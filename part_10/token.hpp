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
    END_OF_FILE,  // 结束
    LP,           // "("
    RP,           // ")"
    PROGRAM,      // "PROGRAM"
    VAR,          // TODO
    COLON,        // ":"
    COMMA,        // ","
    REAL,         // 实数类型
    INTEGER_CONST, // 整数常量，例如3，5等
    REAL_CONST,   // 实属常量，例如3.14等
    INTEGER_DIV,  // 整数除法（DIV关键字）
    FLOAT_DIV,    // 浮点除法（/）
};

std::ostream &operator<<(std::ostream &out, const TokenType &type);

class Token {
   public:
    TokenType type_;
    int value_ = std::numeric_limits<int>::infinity();
    float float_value_ = std::numeric_limits<float>::infinity();
    std::string str_;

    Token() = default;
    Token(TokenType type, float value) : type_(type), float_value_(value) {}
    Token(TokenType type, int value) : type_(type), value_(value) {}
    Token(TokenType type, std::string str) : type_(type), str_(std::move(str)) {}
};

std::ostream &operator<<(std::ostream &out, const Token &token);

#endif
