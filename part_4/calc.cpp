/**
 * 1. 写出支持任意数量 + - * / 表达式的上下文无关语法
 *      expr : addtion-expr
 *      addition-expr : muliplication-expr ((PLUS | MINUS) mulitiplication-expr)
 *      multiplication-expr : factor ((MUL | DIV) factor)*
 *      factor : INTEGER
 * 2. 用代码实现
 */
#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include <limits>

// #define DEBUG

#define INVALID_CHAR 0
enum TokenType { INTEGER, PLUS, MINUS, MUL, DIV, END, INVALID };

class Token {
    friend std::ostream &operator<<(std::ostream &, const Token &);
    friend class Interpreter;

   private:
    TokenType type_;
    int value_;

   public:
    Token() = default;
    explicit Token(TokenType type, int value = std::numeric_limits<int>::infinity()) : type_(type), value_(value) {}
};

std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << "Token(" << token.type_ << ", " << token.value_ << ")";
    return out;
}

// 词法分析器
class Lexer {
   public:
    explicit Lexer(std::string text) : text_(std::move(text)) {
        pos_ = 0;
        current_char_ = text_[pos_];
    }

   private:
    std::string text_;
    size_t pos_ = 0;
    char current_char_;

    inline void throwError(const std::string &msg) { throw std::runtime_error(msg); }

    // 更新 current char
    inline void advance() {
        pos_++;
        if (pos_ >= text_.size()) {
            current_char_ = INVALID_CHAR;
        } else {
            current_char_ = text_[pos_];
        }
    }

    // 跳过空白字符
    void skipWhitespace() {
        while (current_char_ != INVALID_CHAR && isspace(current_char_)) {
            advance();
        }
    }

    // 从输入中获取一个整数（可能有多位）
    int integer() {
        int result = 0;
        while (current_char_ != INVALID_CHAR && isdigit(current_char_)) {
            result = result * 10 + current_char_ - '0';
            advance();
        }
        return result;
    }

   public:
    // 一个简单的词法分析器（lexer）
    Token getNextToken() {
        while (current_char_ != INVALID_CHAR) {
            if (isspace(current_char_)) {
                skipWhitespace();
                continue;
            }
            if (isdigit(current_char_)) {
                return Token(INTEGER, integer());
            }
            if (current_char_ == '+') {
                advance();
                return Token(PLUS);
            }
            if (current_char_ == '-') {
                advance();
                return Token(MINUS);
            }
            if (current_char_ == '*') {
                advance();
                return Token(MUL);
            }
            if (current_char_ == '/') {
                advance();
                return Token(DIV);
            }

            throwError(std::string("Syntax error: not suport char ") + (current_char_));
        }
        return Token(END);
    }
};

class Interpreter {
   private:
    Token current_token_;
    Lexer lexer_;

   public:
    explicit Interpreter(Lexer lexer) : lexer_(lexer) { current_token_ = lexer_.getNextToken(); }

   private:
#define THROW_ERROR throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": Ivalid syntax")

    // 返回当前 token 的 int 值
    // factor : INTEGER
    int factor() {
        auto token = current_token_;
        eatToken(INTEGER);
        return token.value_;
    }

    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const TokenType &type) {
        if (current_token_.type_ == type) {
            current_token_ = lexer_.getNextToken();
            return;
        }
        THROW_ERROR;
    }

   public:
    // 加减乘除计算器
    // expr : addtion-expr
    // addition-expr : muliplication-expr ((PLUS | MINUS) mulitiplication-expr)
    // multiplication-expr : factor ((MUL | DIV) factor)*
    // factor : INTEGER 
    double expr() {
        return additon_expr();
    }

    double additon_expr() {
        double result =muliplication_expr();
        while (current_token_.type_ == PLUS || current_token_.type_ == MINUS) {
            auto token = current_token_;
            if (token.type_ == PLUS) {
                eatToken(PLUS);
                result += muliplication_expr();
            } else if (token.type_ == MINUS) {
                eatToken(MINUS);
                result -= muliplication_expr();
            } else {
                THROW_ERROR;
            }
        }
        return result;
    }

    double muliplication_expr() {
        double result = factor();
        while (current_token_.type_ == MUL || current_token_.type_ == DIV) {
            auto token = current_token_;
            if (token.type_ == MUL) {
                eatToken(MUL);
                result *= factor();
            } else if (token.type_ == DIV) {
                eatToken(DIV);
                result /= factor();
            } else {
                THROW_ERROR;
            }
        }
        return result;
    }
#undef THROW_ERROR
};

int main() {
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text == "exit" || text == "q") {
            break;
        }
        auto lexer = Lexer(text);
        auto interpreter = Interpreter(lexer);
        std::cout << interpreter.expr() << std::endl;
        std::cout << "calc> ";
    }

    return 0;
}
