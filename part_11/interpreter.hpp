#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <iostream>
#include <memory>

#include "ast.hpp"
#include "parser.hpp"
#include "symbol.hpp"

class Interpreter : public Visitor, public std::enable_shared_from_this<Interpreter> {
   public:
    explicit Interpreter(const Parser &parser) : parser_(parser) {}

    double calculate(const std::shared_ptr<ASTNode> &node);

    void printGlobalScope();

    void printSymbolTable();

    void interpret();

    void visit(const std::shared_ptr<ProgramNode> &node) override;

    void visit(const std::shared_ptr<BlockNode> &node) override;

    void visit(const std::shared_ptr<VarDeclNode> &node) override;

    void visit(const std::shared_ptr<TypeNode> &node) override;

    void visit(const std::shared_ptr<BinaryOpNode> &node) override;

    void visit(const std::shared_ptr<NumNode> &node) override;

    void visit(const std::shared_ptr<UnaryOpNode> &node) override;

    void visit(const std::shared_ptr<CompoundNode> &node) override;

    void visit(const std::shared_ptr<AssignNode> &node) override;

    void visit(const std::shared_ptr<VarNode> &node) override;

    void visit(const std::shared_ptr<NoOpNode> &node) override;

   private:
    Parser parser_;
    SymbolTable symbol_table_;
    double expr_value_;
    std::unordered_map<std::string, double> GLOBAL_SCOPE_;
};

#endif
