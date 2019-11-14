#include "flexon.h"
#include "flexon.tab.h"

void yywarning(const char *s) {
  fprintf(stdout, "Warning: %s\n", s);
}

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
  exit(1);
}

UniversalType* newType(char *type, int size) {
  UniversalType *new = (UniversalType*)malloc(sizeof(UniversalType));
  new->type = type;
  new->size = size;

  return new;
}

IDList* addIDToList(IDList *list, char *id) {
  if (list == NULL) {
    list = (IDList*)malloc(sizeof(IDList));
    list->size = 0;
  }

  list->ids[list->size] = id;
  list->size++;

  return list;
}

DeclaredID* newDeclaredID(char *name, UniversalType *type) {
  DeclaredID *new = (DeclaredID*)malloc(sizeof(DeclaredID));
  new->name = name;
  new->type = type;

  return new;
}

DeclaredIDList* addDeclaredIDToList(DeclaredIDList *list, DeclaredID *decl_id) {
  if (list == NULL) {
    list = (DeclaredIDList*)malloc(sizeof(DeclaredIDList));
    list->size = 0;
  }

  list->decl_ids[list->size] = decl_id;
  list->size++;

  return list;
}

void freeUniversalType(UniversalType *type) {
  free(type->type);
  free(type);
}

void freeIDList(IDList *list) {
  for (int i = 0; i < list->size; i++) {
    free(list->ids[i]);
  }
  free(list);
}

void freeDeclaredID(DeclaredID *decl_id) {
  free(decl_id->name);
  freeUniversalType(decl_id->type);
  free(decl_id);
}

void freeDeclaredIDList(DeclaredIDList *list) {
  for (int i = 0; i < list->size; i++) {
    freeDeclaredID(list->decl_ids[i]);
  }
  free(list);
}
