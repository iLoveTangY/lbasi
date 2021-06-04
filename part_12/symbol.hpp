#ifndef SYMBOL_HPP_
#define SYMBOL_HPP_

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

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

#endif
