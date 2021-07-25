#include "error.hpp"

std::string toString(ErrorCode code) {
    if (code == UNEXPECTED_TOKEN) {
        return "Unexpect token";
    } else if (code == ID_NOT_FOUND) {
        return "ID not found";
    } else if (code == DUPLICATE_ID) {
        return "Duplicate ID";
    }
    return "";
}
