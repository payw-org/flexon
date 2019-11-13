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

typedef struct declared_id_list {
  int size;
  char *ids[1000];
} DeclaredIdList;

void declareID(DeclaredIdList *decl_list, char *id);
int isDeclared(DeclaredIdList *decl_list, char *id);

void yywarning(const char *s);
void yyerror(const char *s);

#endif //FLEXON_FLEXON_H
