#ifndef AST_h
#define AST_h
#include "../../Lexer/lexer.h"

typedef struct Expr Expr;
typedef struct Stmt Stmt;

typedef enum {
    EXPR_BINARY,
    EXPR_UNARY,
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_VARIABLE,
    EXPR_ASSIGN
} ExprType;


typedef struct {
    Expr* left;
    Token op;
    Expr* right;
} BinaryExpr;

typedef struct {
    Token op;
    Expr* right;
} UnaryExpr;

typedef struct {
    int value;
} LiteralExpr;

typedef struct {
    Expr* expression;
} GroupingExpr;

typedef struct {
    Token name;
} VariableExpr;

typedef struct {
    Token name;
    Expr* value;
} AssignExpr;

struct Expr {
    ExprType type;
    union {
        BinaryExpr binary;
        UnaryExpr unary;
        LiteralExpr literal;
        GroupingExpr grouping;
        VariableExpr variable;
        AssignExpr assign;
    } as;
};


typedef enum {
    STMT_EXPRESSION,
    STMT_PRINT,
    STMT_VAR_DECLARATION,
    STMT_IF,
    STMT_WHILE,
    STMT_BLOCK
} StmtType;

// typedef struct {
//     Stmt** statements;
//     int count;
// } BlockStmt;

typedef struct {
    Expr* expression;
} ExpressionStmt;

typedef struct {
    Expr* expression;
} PrintStmt;

typedef struct {
    Token name;
    Expr* initializer;
} VarDeclStmt;

typedef struct {
    Expr* condition;
    Stmt* thenBranch;
    Stmt* elseBranch;
} IfStmt;

typedef struct {
    Expr* condition;
    Stmt* body;
} WhileStmt;

struct Stmt {
    StmtType type;
    union {
        ExpressionStmt expression;
        PrintStmt print;
        VarDeclStmt var;
        IfStmt ifStmt;
        WhileStmt whileStmt;
        // BlockStmt block;
    } as;
};

void printAst(Stmt** statements, int count);

#endif