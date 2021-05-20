// @Author: tangyang.01@bytedance.com
// @Date: 2021-04-21 09:59:02

//              program : PROGRAM variable SEMI block DOT

//                block : declaritions compound_statement

//         declaritions : VAR (variable_declarition SEMI)+
//                      | empty

// variable_declarition : variable (COMMA variable)* COLON type_spec

//            type_spec : INTEGER | REAL

//   compound_statement : BEGIN statement_list END

//       statement_list : statement
//                      | statement SEMI statement_list

//            statement : compound_statement
//                      | assignment_statement
//                      | empty

// assignment_statement : variabl ASSIGN expr

//                empty :

//                 expr : term ((PLUS | MINUS) term)*

//                 term : factor ((MUL | INTEGER_DIV | FLOAT_DIV) factor)*

//               factor : PLUS factor
//                      | MINUS factor
//                      | INTEGER_CONST
//                      | REAL_CONST
//                      | LPAREN expr RPAREN
//                      | variable

//             variable : ID

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
    PROGRAM Part10;
    VAR
        number     : INTEGER;
        a, b, c, x : INTEGER;
        y          : REAL;

    BEGIN {Part10}
        BEGIN
            number := 2;
            a := number;
            b := 10 * a + 10 * number DIV 4;
            c := a - - b
        END;
    x := 11;
    y := 20 / 7 + 3.14;
    { writeln('a = ', a); }
    { writeln('b = ', b); }
    { writeln('c = ', c); }
    { writeln('number = ', number); }
    { writeln('x = ', x); }
    { writeln('y = ', y); }
    END.  {Part10}
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
