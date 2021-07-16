#ifndef ERROR_H_
#define ERROR_H_

#include <exception>
#include <optional>
#include <sstream>
#include <unordered_map>

#include "token.hpp"

enum ErrorCode : uint32_t {
    UNEXPECTED_TOKEN,
    ID_NOT_FOUND,
    DUPLICATE_ID,
};

std::string toString(ErrorCode code);

class Error : public std::exception {
   public:
    Error(std::string message) : message_(std::move(message)) {}
    Error(ErrorCode error_code, Token token, std::string message)
        : error_code_(error_code), token_(token), message_(std::move(message)) {
        message_ = toString(error_code);
        message_ += "->";
        std::stringstream ss;
        ss << token;
        message_ += ss.str();
    }

    virtual const char* what() const throw() { return message_.c_str(); }

   private:
    static std::unordered_map<ErrorCode, std::string> m_;
    std::optional<ErrorCode> error_code_;
    std::optional<Token> token_;
    std::string message_;
};

class LexerError : public Error {
   public:
    LexerError(std::string message) : Error(message) {}
};

class ParserError : public Error {
   public:
    ParserError(ErrorCode error_code, Token token, std::string message) : Error(error_code, token, message) {}
};

class SemanticError : public Error {
   public:
    SemanticError(ErrorCode error_code, Token token, std::string message) : Error(error_code, token, message) {}
};

#endif
