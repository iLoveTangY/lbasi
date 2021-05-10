// @Author: tangyang.01@bytedance.com
// @Date: 2021-04-21 09:59:02

// complete grammar
// program : compund_statement DOT
// compound_statement : BEGIN statement_list END
// statement_list : statement
//                | statement SEMI statement_list
// statement : compound_statement
//           | assignment_statement
//           | empty
// assignment_statement: variabl ASSIGN expr
// empty   :
// expr    : term ((PLUS | MINUS) term)*
// term    : factor ((MUL | DIV) factor)*
// factor  : PLUS factor
//         | MINUS factor
//         | INTEGER
//         | LPAREN expr RPAREN
//         | variable
// variable: ID

#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

int main() {
    // std::string text;
    // std::cout << "calc> ";
    // while (getline(std::cin, text)) {
    //     if (text == "exit" || text == "q") {
    //         break;
    //     }
    //     auto lexer = Lexer(text);
    //     auto parser = Parser(lexer);
    //     auto interpreter = std::make_shared<Interpreter>(parser);
    //     std::cout << interpreter->interpreter() << std::endl;
    //     std::cout << "calc> ";
    // }

    auto lexer = Lexer(R"(
    BEGIN
        BEGIN
            number := 2;
            a := number;
            b := 10 * a + 10 * number / 4;
            c := a -- b;
        END;
        x := 11;
    END.
    )");

    auto parser = Parser(lexer);
    auto interpreter = std::make_shared<Interpreter>(parser);
    interpreter->interpret();
    interpreter->printGlobalScope();

    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;
    // std::cout << lexer.getNextToken() << std::endl;

    return 0;
}
