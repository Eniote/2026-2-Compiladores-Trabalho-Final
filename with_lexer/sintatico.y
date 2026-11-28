%{
#include <stdio.h>
#include <stdlib.h>
#include "myscanner.h"
#include "config.h"

extern int yylex();
void yyerror(const char *s);
%}

/* Definição dos Tokens */
%token T_IF T_ELSE T_INT T_RETURN T_VOID T_WHILE
%token T_ID T_NUM
%token T_LTE T_GTE T_EQ T_NEQ

/* Resolução da ambiguidade do 'dangling-else' (else solto) */
%nonassoc LOWER_THAN_ELSE
%nonassoc T_ELSE

/* Símbolo inicial da gramática */
%start program

%%

program:
    declaration_list
    ;

declaration_list:
    declaration_list declaration
    | declaration
    ;

declaration:
    var_declaration
    | fun_declaration
    ;

var_declaration:
    type_specifier T_ID ';'
    | type_specifier T_ID '[' T_NUM ']' ';'
    ;

type_specifier:
    T_INT
    | T_VOID
    ;

fun_declaration:
    type_specifier T_ID '(' params ')' compound_stmt
    ;

params:
    param_list
    | T_VOID
    ;

param_list:
    param_list ',' param
    | param
    ;

param:
    type_specifier T_ID
    | type_specifier T_ID '[' ']'
    ;

compound_stmt:
    '{' local_declarations statement_list '}'
    ;

local_declarations:
    local_declarations var_declaration
    | /* vazio */
    ;

statement_list:
    statement_list statement
    | /* vazio */
    ;

statement:
    expression_stmt
    | compound_stmt
    | selection_stmt
    | iteration_stmt
    | return_stmt
    ;

expression_stmt:
    expression ';'
    | ';'
    ;

selection_stmt:
    T_IF '(' expression ')' statement %prec LOWER_THAN_ELSE
    | T_IF '(' expression ')' statement T_ELSE statement
    ;

iteration_stmt:
    T_WHILE '(' expression ')' statement
    ;

return_stmt:
    T_RETURN ';'
    | T_RETURN expression ';'
    ;

expression:
    var '=' expression
    | simple_expression
    ;

var:
    T_ID
    | T_ID '[' expression ']'
    ;

simple_expression:
    additive_expression relop additive_expression
    | additive_expression
    ;

relop:
    T_LTE | '<' | '>' | T_GTE | T_EQ | T_NEQ
    ;

additive_expression:
    additive_expression addop term
    | term
    ;

addop:
    '+' | '-'
    ;

term:
    term mulop factor
    | factor
    ;

mulop:
    '*' | '/'
    ;

factor:
    '(' expression ')'
    | var
    | call
    | T_NUM
    ;

call:
    T_ID '(' args ')'
    ;

args:
    arg_list
    | /* vazio */
    ;

arg_list:
    arg_list ',' expression
    | expression
    ;

%%

/* Função chamada quando ocorre um erro de sintaxe */
void yyerror(const char *s) {
    fprintf(stderr, "Erro sintático: %s\n", s);
}

/* Função principal (Main) */
int main(void) {
    printf("Digite o código em C- (Pressione Ctrl+D no Linux ou Ctrl+Z no Windows para finalizar):\n");
    if (yyparse() == 0) {
        printf("\nAnálise sintática concluída com sucesso! O código é válido em C-.\n");
    }
    return 0;
}