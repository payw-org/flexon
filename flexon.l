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
{Digit}+ { return Integer; }
{Digit}+\.{Digit}* { return Float; }

"\n" { return Newline; }

 /* things to ignore */
"//".* ;    // comments
[ \t\r] ;	// all whitespace
\\\n ;  // line continuation

.   { fprintf(stderr, "Unknown character: %c\n", yytext[0]); exit(1); }
%%
