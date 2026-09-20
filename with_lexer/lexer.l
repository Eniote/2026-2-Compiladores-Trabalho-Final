%{
#include "myscanner.h"
#include "config.h"
%}

%%
"else"          return ELSE;
"if"            return IF;
"int"           return INT;
"return"        return RETURN;
"void"          return VOID;
"while"         return WHILE;

"<="            return LE;
">="            return GE;
"=="            return EQ;
"!="            return NE;

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

[A-z]+                  return ID;
[0-9]+                  return NUM;
\/\*([ -~]|\n)*\*\/     printf("this is a comment\n");

[ \n\t]         return WHITE;

.               printf("unexpected character\n");
%%

int yywrap(void){
	return 1;
}