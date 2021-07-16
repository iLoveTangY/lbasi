#ifndef SEMANTIC_ANALYZER_HPP_
#define SEMANTIC_ANALYZER_HPP_

#include <iostream>
#include <memory>
#include <sstream>

#include "ast.hpp"
#include "error.hpp"
#include "parser.hpp"
#include "symbol.hpp"

class SemanticAnalyzer : public std::enable_shared_from_this<SemanticAnalyzer>, public Visitor {
   public:
    SemanticAnalyzer(const Parser &parser) : parser_(parser) {}

    void error(ErrorCode error_code, const Token &token) {
        std::string message = toString(error_code);
        message += "->";
        std::stringstream ss;
        ss << token;
        message += ss.str();

        throw SemanticError(error_code, token, message);
    }

    void check() {
        auto root_node = parser_.parse();
        root_node->visit(shared_from_this());
    }

    void visit(const std::shared_ptr<ProgramNode> &node) override {
        std::cout << "ENTER scope: global" << std::endl;
        auto global_scope = std::make_shared<ScopedSymbolTable>("global", 1, current_scope_);
        current_scope_ = global_scope;
        node->block_->visit(shared_from_this());
        std::cout << *global_scope << std::endl;
        current_scope_ = current_scope_->enclosing_scope();
        std::cout << "LEAVE scope: global" << std::endl;
    }

    void visit(const std::shared_ptr<BlockNode> &node) override {
        for (auto &&declaration : node->declarations_) {
            declaration->visit(shared_from_this());
        }
        node->compound_statement_->visit(shared_from_this());
    }

    void visit(const std::shared_ptr<VarDeclNode> &node) override {
        auto type_name = node->type_node_->token_.str_;
        auto type_symbol = current_scope_->lookup(type_name);
        auto var_name = node->var_node_->value_;
        auto var_symbol = std::make_shared<VarSymbol>(var_name, type_symbol);
        if (current_scope_->lookup(var_name, true)) {
            error(DUPLICATE_ID, node->var_node_->token_);
        }
        current_scope_->define(var_symbol);
    }

    void visit(const std::shared_ptr<TypeNode> &node) override {}

    void visit(const std::shared_ptr<BinaryOpNode> &node) override {
        node->left_->visit(shared_from_this());
        node->right_->visit(shared_from_this());
    }

    void visit(const std::shared_ptr<NumNode> &node) override {}

    void visit(const std::shared_ptr<UnaryOpNode> &node) override {}

    void visit(const std::shared_ptr<CompoundNode> &node) override {
        for (const auto &child : node->children_) {
            child->visit(shared_from_this());
        }
    }

    void visit(const std::shared_ptr<AssignNode> &node) override {
        auto var_name = node->left_;
        if (current_scope_->lookup(var_name) == nullptr) {
            error(ID_NOT_FOUND, node->token_);
        }
        // 无需求值，只需遍历检查
        node->right_->visit(shared_from_this());
    }

    void visit(const std::shared_ptr<VarNode> &node) override {
        auto var_name = node->value_;
        auto var_symbol = current_scope_->lookup(var_name);
        if (!var_symbol) {
            error(ID_NOT_FOUND, node->token_);
        }
    }

    void visit(const std::shared_ptr<ProcedureDecl> &node) override {
        auto proc_name = node->proc_name_;
        auto proc_symbol = std::make_shared<ProcedureSymbol>(proc_name);
        current_scope_->define(proc_symbol);
        std::cout << "ENTER scope: " << proc_name << std::endl;
        auto procedure_scope =
            std::make_shared<ScopedSymbolTable>(proc_name, current_scope_->scope_level() + 1, current_scope_);
        current_scope_ = procedure_scope;

        for (const auto &param : node->params_) {
            auto param_type = current_scope_->lookup(param->type_node_->value());
            auto param_name = param->var_node_->value_;
            auto var_symbol = std::make_shared<VarSymbol>(param_name, param_type);
            current_scope_->define(var_symbol);
            proc_symbol->params.push_back(std::move(var_symbol));
        }
        node->block_->visit(shared_from_this());
        std::cout << *procedure_scope << std::endl;
        current_scope_ = current_scope_->enclosing_scope();
        std::cout << "LEAVE scope: " << proc_name << std::endl;
    }

    void visit(const std::shared_ptr<NoOpNode> &node) override {}

    void visit(const std::shared_ptr<ParamNode> &node) override {}

    void print() { std::cout << current_scope_ << std::endl; }

   private:
    std::shared_ptr<ScopedSymbolTable> current_scope_ = nullptr;
    Parser parser_;
};

#endif
