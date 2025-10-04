#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "RDparser.h"
#include "AST.h"

//============ AST HELPER FUNCTIONS(CONSTRUCTORS) ======= 

static Expr* newBinary(Expr* left, Token op, Expr* right) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = EXPR_BINARY;
    expr->as.binary.left = left;
    expr->as.binary.op = op;
    expr->as.binary.right = right;
    return expr;
}

static Expr* newUnary(Token op, Expr* right) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = EXPR_UNARY;
    expr->as.unary.op = op;
    expr->as.unary.right = right;
    return expr;
}

static Expr* newLiteral(int value) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = EXPR_LITERAL;
    expr->as.literal.value = value;
    return expr;
}

static Expr* newVariable(Token name) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = EXPR_VARIABLE;
    expr->as.variable.name = name;
    return expr;
}

static Expr* newAssign(Token name, Expr* value) {
    Expr* expr = (Expr*)malloc(sizeof(Expr));
    expr->type = EXPR_ASSIGN;
    expr->as.assign.name = name;
    expr->as.assign.value = value;
    return expr;
}

static Stmt* newExpressionStmt(Expr* expr) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = STMT_EXPRESSION;
    stmt->as.expression.expression = expr;
    return stmt;
}

static Stmt* newPrintStmt(Expr* expr) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = STMT_PRINT;
    stmt->as.print.expression = expr;
    return stmt;
}

static Stmt* newVarDeclStmt(Token name, Expr* initializer) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = STMT_VAR_DECLARATION;
    stmt->as.var.name = name;
    stmt->as.var.initializer = initializer;
    return stmt;
}

static Stmt* newIfStmt(Expr* condition, Stmt* thenBranch, Stmt* elseBranch) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = STMT_IF;
    stmt->as.ifStmt.condition = condition;
    stmt->as.ifStmt.thenBranch = thenBranch;
    stmt->as.ifStmt.elseBranch = elseBranch;
    return stmt;
}

static Stmt* newWhileStmt(Expr* condition, Stmt* body) {
    Stmt* stmt = (Stmt*)malloc(sizeof(Stmt));
    stmt->type = STMT_WHILE;
    stmt->as.whileStmt.condition = condition;
    stmt->as.whileStmt.body = body;
    return stmt;
}






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
// static Stmt** program();
static Stmt* declaration();
static Stmt* var_declaration();
static Stmt* statement();
static Stmt* expr_statement();
static Stmt* if_statement();
static Stmt* print_statement();
static Stmt* while_statement();
static Expr* expression();
static Expr* assignment();
static Expr* equality();
static Expr* comparison();
static Expr* term();
static Expr* factor();
static Expr* unary();
static Expr* primary();

// static Stmt** program(){
//     while(!check(TOKEN_EOF))declaration();
// }

static Stmt* declaration(){
    if(match(TOKEN_INT))return var_declaration();
    else return statement();
}

static Stmt* var_declaration(){
    consume(TOKEN_IDENTIFIER,"Expected a valid identifier");
    Token name = parser.previous;
    Expr* expr = NULL;
    if(match(TOKEN_EQUAL))expr = expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the declaration");
    return newVarDeclStmt(name,expr);
}

static Stmt* statement(){
    if(match(TOKEN_WHILE))return while_statement();
    else if(match(TOKEN_PRINT))return print_statement();
    else if(match(TOKEN_IF))return if_statement();
    else return expr_statement();
}

static Stmt* expr_statement(){
    Expr* expr = expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the expression statement");
    return newExpressionStmt(expr);
}

static Stmt* if_statement(){
    consume(TOKEN_OPEN_PARENTHESIS,"Expected opening parenthesis");
    Expr* expr = expression();
    consume(TOKEN_CLOSE_PARENTHESIS,"Expected closing parenthesis");
    Stmt* if_stmt = statement();
    Stmt* else_stmt = NULL;
    if(match(TOKEN_ELSE))else_stmt = statement();
    return newIfStmt(expr,if_stmt,else_stmt);
}

static Stmt* print_statement(){
    Expr* expr =  expression();
    consume(TOKEN_SEMICOLON,"Expected a ; at the end of the statement");
    return newPrintStmt(expr);
}

static Stmt* while_statement(){
    consume(TOKEN_OPEN_PARENTHESIS,"Expected opening parenthesis");
    Expr* expr = expression();
    consume(TOKEN_CLOSE_PARENTHESIS,"Expected closing parenthesis");
    Stmt* stmt = statement();
    return newWhileStmt(expr,stmt);
}

static Expr* expression(){
    return assignment();
}

static Expr* assignment(){
    if (check(TOKEN_IDENTIFIER) && lexer.current[0] == '=') {
        advance(); // Consume identifier
        Token name = parser.previous;
        if (match(TOKEN_EQUAL)) {
            Expr* value = assignment();
            return newAssign(name,value); 
        }else return NULL;
    } else {
        return equality();
    }
}

static Expr* equality(){
    Expr* expr = comparison();
    while (match(TOKEN_BANG_EQUAL) || match(TOKEN_EQUAL_EQUAL)){
        Token op = parser.previous;
        Expr* right = comparison();
        expr = newBinary(expr,op,right);
    }
    return expr;
}

static Expr* comparison(){
    Expr* expr = term();
    while (match(TOKEN_GREATER) || match(TOKEN_GREATER_EQUAL) || match(TOKEN_SMALLER) || match(TOKEN_SMALLER_EQUAL)){
        Token op = parser.previous;
        Expr* right = term();
        expr = newBinary(expr,op,right);
    }
    return expr;
}

static Expr* term(){
    Expr* expr = factor();
    while(match(TOKEN_MINUS) || match(TOKEN_PLUS)){
        Token op = parser.previous;
        Expr* right = factor();
        expr = newBinary(expr,op,right);
    }
    return expr;
}

static Expr* factor(){
    Expr* expr = unary();
    while(match(TOKEN_SLASH) || match(TOKEN_STAR)){
        Token op = parser.previous;
        Expr* right = unary();
        expr = newBinary(expr,op,right);
    }
    return expr;
}

static Expr* unary(){
    if(match(TOKEN_MINUS) || match(TOKEN_PLUS)){
        Token op = parser.previous; 
        Expr* expr = unary();
        return newUnary(op,expr);
    }
    else return primary();
}

static Expr* primary(){
    if (match(TOKEN_INTEGER)) return newLiteral(strtol(parser.previous.start, NULL, 10));
    if (match(TOKEN_IDENTIFIER))return newVariable(parser.previous);
    if (match(TOKEN_OPEN_PARENTHESIS)) {
        Expr* expr = expression();
        consume(TOKEN_CLOSE_PARENTHESIS, "Expect ')' after expression.");
        return expr;
    }
    error("Expect primary");
    return NULL; 
}

Stmt** parse(const char* source,int* count){
    lexer_init(&lexer,source);
    parser.hadError = false;
    Stmt** statements = NULL;
    *count = 0;
    int capacity = 0;

    advance(); // Prime the parser with the first token.

    while (!check(TOKEN_EOF)) {
        if (*count >= capacity) {
            capacity = (capacity < 8) ? 8 : capacity * 2;
            statements = realloc(statements, sizeof(Stmt*) * capacity);
        }
        statements[*count] = declaration();
        (*count)++;
        // If we hit an error, we don't want to get stuck in an infinite loop.
        if (parser.hadError) break;
    }
    
    return statements;
}