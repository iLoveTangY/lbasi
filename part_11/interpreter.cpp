#include "interpreter.hpp"
#include "token.hpp"

#include <exception>

double Interpreter::calculate(const std::shared_ptr<ASTNode> &node) {
    node->visit(shared_from_this());
    return expr_value_;
}

void Interpreter::printGlobalScope() {
    std::cout << "GLOBAL_SCOPE.size() = " << GLOBAL_SCOPE_.size() << std::endl;
    for (const auto &[identifier, value] : GLOBAL_SCOPE_) {
        std::cout << identifier + ": " << value << std::endl;
    }
}

void Interpreter::printSymbolTable() {
    std::cout << symbol_table_ << std::endl;
}

void Interpreter::interpret() {
    auto root_node = parser_.parse();
    if (nullptr == root_node) {
        return;
    }
    root_node->visit(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<ProgramNode> &node) {
    std::cout << node->name_ << ": " << std::endl;
    node->block_->visit(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<BlockNode> &node) {
    for (auto && declaration : node->declarations_) {
        declaration->visit(shared_from_this());
    }
    node->compound_statement_->visit(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<VarDeclNode> &node) {
    auto type_name = node->type_node_->token_.str_;
    auto type_symbol = symbol_table_.lookup(type_name);
    auto var_name = node->var_node_->value_;
    auto var_symbol = std::make_shared<VarSymbol>(var_name, type_symbol);
    symbol_table_.define(var_symbol);
}

void Interpreter::visit(const std::shared_ptr<TypeNode> &node) {
  // TODO
}

void Interpreter::visit(const std::shared_ptr<BinaryOpNode> &node) {
    auto this_ptr = shared_from_this();
    if (node->op_.type_ == PLUS) {
        expr_value_ = calculate(node->left_) + calculate(node->right_);
    } else if (node->op_.type_ == MINUS) {
        expr_value_ = calculate(node->left_) - calculate(node->right_);
    } else if (node->op_.type_ == MUL) {
        expr_value_ = calculate(node->left_) * calculate(node->right_);
    } else if (node->op_.type_ == INTEGER_DIV) {  // 整数除法
        expr_value_ = static_cast<int>(calculate(node->left_)) / static_cast<int>(calculate(node->right_));
    } else if (node->op_.type_ == FLOAT_DIV) {
        expr_value_ = calculate(node->left_) / calculate(node->right_);
    }
}

void Interpreter::visit(const std::shared_ptr<NumNode> &node) {
    expr_value_ = node->value_;
}

void Interpreter::visit(const std::shared_ptr<UnaryOpNode> &node) {
    auto op = node->token_.type_;
    auto value = calculate(node->expr_);
    if (op == PLUS) {
        expr_value_ = value;
    } else {
        expr_value_ = -value;
    }
}

void Interpreter::visit(const std::shared_ptr<CompoundNode> &node) {
    for (const auto &child : node->children_) {
        child->visit(shared_from_this());
    }
}

void Interpreter::visit(const std::shared_ptr<AssignNode> &node) {
    auto identifier = node->left_;
    if (!symbol_table_.lookup(identifier)) {
        throw std::runtime_error("identifier " + identifier + " not declare");
    }
    GLOBAL_SCOPE_[identifier] = calculate(node->right_);
}

void Interpreter::visit(const std::shared_ptr<VarNode> &node) {
    auto identifier = node->value_;
    auto it = GLOBAL_SCOPE_.find(identifier);
    if (it != GLOBAL_SCOPE_.end()) {
        expr_value_ = it->second;
    } else {
        throw std::runtime_error("variable " + identifier + " is not defined");
    }
}

void Interpreter::visit(const std::shared_ptr<NoOpNode> &node) {}