%{
#include <stdio.h>
#include "flexon.tab.h"
%}
Letter [a-zA-Z]
Digit [0-9]


%option noyywrap
%%
{Letter}({Letter}|{Digit})* { return ID; }
{Digit}+ { return Integer; }
{Digit}+\.{Digit}* { return Float; }

"+" | "-" | "*" | "/" | "<" | "<=" | ">=" | ">" | "==" | "!=" | "!" { return Operator; }

"mainprog" | "function" | "procedure" | "begin" | "end" | "if" | "then" | "else" | "nop" | "while" | "return" | "print" | "in" { return Keyword; }

";" | "." | "," | "=" | "(" | ")" | "[" | "]" | ":" { return Delimiter; }

[ \t] ;	// ignore all whitespace
\n { return Newline; } 
%%

