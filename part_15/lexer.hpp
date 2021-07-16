#ifndef LEXER_HPP
#define LEXER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ast.hpp"
#include "error.hpp"
#include "token.hpp"

class Lexer {
   public:
    explicit Lexer(std::string text) : text_(std::move(text)) {
        pos_ = 0;
        current_char_ = text_[pos_];
    }

   private:
    static const std::unordered_map<std::string, Token> reserved_keywords;
    std::string text_;
    size_t pos_ = 0;
    char current_char_;
    int lineno_ = 1;
    int column_ = 1;

    inline void throwError(const std::string &msg) {
        std::string s = "Lexer error on '" + std::string(1, current_char_) + "' line: " + std::to_string(lineno_) +
                        " column: " + std::to_string(column_);

        throw LexerError(s);
    }

    // 更新 current char
    inline void advance(unsigned int step = 1);

    // 跳过注释
    void skipComment();

    // 跳过空白字符
    void skipWhitespace();

    Token id();

    // 从输入中获取一个实数或者整数（可能有多位）
    Token number();

    char peek();

   public:
    // 一个简单的词法分析器（lexer）
    Token getNextToken();
};

#endif
