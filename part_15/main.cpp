// @Author: tangyang.01@bytedance.com
// @Date: 2021-04-21 09:59:02

//              program : PROGRAM variable SEMI block DOT

//                block : declaritions compound_statement

//         declaritions : VAR (variable_declarition SEMI)+
//                      | (PROCEDURE ID SEMI block SEMI)*
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

#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "semantic_analyzer.hpp"

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
program Main;
var
   a : integer;

begin { Main }
   a := b;  { semantic error }
end.  { Main }
    )");

    auto parser = Parser(lexer);
    auto sematic_analyzer = std::make_shared<SemanticAnalyzer>(parser);
    sematic_analyzer->check();
    // sematic_analyzer->print();

    // auto interpreter = std::make_shared<Interpreter>(parser);
    // interpreter->interpret();
    // interpreter->printGlobalScope();
    // interpreter->printSymbolTable();

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
