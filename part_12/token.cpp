#include "token.hpp"

std::ostream &operator<<(std::ostream &out, const TokenType &type) {
    switch (type) {
        case BEGIN:
            out << "BEGIN";
            break;
        case END:
            out << "END";
            break;
        case DOT:
            out << "DOT";
            break;
        case ASSIGN:
            out << "ASSIGN";
            break;
        case SEMI:
            out << "SEMI";
            break;
        case ID:
            out << "ID";
            break;
        case INTEGER:
            out << "INTEGER";
            break;
        case PLUS:
            out << "PLUS";
            break;
        case MINUS:
            out << "MIUS";
            break;
        case MUL:
            out << "MUL";
            break;
        case INTEGER_DIV:
            out << "DIV";
            break;
        case FLOAT_DIV:
            out << "/";
            break;
        case END_OF_FILE:
            out << "EOF";
            break;
        case LP:
            out << "LP";
            break;
        case RP:
            out << "RP";
            break;
        case PROCEDURE:
            out << "PROCEDURE";
            break;
        default:
            break;
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << "Token(" << token.type_ << ", ";
    if (token.str_.empty()) {
        out << token.value_;
    } else {
        out << token.str_;
    }
    out << ")";
    return out;
}
