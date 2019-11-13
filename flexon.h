#ifndef FLEXON_FLEXON_H
#define FLEXON_FLEXON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYDEBUG 1

extern int yylineno;
extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yywarning(const char *s);
void yyerror(const char *s);

#endif //FLEXON_FLEXON_H
