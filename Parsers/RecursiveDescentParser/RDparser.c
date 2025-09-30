#include "stdio.h"
#include "stdbool.h"
#include "RDparser.h"


//============ HELPER FUNCTIONS ==================

static void errorAt(Token* token, const char* message) {
    if (parser.hadError) return; // Prevent cascading errors.
    parser.hadError = true;
    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type != TOKEN_ERROR) {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static bool check(token_type type){
    return parser.current.type == type;
}

static void advance(){
    parser.previous = parser.current;
    for(;;){
        parser.current = scan_token(&lexer);
        if(!check(TOKEN_ERROR))break;
    }
}

static bool match(token_type type){
    if(check(type)){
        advance();
        return true;
    }
    else return false;
}

static void consume(token_type type, const char* message){
    if(check(type))advance();
    else errorAtCurrent(message);
}

//=========== GRAMMAR RULES ======================
static void program();
static void declaration();
static void var_declaration();
static void statement();
static void expr_statement();
static void if_statement();
static void print_statement();
static void while_statement();
static void expression();
static void assignment();
static void equality();
static void comparison();
static void term();
static void factor();
static void unary();
static void primary();

static void program(){
    while(!check(TOKEN_EOF))declaration();
}

static void declaration(){
    if(match(TOKEN_INT))var_declaration();
    else statement();
}

static void var_declaration(){
    consume(TOKEN_IDENTIFIER,"Expected a valid identifier");
    if(match(TOKEN_EQUAL))expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the declaration");
}

static void statement(){
    if(match(TOKEN_WHILE))while_statement();
    else if(match(TOKEN_PRINT))print_statement();
    else if(match(TOKEN_IF))if_statement();
    else expr_statement();
}

static void expr_statement(){
    expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the expression statement");
}

static void if_statement(){
    consume(TOKEN_OPEN_PARENTHESIS,"Expected opening parenthesis");
    expression();
    consume(TOKEN_CLOSE_PARENTHESIS,"Expected closing parenthesis");
    statement();
    if(match(TOKEN_ELSE))statement();
}

static void print_statement(){
    expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the statement");
}

static void while_statement(){
    consume(TOKEN_OPEN_PARENTHESIS,"Expected opening parenthesis");
    expression();
    consume(TOKEN_CLOSE_PARENTHESIS,"Expected closing parenthesis");
    statement();
}

static void expression(){
    assignment();
}

static void assignment(){
    if (check(TOKEN_IDENTIFIER) && lexer.current[0] == '=') {
        advance(); // Consume identifier
        if (match(TOKEN_EQUAL)) {
            assignment();
        }
    } else {
        equality();
    }
}

static void equality(){
    comparison();
    while (match(TOKEN_BANG_EQUAL) || match(TOKEN_EQUAL_EQUAL))comparison();
}

static void comparison(){
    term();
    while (match(TOKEN_GREATER) || match(TOKEN_GREATER_EQUAL) || match(TOKEN_SMALLER) || match(TOKEN_SMALLER_EQUAL))term();
}

static void term(){
    factor();
    while(match(TOKEN_MINUS) || match(TOKEN_PLUS))factor();
}

static void factor(){
    unary();
    while(match(TOKEN_SLASH) || match(TOKEN_STAR))unary();
}

static void unary(){
    if(match(TOKEN_MINUS) || match(TOKEN_PLUS))unary();
    else primary();
}

static void primary(){
    if(match(TOKEN_INTEGER) || match(TOKEN_IDENTIFIER))return;
    consume(TOKEN_OPEN_PARENTHESIS,"Expected an opening parenthesis");
    expression();
    consume(TOKEN_CLOSE_PARENTHESIS,"Expected an closing parenthesis");
}

bool parse(const char* source){
    lexer_init(&lexer,source);
    parser.hadError = false;

    advance(); // Prime the parser with the first token.

    while (!check(TOKEN_EOF)) {
        declaration();
        // If we hit an error, we don't want to get stuck in an infinite loop.
        if (parser.hadError) break;
    }

    return !parser.hadError;
}