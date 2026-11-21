%{
#include "myscanner.h"
#include "config.h"
#include "sintatico.tab.h"
%}

%%
"else"          return T_ELSE;
"if"            return T_IF;
"int"           return T_INT;
"return"        return T_RETURN;
"void"          return T_VOID;
"while"         return T_WHILE;

"<="            return T_LTE;
"=="            return T_EQ;
"!="            return T_NEQ;

"+"             return ADD;
"-"             return SUB;
"*"             return MUL;
"/"             return DIV;
"<"             return LT;
">"             return GT;
"="             return ATRIB;
";"             return PVIRG;
","             return VIRG;
"("             return LPAR;
")"             return RPAR;
"["             return LCOL;
"]"             return RCOL;
"{"             return LCHA;
"}"             return RCHA;

[A-z]+                  return T_ID;
[0-9]+                  return T_NUM;
\/\*([ -~]|\n)*\*\/     printf("this is a comment\n");

[ \n\t]         return WHITE;

.               printf("unexpected character\n");
%%

int yywrap(void){
	return 1;
}