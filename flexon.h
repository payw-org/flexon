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

typedef struct declared_id {
  char *name;
  UniversalType *type;
} DeclaredID;

typedef struct declared_id_list {
  int size;
  DeclaredID *decl_ids[128];
} DeclaredIDList;

void yywarning(const char *s);
void yyerror(const char *s);

UniversalType* newType(char *type, int size);
IDList* addIDToList(IDList *list, char *id);
DeclaredID* newDeclaredID(char *name, UniversalType *type);
DeclaredIDList* addDeclaredIDToList(DeclaredIDList *list, DeclaredID *decl_id);

#endif //FLEXON_FLEXON_H
