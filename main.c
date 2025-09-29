#include "stdio.h"
#include "lexer.h"

int main(){
    const char* source =
        "int main() {\n"
        "    int sum = 10 + 25;\n"
        "    if (sum > 30) {\n"
        "        print sum;\n"
        "    } else {\n"
        "        print 0; // Print zero\n"
        "    }\n"
        "    return 0;\n"
        "}";

    Lexer lexer;
    lexer_init(&lexer, source);

    int line = -1;
    for (;;) {
        Token token = scan_token(&lexer);
        if (token.line != line) {
            printf("%4d ", token.line);
            line = token.line;
        } else {
            printf("   | ");
        }
        // Print the token type and its lexeme
        // The '%.*s' is a neat printf trick to print a string of a specific length
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF) {
            break;
        }
    }

    return 0;
}