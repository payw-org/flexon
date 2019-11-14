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

typedef struct universal_type {
  char *type;
  int size;
} UniversalType;

void yywarning(const char *s);
void yyerror(const char *s);

UniversalType* newType(char *type, int size);
#endif //FLEXON_FLEXON_H
