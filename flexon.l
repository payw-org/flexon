%{
#include <stdio.h>
#include <string.h>
#include "flexon.tab.h"
%}
Letter [a-zA-Z]
Digit [0-9]

%option noyywrap yylineno
%%

 /* single character operators */
"+" |
"-" |
"*" |
"/" |
"!" |
";" |
"." |
"," |
"=" |
"(" |
")" |
"[" |
"]" |
":" { return yytext[0]; }

 /* comparison operators */
"<" |
"<=" |
">=" |
">" |
"==" |
"!=" { return Comparator; }

 /* keywords */
"mainprog"      { return Mainprog; }
"function"      { return Function; }
"procedure"     { return Procedure; }
"begin"         { return Begin; }
"end"           { return End; }
"if"            { return If; }
"then"          { return Then; }
"else"          { return Else; }
"nop"           { return Nop; }
"while"         { return While; }
"return"        { return Return; }
"print"         { return Print; }
"in"            { return In; }
"for"           { return For; }
"elif"          { return Elif; }

 /* type */
"int"           { return IntType; }
"float"         { return FloatType; }

{Letter}({Letter}|{Digit})* { return ID; }
{Digit}+ { yylval.ival = atoi(yytext); return Integer; }
{Digit}+\.{Digit}* { yylval.fval = atof(yytext); return Float; }

 /* things to ignore */
"//".* ;    // comments
[ \t\r\n] ;	// all whitespace
\\\n ;  // line continuation

.   { fprintf(stderr, "Line %d: Unknown character %c\n", yylineno, yytext[0]); exit(1); }
%%
