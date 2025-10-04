#include "stdio.h"
#include "./Lexer/lexer.h"
#include "./Parsers/RecursiveDescentParser/RDparser.h"
#include "./Parsers/RecursiveDescentParser/AST.h"
Lexer lexer;
Parser parser;
int main(){
    const char* source =
        "    //hey i am ankit\n"
        "    "
        "  //  afkasdfkh\n"
        "    int sum = 10 + 25;\n"
        "    if (sum > 30) \n"
        "        print sum;\n"
        "     else \n"
        "        print 0; // Print zero\n"
        "    \n";

    // Lexer lexer;
    // lexer_init(&lexer, source);

    // int line = -1;
    // for (;;) {
    //     Token token = scan_token(&lexer);
    //     if (token.line != line) {
    //         printf("%4d ", token.line);
    //         line = token.line;
    //     } else {
    //         printf("   | ");
    //     }
    //     // Print the token type and its lexeme
    //     // The '%.*s' is a neat printf trick to print a string of a specific length
    //     printf("%2d '%.*s'\n", token.type, token.length, token.start);

    //     if (token.type == TOKEN_EOF) {
    //         break;
    //     }
    // }
    int cnt = 0;
    Stmt** stmt = parse(source,&cnt);
    printAst(stmt,cnt);

    return 0;
}