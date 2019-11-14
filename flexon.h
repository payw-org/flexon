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

typedef struct id_list {
  int size;
  char *ids[64];
} IDList;

void yywarning(const char *s);
void yyerror(const char *s);

UniversalType* newType(char *type, int size);
IDList* addIDToList(IDList *list, char *id);
#endif //FLEXON_FLEXON_H
