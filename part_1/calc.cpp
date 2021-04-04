/**
 * 1. 完善程序使之支持多位数字相加，例如“12+3”
 * 2. 完善程序使之支持减法，例如“7-5”
 * 3. 添加一个处理空白字符的函数，使程序能够处理这种情况“1 + 2”
 */
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>
#include <string>

enum TokenType { INTEGER, PLUS, END };

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

    inline void throwError() { throw std::runtime_error("Error parsing input"); }

    // 一个简单的词法分析器（lexer）
    Token getNextToken() {
        if (_pos >= _text.size()) {
            return Token(END);
        }
        auto start_pos = _pos;
        while (isdigit(_text[_pos])) {
            ++_pos;
        }

        if (isdigit(_text[start_pos])) {
            auto value = std::stoi(_text.substr(start_pos, _pos - start_pos));
            return Token(INTEGER, value);
        } else if (_text[start_pos] == '+') {
            ++_pos;
            return Token(PLUS);
        }
        throwError();
        // should not be here
        return Token(END);
    }

    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const TokenType &token_type) {
        if (_current_token._type == token_type) {
            _current_token = getNextToken();
            return;
        }
        throwError();
    }

   public:
    Interpreter(std::string text) : _text(std::move(text)) {}

    // 仅仅能处理 A+B 形式的算式
    int expr() {
        _current_token = getNextToken();

        auto left = _current_token;
        eatToken(INTEGER);

        auto op = _current_token;
        eatToken(PLUS);

        auto right = _current_token;
        eatToken(INTEGER);

        return left._value + right._value;
    }
};

int main() {
    std::string text;

    while (std::cin >> text) {
        auto interpreter = Interpreter(text);
        std::cout << interpreter.expr() << std::endl;
    }

    return 0;
}
