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

