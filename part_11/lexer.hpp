#ifndef LEXER_HPP
#define LEXER_HPP

#include "ast.hpp"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

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

    inline void throwError(const std::string &msg) { throw std::runtime_error(msg); }

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
