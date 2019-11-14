#ifndef FLEXON_FLEXON_H
#define FLEXON_FLEXON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define YYDEBUG 1 // debug mode

extern int yylineno;
extern int yylex();
extern int yyparse();
extern FILE *yyin;

/**
 * Struct for all types
 *
 * Ex)
 *  If `int`, type = "int" and size = 0
 *  If `float[5]`, type = "float" and size = 5
 */
typedef struct universal_type {
  char *type;
  int size;
} UniversalType;

/**
 * Struct for the non-terminal `identifier_list`
 */
typedef struct id_list {
  int size;
  char *ids[64];
} IDList;

/**
 * Struct for the declared id
 *
 * Ex)
 *  If `int[5] num` then name = "num", type->type = "int" and type->size = 5
 */
typedef struct declared_id {
  char *name;
  UniversalType *type;
} DeclaredID;

/**
 * Struct for the declared id list
 */
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

void freeUniversalType(UniversalType *type);
void freeIDList(IDList *list);
void freeDeclaredID(DeclaredID *decl_id);
void freeDeclaredIDList(DeclaredIDList *list);

#endif //FLEXON_FLEXON_H
