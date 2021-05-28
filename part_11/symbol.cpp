#include "symbol.hpp"

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