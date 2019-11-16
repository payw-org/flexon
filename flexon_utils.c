#include "flexon.h"
#include "flexon.tab.h"

/**
 * Print the warning statement.
 *
 * @param s
 */
void yywarning(const char *s) {
  fprintf(stdout, "Warning: %s\n", s);
}

/**
 * Print the error statement.
 *
 * @param s
 */
void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
  exit(1);
}

/**
 * Create the new type.
 *
 * @param type  Standard type
 * @param size  0 when non-array, if not this is array size
 * @return
 */
UniversalType* newType(char *type, int size) {
  UniversalType *new = (UniversalType*)malloc(sizeof(UniversalType));
  new->type = type;
  new->size = size;

  return new;
}

/**
 * Add new identifier to list.
 *
 * @param list
 * @param id
 * @return
 */
IDList* addIDToList(IDList *list, char *id) {
  if (list == NULL) {
    list = (IDList*)malloc(sizeof(IDList));
    list->size = 0;
  }

  list->ids[list->size] = id;
  list->size++;

  return list;
}

/**
 * Create the new declared id.
 *
 * @param name
 * @param type
 * @return
 */
DeclaredID* newDeclaredID(char *name, UniversalType *type) {
  DeclaredID *new = (DeclaredID*)malloc(sizeof(DeclaredID));
  new->name = name;
  new->type = type;

  return new;
}

/**
 * Add new declared id to list.
 *
 * @param list
 * @param decl_id
 * @return
 */
DeclaredIDList* addDeclaredIDToList(DeclaredIDList *list, DeclaredID *decl_id) {
  if (list == NULL) {
    list = (DeclaredIDList*)malloc(sizeof(DeclaredIDList));
    list->size = 0;
  }

  list->decl_ids[list->size] = decl_id;
  list->size++;

  return list;
}

DeclaredFunction* newDeclaredFunction(char *name, DeclaredIDList *parameters, char *return_type) {
  DeclaredFunction *new = (DeclaredFunction*)malloc(sizeof(DeclaredFunction));
  new->name = name;
  new->parameters = parameters;
  new->return_type = return_type;

  return new;
}

DeclaredFunctionList* addDeclaredFunctionToList(DeclaredFunctionList* list, DeclaredFunction *decl_func) {
  if (list == NULL) {
    list = (DeclaredFunctionList*)malloc(sizeof(DeclaredFunctionList));
    list->size = 0;
  }

  list->decl_funcs[list->size] = decl_func;
  list->size++;

  return list;
}

Collector* newCollector() {
  Collector *new = (Collector*)malloc(sizeof(Collector));
  new->global_vars = NULL;
  new->local_vars = NULL;
  new->funcs = NULL;

  return new;
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeUniversalType(UniversalType *type) {
  if (type == NULL) {
    return;
  }

  free(type->type);
  free(type);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeIDList(IDList *list) {
  if (list == NULL) {
    return;
  }

  for (int i = 0; i < list->size; i++) {
    free(list->ids[i]);
  }
  free(list);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeDeclaredID(DeclaredID *decl_id) {
  if (decl_id == NULL) {
    return;
  }

  free(decl_id->name);
  freeUniversalType(decl_id->type);
  free(decl_id);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeDeclaredIDList(DeclaredIDList *list) {
  if (list == NULL) {
    return;
  }

  for (int i = 0; i < list->size; i++) {
    freeDeclaredID(list->decl_ids[i]);
  }
  free(list);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeDeclaredFunction(DeclaredFunction *decl_func) {
  if (decl_func == NULL) {
    return;
  }

  free(decl_func->name);
  freeDeclaredIDList(decl_func->parameters);
  free(decl_func->return_type);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeDeclaredFunctionList(DeclaredFunctionList *list) {
  if (list == NULL) {
    return;
  }

  for (int i = 0; i < list->size; i++) {
    freeDeclaredFunction(list->decl_funcs[i]);
  }
  free(list);
}

/**
 * Deallocate memory for the type.
 *
 * @param type
 */
void freeCollector(Collector *collector) {
  if (collector == NULL) {
    return;
  }

  freeDeclaredIDList(collector->global_vars);
  freeDeclaredIDList(collector->local_vars);
  freeDeclaredFunctionList(collector->funcs);
}
