#include "symbol.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &out, const Symbol &symbol) {
    if (symbol.type_) {
        // 只打印 VarSymbol
        out << "<" << symbol.name_;
        out << ":" << symbol.type_->name_;
        out << ">";
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const SymbolTable &table) {
    for (const auto &[name, symbol] : table.symbols_) {
        out << *symbol;
    }
    return out;
}

std::shared_ptr<Symbol> SymbolTable::lookup(const std::string &name) {
    std::shared_ptr<Symbol> ret = nullptr;
    if (symbols_.find(name) != symbols_.end()) {
        ret = symbols_[name];
    }
    return ret;
}

std::ostream &operator<<(std::ostream &out, const ScopedSymbolTable &table) {
    out << "SCOPE (SCOPED SYMBOL TABLE)\n";
    out << "===========================" << std::endl;
    out << "Scope name: " << table.scope_name_ << std::endl;
    out << "Scope level: " << table.scope_level_ << std::endl;
    if (table.enclosing_scope_) {
        out << "Enclosing scope: " << table.enclosing_scope_->scope_name_ << std::endl;
    }
    out << "Scope (Scope symbol table) contents\n";
    out << "-----------------------------------" << std::endl;
    for (const auto &[name, symbol] : table.symbols_) {
        out << *symbol << std::endl;
    }
    return out;
}

std::shared_ptr<Symbol> ScopedSymbolTable::lookup(const std::string &name) {
    std::cout << "lookup: " << name << ". (Scope name: " << scope_name_ << ")" << std::endl;
    if (symbols_.find(name) != symbols_.end()) {
        return symbols_[name];
    }
    if (enclosing_scope_ != nullptr) {
        return enclosing_scope_->lookup(name);
    }
    return nullptr;
}