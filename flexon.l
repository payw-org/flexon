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
":" { yylval.sval = strdup(yytext); return yytext[0]; }

 /* comparison operators */
"<" |
"<=" |
">=" |
">" |
"==" |
"!=" { yylval.sval = strdup(yytext); return Comparator; }

 /* keywords */
"mainprog"      { yylval.sval = strdup(yytext); return Mainprog; }
"function"      { yylval.sval = strdup(yytext); return Function; }
"procedure"     { yylval.sval = strdup(yytext); return Procedure; }
"begin"         { yylval.sval = strdup(yytext); return Begin; }
"end"           { yylval.sval = strdup(yytext); return End; }
"if"            { yylval.sval = strdup(yytext); return If; }
"then"          { yylval.sval = strdup(yytext); return Then; }
"else"          { yylval.sval = strdup(yytext); return Else; }
"nop"           { yylval.sval = strdup(yytext); return Nop; }
"while"         { yylval.sval = strdup(yytext); return While; }
"return"        { yylval.sval = strdup(yytext); return Return; }
"print"         { yylval.sval = strdup(yytext); return Print; }
"in"            { yylval.sval = strdup(yytext); return In; }
"for"           { yylval.sval = strdup(yytext); return For; }
"elif"          { yylval.sval = strdup(yytext); return Elif; }

 /* type */
"int"           { yylval.sval = strdup(yytext); return IntType; }
"float"         { yylval.sval = strdup(yytext); return FloatType; }

{Letter}({Letter}|{Digit})* { yylval.sval = strdup(yytext); return ID; }
{Digit}+ { yylval.ival = atoi(yytext); return Integer; }
{Digit}+\.{Digit}* { yylval.fval = atof(yytext); return Float; }

 /* things to ignore */
"//".* ;    // comments
[ \t\r\n] ;	// all whitespace
\\\n ;  // line continuation

.   { fprintf(stderr, "Line %d: Unknown character %c\n", yylineno, yytext[0]); exit(1); }
%%
