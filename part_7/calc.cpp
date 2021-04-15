// @Author: tangyang.01@bytedance.com  
// @Date: 2021-04-15 18:31:55 

#include <iostream>
#include <memory>
#include <string>

#define INVALID_CHAR 0
enum TokenType {
    INTEGER,  // 正整数
    PLUS,     // "+"
    MINUS,    // "-"
    MUL,      // "*"
    DIV,      // "/"
    END,      // 结束
    LP,       // "("
    RP,       // ")"
};

class Token {
    friend std::ostream &operator<<(std::ostream &, const Token &);
    friend class Interpreter;

   public:
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

class BinaryOpNode;
class NumNode;

class Visitor {
public:
    virtual double visit(const std::shared_ptr<BinaryOpNode> &node) = 0;
    virtual double visit(const std::shared_ptr<NumNode> &node) = 0;
};

class ASTNode {
public:
    virtual double visit(const std::shared_ptr<Visitor> &visitor) = 0;
};

class BinaryOpNode : public ASTNode, public std::enable_shared_from_this<BinaryOpNode> {
   public:
    BinaryOpNode(std::shared_ptr<ASTNode> left, Token op, std::shared_ptr<ASTNode> right)
        : left_(std::move(left)), op_(op), right_(std::move(right)) {}

    double visit(const std::shared_ptr<Visitor> &visitor) override {
        return visitor->visit(shared_from_this());
    }

    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<ASTNode> right_;
    Token op_;
};

class NumNode : public ASTNode, public std::enable_shared_from_this<NumNode> {
   public:
    NumNode(Token token) : token_(token) { value_ = token.value_; }

    double visit(const std::shared_ptr<Visitor> &visitor) override {
        return visitor->visit(shared_from_this());
    }
    Token token_;
    int value_;
};

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
            if (current_char_ == '(') {
                advance();
                return Token(LP);
            }
            if (current_char_ == ')') {
                advance();
                return Token(RP);
            }

            throwError(std::string("Syntax error: not suport char ") + (current_char_));
        }
        return Token(END);
    }
};

// 生成一颗 AST
class Parser {
#define THROW_ERROR throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": Ivalid syntax")
public:
    explicit Parser(Lexer lexer) : lexer_(lexer) {
        current_token_ = lexer_.getNextToken();
    }

    std::shared_ptr<ASTNode> parse() {
        return expr();
    }

private:
    // 确保当前 token 的 type 为指定的 token_type，并且获取下一个 token
    void eatToken(const TokenType &type) {
        if (current_token_.type_ == type) {
            current_token_ = lexer_.getNextToken();
            return;
        }
        THROW_ERROR;
    }

    // factor: INTEGER | (LP expr RP)
    std::shared_ptr<ASTNode> factor() {
        auto token = current_token_;
        if (token.type_ == INTEGER) {
            eatToken(INTEGER);
            return std::make_shared<NumNode>(token);
        } else if (token.type_ == LP) {
            eatToken(LP);
            auto node = expr();
            eatToken(RP);
            return node;
        }
        THROW_ERROR;
        return nullptr;
    }

    // term: factor ((MUL | DIV) factor)*
    std::shared_ptr<ASTNode> term() {
        auto node = factor();
        while (current_token_.type_ == MUL || current_token_.type_ == DIV) {
            auto token = current_token_;
            if (token.type_ == MUL) {
                eatToken(MUL);
            } else if (token.type_ == DIV) {
                eatToken(DIV);
            }
            node = std::make_shared<BinaryOpNode>(node, token, factor());
        }

        return node;
    }

    // expr : term ((PLUS | MINUS) term)*
    std::shared_ptr<ASTNode> expr() {
        auto node = term();

        while (current_token_.type_ == PLUS || current_token_.type_ == MINUS) {
            auto token = current_token_;
            if (token.type_ == PLUS) {
                eatToken(PLUS);
            } else if (token.type_ == MINUS) {
                eatToken(MINUS);
            }
            node = std::make_shared<BinaryOpNode>(node, token, term());
        }
        return node;
    }

    Lexer lexer_;
    Token current_token_;
};


class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter> {
public:
    explicit Interpreter(const Parser &parser) : parser_(parser) {}

    double interpreter() {
        auto root_node = parser_.parse();
        if (nullptr == root_node) {
            return 0;
        }
        auto ptr = shared_from_this();
        return root_node->visit(ptr);
    }

    double visit(const std::shared_ptr<BinaryOpNode> &node) override {
        auto this_ptr = shared_from_this();
        if (node->op_.type_ == PLUS) {
            return (node->left_->visit(this_ptr) + node->right_->visit(this_ptr));
        } else if (node->op_.type_ == MINUS) {
            return (node->left_->visit(this_ptr) - node->right_->visit(this_ptr));
        } else if (node->op_.type_ == MUL) {
            return (node->left_->visit(this_ptr) * node->right_->visit(this_ptr));
        } else {  // DIV
            return (node->left_->visit(this_ptr) / node->right_->visit(this_ptr));
        }
    } 

    double visit(const std::shared_ptr<NumNode> &node) override {
        return node->value_;
    }

private:

    Parser parser_;
};

int main() {
    std::string text;
    std::cout << "calc> ";
    while (getline(std::cin, text)) {
        if (text == "exit" || text == "q") {
            break;
        }
        auto lexer = Lexer(text);
        auto parser = Parser(lexer);
        auto interpreter = std::make_shared<Interpreter>(parser);
        std::cout << interpreter->interpreter() << std::endl;
        std::cout << "calc> ";
    }

    return 0;
}
