/**
 * 1. 扩展程序使之支持两个数相乘
 * 2. 扩展程序使之支持两个数相除
 * 3. 扩展程序使之支持任意数量的加法和减法表达式，例如“9 - 5 + 3 + 11 - 22”
 */
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <string>
#include <unordered_set>

// #define DEBUG

enum TokenType { INTEGER, PLUS, MINUS, MUL, DIV, END };

class Token {
    friend std::ostream &operator<<(std::ostream &, const Token &);
    friend class Interpreter;

   private:
    TokenType _type;
    int _value;

   public:
    Token() = default;
    Token(TokenType type, int value = std::numeric_limits<int>::infinity()) : _type(type), _value(value) {}
};

std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << "Token(" << token._type << ", " << token._value << ")";
    return out;
}

class Interpreter {
   private:
    std::string _text;
    size_t _pos = 0;
    Token _current_token;
    char _current_char;

    inline void throwError() { throw std::runtime_error("Error parsing input"); }

    // 更新 current char
    inline void advance() {
        _pos++;
        if (_pos >= _text.size()) {
            _current_char = NULL;
        } else {
            _current_char = _text[_pos];
        }
    }

    // 跳过空白字符
    void skipWhitespace() {
        while (_current_char != NULL && isspace(_current_char)) {
            advance();
        }
    }

    // 从输入中获取一个整数（可能有多位）
    int integer() {
        std::string result = "";
        while (_current_char != NULL && isdigit(_current_char)) {
            result.append({_current_char});
            advance();
        }
        return std::stoi(result);
    }

    // 一个简单的词法分析器（lexer）
    Token getNextToken() {
        while (_current_char != NULL) {
            if (isspace(_current_char)) {
                skipWhitespace();
                continue;
            }
            if (isdigit(_current_char)) {
                return Token(INTEGER, integer());
            }
            if (_current_char == '+') {
                advance();
                return Token(PLUS);
            }
            if (_current_char == '-') {
                advance();
                return Token(MINUS);
            }
            if (_current_char == '*') {
                advance();
                return Token(MUL);
            }
            if (_current_char == '/') {
                advance();
                return Token(DIV);
            }
            throwError();
        }
        return Token(END);
    }

    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const TokenType &type) {
        if (_current_token._type == type) {
            _current_token = getNextToken();
            return;
        }
        throwError();
    }

   public:
    Interpreter(std::string text) : _text(std::move(text)), _current_char(_text[0]) {}

    // 语法分析器（parser），仅能处理两个正整数的加减法
    double expr() {
        _current_token = getNextToken();

        auto left = _current_token;
#ifdef DEBUG
        std::cout << "left: " << left << std::endl;
#endif
        eatToken(INTEGER);

        auto op = _current_token;
#ifdef DEBUG
        std::cout << "op: " << op << std::endl;
#endif
        switch (op._type)
        {
        case PLUS:
            eatToken(PLUS);
            break;
        case MINUS:
            eatToken(MINUS);
            break;
        case MUL:
            eatToken(MUL);
            break;
        case DIV:
            eatToken(DIV);
            break;
        default:
            throwError();
            break;
        }
        auto right = _current_token;
#ifdef DEBUG
        std::cout << "right: " << right << std::endl;
#endif
        eatToken({INTEGER});
        switch (op._type)
        {
        case PLUS:
            return left._value + right._value;
            break;
        case MINUS:
            return left._value - right._value;
            break;
        case MUL:
            return 1.0 * left._value * right._value;
            break;
        case DIV:
            return 1.0 * left._value / right._value;
            break;
        default:
            throwError();
            break;
        }
        return std::numeric_limits<double>::infinity();
    }
};

int main() {
    std::string text;

    while (getline(std::cin, text)) {
        auto interpreter = Interpreter(text);
        std::cout << interpreter.expr() << std::endl;
    }

    return 0;
}
