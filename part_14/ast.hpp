#ifndef AST_HPP
#define AST_HPP

#include <memory>
#include <string>
#include <vector>

#include "token.hpp"

class BinaryOpNode;
class NumNode;
class UnaryOpNode;
class CompoundNode;
class AssignNode;
class VarNode;
class NoOpNode;
class ProgramNode;
class BlockNode;
class VarDeclNode;
class TypeNode;
class ProcedureDecl;
class ParamNode;

class Visitor {
   public:
    virtual void visit(const std::shared_ptr<BinaryOpNode> &node) = 0;
    virtual void visit(const std::shared_ptr<NumNode> &node) = 0;
    virtual void visit(const std::shared_ptr<UnaryOpNode> &node) = 0;
    virtual void visit(const std::shared_ptr<CompoundNode> &node) = 0;
    virtual void visit(const std::shared_ptr<AssignNode> &node) = 0;
    virtual void visit(const std::shared_ptr<VarNode> &node) = 0;
    virtual void visit(const std::shared_ptr<NoOpNode> &node) = 0;
    virtual void visit(const std::shared_ptr<ProgramNode> &node) = 0;
    virtual void visit(const std::shared_ptr<BlockNode> &node) = 0;
    virtual void visit(const std::shared_ptr<VarDeclNode> &node) = 0;
    virtual void visit(const std::shared_ptr<TypeNode> &node) = 0;
    virtual void visit(const std::shared_ptr<ProcedureDecl> &node) = 0;
    virtual void visit(const std::shared_ptr<ParamNode> &node) = 0;
};

class ASTNode {
   public:
    virtual void visit(const std::shared_ptr<Visitor> &visitor) = 0;
};

class UnaryOpNode : public ASTNode, public std::enable_shared_from_this<UnaryOpNode> {
   public:
    UnaryOpNode(const Token &op, const std::shared_ptr<ASTNode> &expr) : token_(op), expr_(std::move(expr)) {}

    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }

    Token token_;
    std::shared_ptr<ASTNode> expr_;
};

class BinaryOpNode : public ASTNode, public std::enable_shared_from_this<BinaryOpNode> {
   public:
    BinaryOpNode(std::shared_ptr<ASTNode> left, Token op, std::shared_ptr<ASTNode> right)
        : left_(std::move(left)), op_(op), right_(std::move(right)) {}

    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }

    std::shared_ptr<ASTNode> left_;
    std::shared_ptr<ASTNode> right_;
    Token op_;
};

// 表示"BEGIN ... END" 块
class CompoundNode : public ASTNode, public std::enable_shared_from_this<CompoundNode> {
   public:
    explicit CompoundNode(std::vector<std::shared_ptr<ASTNode>> children) : children_(std::move(children)) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::vector<std::shared_ptr<ASTNode>> children_;
};

class AssignNode : public ASTNode, public std::enable_shared_from_this<AssignNode> {
   public:
    AssignNode(std::string left, Token op, std::shared_ptr<ASTNode> right) : left_(left), token_(op), right_(right) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::string left_;
    std::shared_ptr<ASTNode> right_;
    Token token_;
};

class VarNode : public ASTNode, public std::enable_shared_from_this<VarNode> {
   public:
    explicit VarNode(Token token) : token_(token), value_(std::move(token.str_)) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    Token token_;
    std::string value_;
};

class NoOpNode : public ASTNode, public std::enable_shared_from_this<NoOpNode> {
   public:
    NoOpNode() = default;
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
};

class NumNode : public ASTNode, public std::enable_shared_from_this<NumNode> {
   public:
    NumNode(Token token) : token_(token) { value_ = token.value_; }

    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    Token token_;
    int value_;
};

class ProgramNode : public ASTNode, public std::enable_shared_from_this<ProgramNode> {
   public:
    ProgramNode(std::string name, std::shared_ptr<BlockNode> block) : name_(std::move(name)), block_(block) {}

    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }

    std::string name_;
    std::shared_ptr<BlockNode> block_;
};

class BlockNode : public ASTNode, public std::enable_shared_from_this<BlockNode> {
   public:
    BlockNode(std::vector<std::shared_ptr<ASTNode>> declarations, std::shared_ptr<CompoundNode> compound_statement)
        : declarations_(declarations), compound_statement_(compound_statement) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::shared_ptr<CompoundNode> compound_statement_;
    std::vector<std::shared_ptr<ASTNode>> declarations_;
};

class VarDeclNode : public ASTNode, public std::enable_shared_from_this<VarDeclNode> {
   public:
    VarDeclNode(std::shared_ptr<VarNode> var_node, std::shared_ptr<TypeNode> type_node)
        : var_node_(var_node), type_node_(type_node) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::shared_ptr<VarNode> var_node_;
    std::shared_ptr<TypeNode> type_node_;
};

class TypeNode : public ASTNode, public std::enable_shared_from_this<TypeNode> {
   public:
    TypeNode(Token token) : token_(token) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::string value() const { return token_.str_; }
    Token token_;
};

class ParamNode : public ASTNode, public std::enable_shared_from_this<ParamNode> {
   public:
    ParamNode(std::shared_ptr<VarNode> var_node, std::shared_ptr<TypeNode> type_node)
        : var_node_(std::move(var_node)), type_node_(std::move(type_node)) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::shared_ptr<VarNode> var_node_;
    std::shared_ptr<TypeNode> type_node_;
};

class ProcedureDecl : public ASTNode, public std::enable_shared_from_this<ProcedureDecl> {
   public:
    ProcedureDecl(std::string proc_name, std::vector<std::shared_ptr<ParamNode>> params, std::shared_ptr<BlockNode> block)
        : proc_name_(std::move(proc_name)), params_(std::move(params)), block_(std::move(block)) {}
    void visit(const std::shared_ptr<Visitor> &visitor) override { visitor->visit(shared_from_this()); }
    std::string proc_name_;
    std::shared_ptr<BlockNode> block_;
    std::vector<std::shared_ptr<ParamNode>> params_;
};

#endif
