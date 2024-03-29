#ifndef FLEXON_FLEXON_H
#define FLEXON_FLEXON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

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
 * Struct for the universal type list
 */
typedef struct universal_type_list {
  int size;
  UniversalType *types[128];
} UniversalTypeList;

/**
 * Struct for the non-terminal `identifier_list`
 */
typedef struct id_list {
  int size;
  char *ids[64];
  int decl_lineno;
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

typedef struct declared_function {
  char *name;
  DeclaredIDList *arguments;
  DeclaredIDList *local_vars;
  char *return_type;
} DeclaredFunction;

typedef struct declared_function_list {
  int size;
  DeclaredFunction *decl_funcs[128];
} DeclaredFunctionList;

typedef struct collector {
  DeclaredIDList *global_vars;  // global variable list
  DeclaredIDList *arguments;  // function argument list
  DeclaredIDList *local_vars; // local variable list
  DeclaredFunctionList *funcs;  // function & procedure list
} Collector;

void yyerror(const char *);
void yaccError(int lineno, char *s, ...);

IDList* newIDList(int lineno);
UniversalType* newType(char *type, int size);
UniversalTypeList* newTypeList();
DeclaredID* newDeclaredID(char *name, UniversalType *type);
DeclaredIDList* newDeclaredIDList();
DeclaredFunction* newDeclaredFunction(char *name, DeclaredIDList *arguments, DeclaredIDList *local_vars, char *return_type);
DeclaredFunctionList* newDeclaredFunctionList();
Collector* newCollector();

UniversalType* copyType(UniversalType *type);
DeclaredID* copyDeclaredID(DeclaredID *decl_id);
DeclaredIDList* copyDeclaredIDList(DeclaredIDList *list);

void addTypeToList(UniversalTypeList **list, UniversalType *type);
void addIDToList(IDList **list, char *id);
void addDeclaredIDToList(DeclaredIDList **list, DeclaredID *decl_id);
void addDeclaredFunctionToList(DeclaredFunctionList **list, DeclaredFunction *decl_func);

void collectGlobalVars(Collector **collector, UniversalType *type, IDList *id_list);
void collectArguments(Collector **collector, UniversalType *type, IDList *id_list);
void collectLocalVars(Collector **collector, UniversalType *type, IDList *id_list);
void collectFuncs(Collector **collector, char *name, char *return_type, int lineno);

void copyLocalVarsToCurrFunc(Collector **collector);
UniversalType* incompatibleArrayUsageError(UniversalType *type, int lineno);

DeclaredFunction* checkFunc(Collector *collector, char *id, UniversalTypeList *param_type_list, int lineno);
DeclaredID* checkIDInGlobalStmt(Collector *collector, char *id, int is_array, int lineno);
DeclaredID* checkIDInLocalStmt(Collector *collector, char *id, int is_array, int lineno);
int isCompatible(DeclaredID *decl_id, int is_array, int lineno);

void printUniversalType(UniversalType *type);
void printDeclaredID(DeclaredID *decl_id);
void printDeclaredIDList(DeclaredIDList *list);
void printDeclaredFunction(DeclaredFunction *decl_func);
void printDeclaredFunctionList(DeclaredFunctionList *list);
void printCollector(Collector *collector);

void freeUniversalType(UniversalType *type);
void freeIDList(IDList *list);
void freeDeclaredID(DeclaredID *decl_id);
void freeDeclaredIDList(DeclaredIDList *list);
void freeDeclaredFunction(DeclaredFunction *decl_func);
void freeDeclaredFunctionList(DeclaredFunctionList *list);
void freeCollector(Collector *collector);

#endif //FLEXON_FLEXON_H
