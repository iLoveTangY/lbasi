#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

class Symbol {
   public:
    Symbol(std::string name, std::shared_ptr<Symbol> type) : name_(std::move(name)), type_(type) {}

    std::string name_;
    std::shared_ptr<Symbol> type_;
};

std::ostream &operator<<(std::ostream &out, const Symbol &symbol);

class BuiltinTypeSymbol : public Symbol {
   public:
    BuiltinTypeSymbol(std::string name) : Symbol(std::move(name), nullptr) {}
};

class VarSymbol : public Symbol {
   public:
    VarSymbol(std::string name, std::shared_ptr<Symbol> type) : Symbol(std::move(name), type) {}
};

class ProcedureSymbol : public Symbol {
   public:
    ProcedureSymbol(std::string name /* params */) : Symbol(std::move(name), nullptr) {}
    std::vector<std::shared_ptr<VarSymbol>> params;
};

class SymbolTable {
   public:
    SymbolTable() { init_builtins(); }

    void define(std::shared_ptr<Symbol> symbol) { symbols_[symbol->name_] = symbol; }

    std::shared_ptr<Symbol> lookup(const std::string &name);

    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;

   private:
    void init_builtins() {
        define(std::make_shared<BuiltinTypeSymbol>("INTEGER"));
        define(std::make_shared<BuiltinTypeSymbol>("REAL"));
    }
};

std::ostream &operator<<(std::ostream &out, const SymbolTable &table);

class ScopedSymbolTable {
   public:
    friend std::ostream &operator<<(std::ostream &out, const ScopedSymbolTable &table);

    ScopedSymbolTable(std::string scope_name, int scope_level, std::shared_ptr<ScopedSymbolTable> enclosing_scope = nullptr)
        : scope_name_(std::move(scope_name)), scope_level_(scope_level), enclosing_scope_(std::move(enclosing_scope)) {
        init_builtins();
    }

    void define(std::shared_ptr<Symbol> symbol) { symbols_[symbol->name_] = symbol; }

    std::shared_ptr<Symbol> lookup(const std::string &name, bool current_scope_only = false);

    int scope_level() { return scope_level_; }
    std::shared_ptr<ScopedSymbolTable> enclosing_scope() { return enclosing_scope_; }

   private:
    void init_builtins() {
        define(std::make_shared<BuiltinTypeSymbol>("INTEGER"));
        define(std::make_shared<BuiltinTypeSymbol>("REAL"));
    }

    std::unordered_map<std::string, std::shared_ptr<Symbol>> symbols_;
    std::shared_ptr<ScopedSymbolTable> enclosing_scope_;
    std::string scope_name_;
    int scope_level_;
};

std::ostream &operator<<(std::ostream &out, const ScopedSymbolTable &table);

#endif
