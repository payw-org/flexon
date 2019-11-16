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
 * Print this type to standard output.
 * Ex)
 *  `int[5]`
 *
 * @param type
 */
void printUniversalType(UniversalType *type) {
  if (type == NULL) {
    return;
  }

  printf("%s", type->type);
  if (type->size > 0) {
    printf("[%d]", type->size);
  }
}

/**
 * Print this type to standard output.
 * Ex)
 *  `int[5] arr`
 *
 * @param type
 */
void printDeclaredID(DeclaredID *decl_id) {
  if (decl_id == NULL) {
    return;
  }

  printUniversalType(decl_id->type);
  printf(" ");
  printf("%s", decl_id->name);
}

/**
 * Print this type to standard output.
 * Ex)
 *  `int[5] arr, float var, float[3] arr2`
 *
 * @param type
 */
void printDeclaredIDList(DeclaredIDList *list) {
  if (list == NULL) {
    return;
  }

  int i;
  for (i = 0; i < list->size - 1; i++) {
    printDeclaredID(list->decl_ids[i]);
    printf(", ");
  }
  printDeclaredID(list->decl_ids[i]);
}

/**
 * Print this type to standard output.
 * Ex)
 *  `<Function>
 *    name: func
 *    return type: int
 *    params: int[5] arr, float var, float[3] arr2
 *  `
 *
 * @param type
 */
void printDeclaredFunction(DeclaredFunction *decl_func) {
  if (decl_func == NULL) {
    return;
  }

  printf("<Function>\n");
  printf("\tname: %s\n", decl_func->name);
  printf("\treturn type: %s\n", decl_func->return_type);
  printf("\tparams: ");
  printDeclaredIDList(decl_func->parameters);
  printf("\n");
}

/**
 * Print this type to standard output.
 * Ex)
 *  `<Function>
 *    name: func
 *    return type: int
 *    params: int[5] arr, float var, float[3] arr2
 *  <Function>
 *    name: func2
 *    return type: float
 *    params: int[5] arr3, float var2, float[3] arr4
 *  `
 *
 * @param type
 */
void printDeclaredFunctionList(DeclaredFunctionList *list) {
  if (list == NULL) {
    return;
  }

  for (int i = 0; i < list->size; i++) {
    printDeclaredFunction(list->decl_funcs[i]);
  }
}

/**
 * Print this type to standard output.
 * Ex)
 *  `<Global variables>
 *    int[5] arr, float var, float[3] arr2
 *  <Local variables>
 *    int[3] arr3, float var2, float[2] arr4
 *  <Function>
 *    name: func
 *    return type: int
 *    params: int[5] arr5, float var3, float[3] arr6
 *  <Function>
 *    name: func2
 *    return type: float
 *    params: int[5] arr7, float var4, float[3] arr8
 *  `
 *
 * @param type
 */
void printCollector(Collector *collector) {
  if (collector == NULL) {
    return;
  }

  printf("\n\n");
  printf("<Global variables>\n");
  printf("\t");
  printDeclaredIDList(collector->global_vars);
  printf("\n");
  printf("<Local variables>\n");
  printf("\t");
  printDeclaredIDList(collector->local_vars);
  printf("\n");
  printDeclaredFunctionList(collector->funcs);
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
//  freeUniversalType(decl_id->type); @TODO: resolve double free error
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
  free(decl_func);
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
  free(collector);
}
