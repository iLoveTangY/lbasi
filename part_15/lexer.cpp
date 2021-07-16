#include "lexer.hpp"

#include <algorithm>

static std::string toUpper(std::string str) {
    std::string ret = std::move(str);
    std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
}

// pascal 中的所有关键字
const std::unordered_map<std::string, Token> Lexer::reserved_keywords = {
    {"BEGIN", Token(BEGIN, "BEGIN")},       {"END", Token(END, "END")},
    {"PROGRAM", Token(PROGRAM, "PROGRAM")}, {"VAR", Token(VAR, "VAR")},
    {"DIV", Token(INTEGER_DIV, "DIV")},     {"INTEGER", Token(INTEGER, "INTEGER")},
    {"REAL", Token(REAL, "REAL")},          {"PROCEDURE", Token(PROCEDURE, "PROCEDURE")},
};
// 更新 current char
void Lexer::advance(unsigned int step) {
    if (current_char_ == '\n') {
        lineno_ += 1;
        column_ = 0;
    }
    pos_ += step;
    if (pos_ >= text_.size()) {
        current_char_ = INVALID_CHAR;
    } else {
        current_char_ = text_[pos_];
        column_ += 1;
    }
}

void Lexer::skipComment() {
    while (current_char_ != '}') {
        advance();
    }
    advance();  // 跳过最后的花括号
}

// 跳过空白字符
void Lexer::skipWhitespace() {
    while (current_char_ != INVALID_CHAR && isspace(current_char_)) {
        advance();
    }
}

Token Lexer::id() {
    std::string result = "";
    // 数字、字母、下划线
    while (isalnum(current_char_) || '_' == current_char_) {
        result += current_char_;
        advance();
    }
    // 大小写不敏感
    result = toUpper(result);
    auto it = reserved_keywords.find(result);
    if (it != reserved_keywords.end()) {
        return it->second;
    }
    return Token(ID, result, lineno_, column_);
}

Token Lexer::number() {
    std::string result = "";
    while (current_char_ != INVALID_CHAR && isdigit(current_char_)) {
        result += current_char_;
        advance();
    }
    if (current_char_ == '.') {
        result += current_char_;
        advance();

        while (current_char_ != INVALID_CHAR && isdigit(current_char_)) {
            result += current_char_;
            advance();
        }
        return Token(REAL_CONST, std::stof(result), lineno_, column_);
    } else {
        return Token(INTEGER_CONST, std::stoi(result), lineno_, column_);
    }
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
        if (current_char_ == '{') {
            advance();
            skipComment();
            continue;
        }
        if (isalpha(current_char_) || '_' == current_char_) {
            return id();
        }
        if (current_char_ == ':' && peek() == '=') {
            advance(2);
            return Token(ASSIGN, ":=", lineno_, column_);
        }
        // COLON 必须在 ASSIGN 之后判断
        if (current_char_ == ':') {
            advance();
            return Token(COLON, ":", lineno_, column_);
        }
        if (current_char_ == ',') {
            advance();
            return Token(COMMA, ",", lineno_, column_);
        }
        if (current_char_ == ';') {
            advance();
            return Token(SEMI, ";", lineno_, column_);
        }
        if (current_char_ == '.') {
            advance();
            return Token(DOT, ".", lineno_, column_);
        }
        if (isdigit(current_char_)) {
            return number();
        }
        if (current_char_ == '+') {
            advance();
            return Token(PLUS, "+", lineno_, column_);
        }
        if (current_char_ == '-') {
            advance();
            return Token(MINUS, "-", lineno_, column_);
        }
        if (current_char_ == '*') {
            advance();
            return Token(MUL, "*", lineno_, column_);
        }
        if (current_char_ == '/') {
            advance();
            return Token(FLOAT_DIV, "/", lineno_, column_);
        }
        if (current_char_ == '(') {
            advance();
            return Token(LP, "(", lineno_, column_);
        }
        if (current_char_ == ')') {
            advance();
            return Token(RP, ")", lineno_, column_);
        }

        throwError(std::string("Syntax error: not suport char ") + (current_char_));
    }
    return Token(END_OF_FILE, "EOF");
}