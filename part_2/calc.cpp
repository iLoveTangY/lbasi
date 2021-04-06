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
#include <unordered_set>

// #define DEBUG

#define INVALID_CHAR 0
enum TokenType { INTEGER, PLUS, MINUS, MUL, DIV, END, INVALID };

class Token {
    friend std::ostream &operator<<(std::ostream &, const Token &);
    friend class Interpreter;

   private:
    TokenType _type;
    int _value;

   public:
    Token() = default;
    explicit Token(TokenType type, int value = std::numeric_limits<int>::infinity()) : _type(type), _value(value) {}
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
            _current_char = INVALID_CHAR;
        } else {
            _current_char = _text[_pos];
        }
    }

    // 跳过空白字符
    void skipWhitespace() {
        while (_current_char != INVALID_CHAR && isspace(_current_char)) {
            advance();
        }
    }

    // 从输入中获取一个整数（可能有多位）
    int integer() {
        std::string result = "";
        while (_current_char != INVALID_CHAR && isdigit(_current_char)) {
            result.append({_current_char});
            advance();
        }
        return std::stoi(result);
    }

    // 一个简单的词法分析器（lexer）
    Token getNextToken() {
        while (_current_char != INVALID_CHAR) {
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
    explicit Interpreter(std::string text) : _text(std::move(text)), _current_char(_text[0]) {}

    // 语法分析器（parser）仅能处理正整数的加减法
    double expr() {
        double result = 0;
        TokenType last_token_type = INVALID;
        while ((_current_token = getNextToken())._type != END) {
            switch (_current_token._type) {
                case INTEGER:
                    if (last_token_type == INVALID) {
                        result = _current_token._value;
                    } else if (last_token_type == PLUS) {
                        result += _current_token._value;
                    } else if (last_token_type == MINUS) {
                        result -= _current_token._value;
                    } else {
                        throwError();
                    }
                    break;
                case PLUS:
                case MINUS:
                    last_token_type = _current_token._type;
                    break;
                default:
                    throwError();
                    break;
            }
        }
        return result;
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
