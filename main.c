#include "stdio.h"
#include "./Lexer/lexer.h"
#include "./Parsers/RecursiveDescentParser/RDparser.h"
Lexer lexer;
Parser parser;
int main(){
    const char* source =
        "    //hey i am ankit\n"
        "    "
        "  //  afkasdfkh"
        "    int sum = 10 + 25;\n"
        "    if (sum > 30) \n"
        "        print sum;\n"
        "     else \n"
        "        print1 0; // Print zero\n"
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
    bool flag = parse(source);
    if(flag)printf("Succesfully parsed");

    return 0;
}