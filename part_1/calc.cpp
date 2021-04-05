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
#include <unordered_set>

enum TokenType { INTEGER, PLUS, MINUS, END };

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

    // 跳过空格
    void eatWhitespace() {
        // 跳过符号开头部分的空格
        while (_pos < _text.size() && _text[_pos] == ' ') {
            ++_pos;
        }
    }

    // 一个简单的词法分析器（lexer）
    Token getNextToken() {
        eatWhitespace();
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
        } else if (_text[start_pos] == '-') {
            ++_pos;
            return Token(MINUS);
        }
        throwError();
        // should not be here
        return Token(END);
    }

    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const std::unordered_set<TokenType> &types) {
        if (types.find(_current_token._type) != types.end()) {
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
#ifdef DEBUG
        std::cout << "left: " << left << std::endl;
#endif
        eatToken({INTEGER});

        auto op = _current_token;
#ifdef DEBUG
        std::cout << "op: " << op << std::endl;
#endif
        eatToken({PLUS, MINUS});

        auto right = _current_token;
#ifdef DEBUG
        std::cout << "right: " << right << std::endl;
#endif
        eatToken({INTEGER});
        if (op._type == PLUS) {
            return left._value + right._value;
        } else if (op._type == MINUS) {
            return left._value - right._value;
        }
        return std::numeric_limits<int>::infinity();
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
