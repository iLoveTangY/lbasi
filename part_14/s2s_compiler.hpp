#ifndef S2S_COMPILER_H_
#define S2S_COMPILER_H_

#include <iostream>

#include "ast.hpp"
#include "symbol.hpp"

class SourceToSourceCompiler : public std::enable_shared_from_this<SourceToSourceCompiler>, public Visitor {
   public:
    void visit(const std::shared_ptr<ProgramNode> &node) override {
        auto program_name = node->name_;
        std::string result_str = "program " + program_name + "0;\n";
        std::cout << "ENTER scope: global" << std::endl;
        auto global_scope = std::make_shared<ScopedSymbolTable>("global", 1, current_scope_);
        current_scope_ = global_scope;
        node->block_->visit(shared_from_this());
        result_str += function_return_str_;
        result_str += ".";
        result_str += " {END OF " + program_name + "}";
        output_ = result_str;
        std::cout << *global_scope << std::endl;
        current_scope_ = current_scope_->enclosing_scope();
        std::cout << "LEAVE scope: global" << std::endl;
    }

    void visit(const std::shared_ptr<BlockNode> &node) override {
        function_return_str_ = "";
        std::string results = "";
        for (auto &&declaration : node->declarations_) {
            declaration->visit(shared_from_this());
            results += (function_return_str_ + "\n");
        }
        node->compound_statement_->visit(shared_from_this());
        auto result = "\t" + function_return_str_;
        results += result + "\n";
        results += "end";
        function_return_str_ = results;
    }

    void visit(const std::shared_ptr<VarDeclNode> &node) override {
        function_return_str_ = "";
        auto type_name = node->type_node_->token_.str_;
        auto type_symbol = current_scope_->lookup(type_name);
        auto var_name = node->var_node_->value_;
        auto var_symbol = std::make_shared<VarSymbol>(var_name, type_symbol);
        current_scope_->define(var_symbol);
        function_return_str_ = "\t" + var_name + std::to_string(current_scope_->scope_level()) + " : " + type_name;
    }

    void visit(const std::shared_ptr<TypeNode> &node) override {}

    void visit(const std::shared_ptr<BinaryOpNode> &node) override {
        function_return_str_ = "";
        node->left_->visit(shared_from_this());
        auto t1 = function_return_str_;
        node->right_->visit(shared_from_this());
        auto t2 = function_return_str_;
        function_return_str_ = t1 + " " + node->op_.str_ + " " + t2;
    }

    void visit(const std::shared_ptr<NumNode> &node) override {}

    void visit(const std::shared_ptr<UnaryOpNode> &node) override {}

    void visit(const std::shared_ptr<CompoundNode> &node) override {
        function_return_str_ = "";
        std::string results = "";
        for (const auto &child : node->children_) {
            child->visit(shared_from_this());
            results += function_return_str_ + "\n";
        }
        function_return_str_ = std::move(results);
    }

    void visit(const std::shared_ptr<AssignNode> &node) override {
        function_return_str_ = "";
        auto var_name = node->left_;
        if (current_scope_->lookup(var_name) == nullptr) {
            throw std::runtime_error("Error: Symbol(identifier) not found " + var_name);
        }
        std::string result = var_name + " := ";
        // 无需求值，只需遍历检查
        node->right_->visit(shared_from_this());
        function_return_str_ = result + " " + function_return_str_;
    }

    void visit(const std::shared_ptr<VarNode> &node) override {
        function_return_str_ = "";
        auto var_name = node->value_;
        auto var_symbol = current_scope_->lookup(var_name);
        if (!var_symbol) {
            throw std::runtime_error("Error: Symbol(identifier) not found " + var_name);
        }
        // auto scope_level = std::to_string(var_symbol->)
        function_return_str_ = "<" + var_name + "*:" + var_symbol->type_->name_ + ">";
    }

    void visit(const std::shared_ptr<ProcedureDecl> &node) override {
        function_return_str_ = "";
        std::string result = "procdeure " + node->proc_name_ + std::to_string(current_scope_->scope_level());
        auto proc_name = node->proc_name_;
        auto proc_symbol = std::make_shared<ProcedureSymbol>(proc_name);
        current_scope_->define(proc_symbol);
        std::cout << "ENTER scope: " << proc_name << std::endl;
        auto procedure_scope =
            std::make_shared<ScopedSymbolTable>(proc_name, current_scope_->scope_level() + 1, current_scope_);
        current_scope_ = procedure_scope;

        if (!(node->params_.empty())) {
            result += "(";
        }

        for (const auto &param : node->params_) {
            auto param_type = current_scope_->lookup(param->type_node_->value());
            auto param_name = param->var_node_->value_;
            auto var_symbol = std::make_shared<VarSymbol>(param_name, param_type);
            current_scope_->define(var_symbol);
            proc_symbol->params.push_back(std::move(var_symbol));
            auto scope_level = std::to_string(current_scope_->scope_level());
            result += param_name + scope_level + " : " + param_type->name_ + ";";
        }
        if (!(node->params_.empty())) {
            result += ")";
        }
        result += ";\n";
        node->block_->visit(shared_from_this());
        result += function_return_str_;
        result += "; {END of " + proc_name + "}";
        std::cout << *procedure_scope << std::endl;
        current_scope_ = current_scope_->enclosing_scope();
        std::cout << "LEAVE scope: " << proc_name << std::endl;
        function_return_str_ = std::move(result);
    }

    void visit(const std::shared_ptr<NoOpNode> &node) override {}

    void visit(const std::shared_ptr<ParamNode> &node) override {}

    void print() { std::cout << current_scope_ << std::endl; }

   private:
    std::shared_ptr<ScopedSymbolTable> current_scope_ = nullptr;
    std::string function_return_str_ = "";
    std::string output_ = "";
};

#endif
