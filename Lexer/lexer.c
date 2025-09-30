#include "stdio.h"
#include "stdbool.h" 
#include "lexer.h"


void lexer_init(Lexer* lex,const char* source){
    lex->current = source;
    lex->start = source;
    lex->line = 1;
}

static bool is_alphabet(char ch){
    return (ch>='a' && ch<='z') || (ch>='A' && ch<='Z') || ch == '_';   
}

static bool is_digit(char ch){
    return ch>='0' && ch<='9';   
}

static bool is_end(char ch){
    return ch == '\0';   
}

static char advance(Lexer* lex){
    if(is_end(*lex->current))return '\0';
    lex->current++;
    return lex->current[-1];
}

static char peek(Lexer* lex){
    return *lex->current;
}

static char next_peek(Lexer* lex){
    if(is_end(*lex->current))return '\0';
    return *(lex->current+1);
}

//Skip the white spaces or new line
void skip_white_space(Lexer* lex){
    while(!is_end(peek(lex))){
        if(peek(lex) == ' ' || peek(lex) == '\t')advance(lex);
        else if (peek(lex) == '\n'){
            lex->line++;
            advance(lex);
        }else if(peek(lex) == '/' && next_peek(lex) == '/'){
            while(true){
                char ch = advance(lex);
                if(ch == '\n' || is_end(ch))break;
            }
        }
        else break;
    }
    return;
}


//Create a token 
Token create_token(Lexer* lex, token_type type){
    Token token;
    token.type = type;
    token.length = lex->current-lex->start;
    token.line = lex->line;
    token.start = lex->start;
    return token;
} 

// Integer literal token
Token token_number(Lexer* lex){
    while(is_digit(*lex->current))lex->current++;
    Token token;
    token.length = lex->current-lex->start;
    token.line = lex->line;
    token.start = lex->start;
    token.type = TOKEN_INTEGER;
    return token;
}

bool compare(const char* str1,int size1,const char* str2,int size2){
    if(size1!=size2)return false;
    for(int i = 0;i<size1;i++)if(str1[i]!=str2[i])return false;
    return true;
}

int get_size(const char* str){
    int i = 0;
    while(str[i]!='\0')i++;
    return i;
}

void check_keyword(Token* token){
    if(compare("if",2,token->start,token->length))token->type = TOKEN_IF;
    else if(compare("int",3,token->start,token->length))token->type = TOKEN_INT;
    else if(compare("else",4,token->start,token->length))token->type = TOKEN_ELSE;
    else if(compare("print",5,token->start,token->length))token->type = TOKEN_PRINT;
    else if(compare("while",5,token->start,token->length))token->type = TOKEN_WHILE;
    else return;
}

// Identifier token
Token token_identifier_keyword(Lexer* lex){
    while(is_digit(*lex->current) || is_alphabet(*lex->current))lex->current++;
    Token token;
    token.length = lex->current-lex->start;
    token.line = lex->line;
    token.start = lex->start;
    token.type = TOKEN_IDENTIFIER;
    check_keyword(&token);
    return token;
}

Token error_token(Lexer* lex,const char* str){
    Token err;
    err.line = lex->line;
    err.start = str;
    err.type = TOKEN_ERROR;
    err.length = get_size(str);
}

Token scan_token(Lexer* lex){
    skip_white_space(lex);
    lex->start = lex->current;
    char ch = advance(lex);

    if(is_digit(ch))return token_number(lex);
    if(is_alphabet(ch))return token_identifier_keyword(lex);
    switch (ch){
        //Single character tokens
        case '+': return create_token(lex,TOKEN_PLUS);
        case '-': return create_token(lex,TOKEN_MINUS);
        case '*': return create_token(lex,TOKEN_STAR);
        case '/': return create_token(lex,TOKEN_SLASH);
        case '(': return create_token(lex,TOKEN_OPEN_PARENTHESIS);
        case ')': return create_token(lex,TOKEN_CLOSE_PARENTHESIS);
        case '{': return create_token(lex,TOKEN_OPEN_BRACE);
        case '}': return create_token(lex,TOKEN_CLOSE_BRACE);
        case ';': return create_token(lex,TOKEN_SEMICOLON);
        case '\0': return create_token(lex,TOKEN_EOF);
        //One or  two character tokens
        case '!':{
            char chr = peek(lex);
            if(chr == '='){
                lex->current++;
                return create_token(lex,TOKEN_BANG_EQUAL);
            }else return create_token(lex,TOKEN_BANG);
        }
        case '=':{
            char chr = peek(lex);
            if(chr == '='){
                lex->current++;
                return create_token(lex,TOKEN_EQUAL_EQUAL);
            }return create_token(lex,TOKEN_EQUAL);
        }
        case '>':{
            char chr = peek(lex);
            if(chr == '='){
                lex->current++;
                return create_token(lex,TOKEN_GREATER_EQUAL);
            }else return create_token(lex,TOKEN_GREATER);
        }
        case '<':{
            char chr = peek(lex);
            if(chr == '='){
                lex->current++;
                return create_token(lex,TOKEN_SMALLER_EQUAL);
            }else return create_token(lex,TOKEN_SMALLER);
        }
    }
    return error_token(lex,"unexpected character");
}