#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ast.h"

// =================================================================
// ==================== FORWARD DECLARATIONS =======================
// =================================================================

// We declare the functions upfront to allow for mutual recursion.
static void printStmt(Stmt* stmt, const char* prefix, bool isLast);
static void printExpr(Expr* expr, const char* prefix, bool isLast);


// =================================================================
// ==================== EXPRESSION PRINTER =========================
// =================================================================

static void printExpr(Expr* expr, const char* prefix, bool isLast) {
    // Print the prefix for the current node, including the tree branch characters.
    printf("%s%s", prefix, isLast ? "`- " : "|- ");

    // If the expression is null (can happen on error), print and return.
    if (expr == NULL) {
        printf("NULL_EXPR\n");
        return;
    }

    // Determine the new prefix for this node's children.
    char newPrefix[256];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, isLast ? "   " : "|  ");

    // Switch on the expression type to print its specific details.
    switch (expr->type) {
        case EXPR_ASSIGN:
            printf("Assign: %.*s\n", expr->as.assign.name.length, expr->as.assign.name.start);
            printExpr(expr->as.assign.value, newPrefix, true);
            break;
        case EXPR_BINARY:
            printf("BinaryExpr: %.*s\n", expr->as.binary.op.length, expr->as.binary.op.start);
            printExpr(expr->as.binary.left, newPrefix, false);
            printExpr(expr->as.binary.right, newPrefix, true);
            break;
        case EXPR_GROUPING:
            printf("Grouping\n");
            printExpr(expr->as.grouping.expression, newPrefix, true);
            break;
        case EXPR_LITERAL:
            printf("Literal: %d\n", expr->as.literal.value);
            break;
        case EXPR_UNARY:
            printf("UnaryExpr: %.*s\n", expr->as.unary.op.length, expr->as.unary.op.start);
            printExpr(expr->as.unary.right, newPrefix, true);
            break;
        case EXPR_VARIABLE:
            printf("Variable: %.*s\n", expr->as.variable.name.length, expr->as.variable.name.start);
            break;
    }
}


// =================================================================
// ==================== STATEMENT PRINTER ==========================
// =================================================================

static void printStmt(Stmt* stmt, const char* prefix, bool isLast) {
    printf("%s%s", prefix, isLast ? "`- " : "|- ");
    
    if (stmt == NULL) {
        printf("NULL_STMT\n");
        return;
    }

    char newPrefix[256];
    strcpy(newPrefix, prefix);
    strcat(newPrefix, isLast ? "   " : "|  ");

    switch (stmt->type) {
        // case STMT_BLOCK:
        //     printf("Block\n");
        //     for (int i = 0; i < stmt->as.block.count; i++) {
        //         printStmt(stmt->as.block.statements[i], newPrefix, i == stmt->as.block.count - 1);
        //     }
        //     break;
        case STMT_EXPRESSION:
            printf("ExpressionStmt\n");
            printExpr(stmt->as.expression.expression, newPrefix, true);
            break;
        case STMT_IF:
            printf("IfStmt\n");
            // Print the condition as the first child.
            printExpr(stmt->as.ifStmt.condition, newPrefix, false);
            // Print the "then" branch. It's the last child only if there's no "else".
            printStmt(stmt->as.ifStmt.thenBranch, newPrefix, stmt->as.ifStmt.elseBranch == NULL);
            // If an "else" branch exists, print it as the last child.
            if (stmt->as.ifStmt.elseBranch != NULL) {
                printStmt(stmt->as.ifStmt.elseBranch, newPrefix, true);
            }
            break;
        case STMT_PRINT:
            printf("PrintStmt\n");
            printExpr(stmt->as.print.expression, newPrefix, true);
            break;
        case STMT_VAR_DECLARATION:
            printf("VarDecl: %.*s\n", stmt->as.var.name.length, stmt->as.var.name.start);
            if (stmt->as.var.initializer != NULL) {
                printExpr(stmt->as.var.initializer, newPrefix, true);
            }
            break;
        case STMT_WHILE:
            printf("WhileStmt\n");
            printExpr(stmt->as.whileStmt.condition, newPrefix, false);
            printStmt(stmt->as.whileStmt.body, newPrefix, true);
            break;
    }
}


// =================================================================
// ==================== PUBLIC INTERFACE ===========================
// =================================================================

void printAst(Stmt** statements, int count) {
    printf("--- Abstract Syntax Tree ---\n");
    if (count == 0 || statements == NULL) {
        printf(" (No statements)\n");
    } else {
        // Begin the process for each top-level statement in the program.
        for (int i = 0; i < count; i++) {
            printStmt(statements[i], "", i == count - 1);
        }
    }
    printf("--------------------------\n");
}

