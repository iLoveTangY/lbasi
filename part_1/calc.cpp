#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>

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

    Token getNextToken() {
        if (_pos >= _text.size()) {
            return Token(END);
        }
        auto current_char = _text[_pos++];
        if (isdigit(current_char)) {
            return Token(INTEGER, current_char - '0');
        } else if (current_char == '+') {
            return Token(PLUS);
        }
        throwError();
        // should not be here
        return Token(END);
    }

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
