## Grammar for the language (BNF Backus Naur Formas)
#### program ::= declaration*
#### declaration ::= var_declaration | statement
#### statement ::= expr_statement | if_statement | print_statement | while_statement ;

#### var_declaration ::= int [identifier] ( "=" expression )? ";" ;

#### identifier ::= /[ _a-zA-Z ]w*/
#### expr_statement ::= expression ";" ;
#### if_statement ::= "if" "(" expression ")" statement ("else" statement)?;
#### print_statement ::= "print" expression ";";
#### while_statement ::= "while" "(" expression ")" statement ;

#### expression ::= assignment;
#### assignment ::= IDENTIFIER "=" assignment | equality;
#### equality ::= comparison (( "!=" | "==" ) comparison )*;
#### comparison ::= term (( ">" | ">=" | "<" | "<=") term)*
#### term ::= factor (( "-" | "+" ) factor)* ;
#### factor ::= unary(( "/" | "*" ) unary)* ;
#### unary ::= ("-"|"!") unary | primary ;
#### primary ::= INTEGER_LITERAL | IDETIFIER | "(" expression ")";


