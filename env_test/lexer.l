%option noyywrap
%{
#include <iostream>
%}

%%
[0-9]+      { std::cout << "NUMBER: " << yytext << std::endl; }
[a-zA-Z]+   { std::cout << "WORD: " << yytext << std::endl; }
[ \t\n\r]+  { /* Discard standard whitespace */ }
.           { std::cout << "UNKNOWN: " << yytext << std::endl; }
%%