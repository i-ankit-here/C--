#ifndef PARSER_HEADER_H
#define PARSER_HEADER_H
#include "../../Lexer/lexer.h"
#include "stdbool.h"

typedef struct {
    Token current;
    Token previous;
    bool hadError;
} Parser;

extern Lexer lexer;
extern Parser parser;

// The main function that kicks off the parsing.
// It takes the source code as input and returns true if it's syntactically valid, and false otherwise.
bool parse(const char* source);

#endif