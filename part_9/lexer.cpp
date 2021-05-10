#include "lexer.hpp"

const std::unordered_map<std::string, Token> Lexer::reserved_keywords = {{"BEGIN", Token(BEGIN, "BEGIN")},
                                                                         {"END", Token(END, "END")}};
// 更新 current char
void Lexer::advance(unsigned int step) {
    pos_ += step;
    if (pos_ >= text_.size()) {
        current_char_ = INVALID_CHAR;
    } else {
        current_char_ = text_[pos_];
    }
}

// 跳过空白字符
void Lexer::skipWhitespace() {
    while (current_char_ != INVALID_CHAR && isspace(current_char_)) {
        advance();
    }
}

Token Lexer::id() {
    std::string result = "";
    while (isalnum(current_char_)) {
        result += current_char_;
        advance();
    }
    auto it = reserved_keywords.find(result);
    if (it != reserved_keywords.end()) {
        return it->second;
    }
    return Token(ID, result);
}

// 从输入中获取一个整数（可能有多位）
int Lexer::integer() {
    int result = 0;
    while (current_char_ != INVALID_CHAR && isdigit(current_char_)) {
        result = result * 10 + current_char_ - '0';
        advance();
    }
    return result;
}

char Lexer::peek() {
    auto peek_pos = pos_ + 1;
    if (peek_pos > text_.size() + 1) {
        return INVALID_CHAR;
    } else {
        return text_[peek_pos];
    }
}

// 一个简单的词法分析器（lexer）
Token Lexer::getNextToken() {
    while (current_char_ != INVALID_CHAR) {
        if (isspace(current_char_)) {
            skipWhitespace();
            continue;
        }
        if (isalpha(current_char_)) {
            return id();
        }
        if (current_char_ == ':' && peek() == '=') {
            advance(2);
            return Token(ASSIGN, ":=");
        }
        if (current_char_ == ';') {
            advance();
            return Token(SEMI, ";");
        }
        if (current_char_ == '.') {
            advance();
            return Token(DOT, ".");
        }
        if (isdigit(current_char_)) {
            return Token(INTEGER, integer());
        }
        if (current_char_ == '+') {
            advance();
            return Token(PLUS, "+");
        }
        if (current_char_ == '-') {
            advance();
            return Token(MINUS, "-");
        }
        if (current_char_ == '*') {
            advance();
            return Token(MUL, "*");
        }
        if (current_char_ == '/') {
            advance();
            return Token(DIV, "/");
        }
        if (current_char_ == '(') {
            advance();
            return Token(LP, "(");
        }
        if (current_char_ == ')') {
            advance();
            return Token(RP, ")");
        }

        throwError(std::string("Syntax error: not suport char ") + (current_char_));
    }
    return Token(END_OF_FILE, "EOF");
}