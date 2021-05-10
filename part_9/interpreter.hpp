#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "ast.hpp"
#include <memory>
#include "parser.hpp"
#include <iostream>

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter> {
   public:
    explicit Interpreter(const Parser &parser) : parser_(parser) {}

    double calculate(const std::shared_ptr<ASTNode> &node);

    void printGlobalScope(); 

    void interpret();

    void visit(const std::shared_ptr<BinaryOpNode> &node) override;

    void visit(const std::shared_ptr<NumNode> &node) override; 

    void visit(const std::shared_ptr<UnaryOpNode> &node) override;

    void visit(const std::shared_ptr<CompoundNode> &node) override;

    void visit(const std::shared_ptr<AssignNode> &node) override;

    void visit(const std::shared_ptr<VarNode> &node) override;

    void visit(const std::shared_ptr<NoOpNode> &node) override;

   private:
    Parser parser_;
    double expr_value_;
    std::unordered_map<std::string, double> GLOBAL_SCOPE_;
};

#endif
