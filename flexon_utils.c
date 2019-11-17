#include "flexon.h"
#include "flexon.tab.h"

/**
 * Default yacc error printer.
 *
 * @param s
 */
void yyerror (char const *s) {
  fprintf (stderr, "%s\n", s);
}

/**
 * Custom yacc error printer which show error occurred line number.
 *
 * @param s
 */
void yaccError(int lineno, char *s, ...) {
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "Line(%d) Error: ", lineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

/**
 * Create new identifier list.
 *
 * @param lineno Declared line number
 * @return
 */
IDList* newIDList(int lineno) {
  IDList *new = (IDList*)malloc(sizeof(IDList));
  new->size = 0;
  new->decl_lineno = lineno;

  return new;
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
 * Create declared id list.
 *
 * @return
 */
DeclaredIDList* newDeclaredIDList() {
  DeclaredIDList *new = (DeclaredIDList*)malloc(sizeof(DeclaredIDList));
  new->size = 0;

  return new;
}

/**
 * Create declared function.
 *
 * @param name
 * @param arguments
 * @param return_type
 * @return
 */
DeclaredFunction* newDeclaredFunction(char *name, DeclaredIDList *arguments, char *return_type) {
  DeclaredFunction *new = (DeclaredFunction*)malloc(sizeof(DeclaredFunction));
  new->name = name;
  new->arguments = arguments;
  new->return_type = return_type;

  return new;
}

/**
 * Create declared function list.
 *
 * @return
 */
DeclaredFunctionList* newDeclaredFunctionList() {
  DeclaredFunctionList *new = (DeclaredFunctionList*)malloc(sizeof(DeclaredFunctionList));
  new->size = 0;

  return new;
}

/**
 * Create collector.
 *
 * @return
 */
Collector* newCollector() {
  Collector *new = (Collector*)malloc(sizeof(Collector));
  new->global_vars = newDeclaredIDList();
  new->local_vars = newDeclaredIDList();
  new->funcs = newDeclaredFunctionList();

  return new;
}

/**
 * Add new identifier to list.
 *
 * @param list
 * @param id
 */
void addIDToList(IDList **list, char *id) {
  (*list)->ids[(*list)->size] = id;
  (*list)->size++;
}

/**
 * Add new declared id to list.
 *
 * @param list
 * @param decl_id
 */
void addDeclaredIDToList(DeclaredIDList **list, DeclaredID *decl_id) {
  (*list)->decl_ids[(*list)->size] = decl_id;
  (*list)->size++;
}

/**
 * Add new declared function to list.
 *
 * @param list
 * @param decl_func
 */
void addDeclaredFunctionToList(DeclaredFunctionList **list, DeclaredFunction *decl_func) {
  (*list)->decl_funcs[(*list)->size] = decl_func;
  (*list)->size++;
}

/**
 * Collect global variables.
 * If the variable is already declared, not collect it and report error.
 *
 * @param collector
 * @param type
 * @param id_list
 */
void collectGlobalVars(Collector **collector, UniversalType *type, IDList *id_list) {
  int i, j;
  int is_duplicate;
  char *new_name, *declared_name;

  for (i = id_list->size - 1; i >= 0; i--) {
    is_duplicate = 0;
    new_name = id_list->ids[i];
    for (j = 0; j < (*collector)->global_vars->size; j++) {
      declared_name = (*collector)->global_vars->decl_ids[j]->name;

      // already declared
      if (strcmp(new_name, declared_name) == 0) {
        is_duplicate = 1;
        yaccError(id_list->decl_lineno, "Duplicate variable name \"%s\"", new_name);
        break;
      }
    }

    if (!is_duplicate) {
      addDeclaredIDToList(&((*collector)->global_vars), newDeclaredID(new_name, type));
    }
  }
}

void collectLocalVars(Collector **collector, UniversalType *type, IDList *id_list) {
  int i, j;
  int is_duplicate;
  char *new_name, *declared_name;
  DeclaredFunction *curr_func = (*collector)->funcs->decl_funcs[(*collector)->funcs->size - 1];

  for (i = id_list->size - 1; i >= 0; i--) {
    is_duplicate = 0;
    new_name = id_list->ids[i];

    // check among the function arguments
    for (j = 0; j < curr_func->arguments->size; j++) {
      declared_name = curr_func->arguments->decl_ids[j]->name;

      // already declared
      if (strcmp(new_name, declared_name) == 0) {
        is_duplicate = 1;
        yaccError(id_list->decl_lineno, "Duplicate variable name \"%s\"", new_name);
        break;
      }
    }

    // check among the local variables
    if (!is_duplicate) {
      for (j = 0; j < (*collector)->local_vars->size; j++) {
        declared_name = (*collector)->local_vars->decl_ids[j]->name;

        // already declared
        if (strcmp(new_name, declared_name) == 0) {
          is_duplicate = 1;
          yaccError(id_list->decl_lineno, "Duplicate variable name \"%s\"", new_name);
          break;
        }
      }

      if (!is_duplicate) {
        addDeclaredIDToList(&((*collector)->local_vars), newDeclaredID(new_name, type));
      }
    }
  }
}

void collectFuncs(Collector **collector, char *name, DeclaredIDList *arguments, char *return_type, int lineno) {
  int i;
  DeclaredFunction *declared_func;

  for (i = 0; i < (*collector)->funcs->size; i++) {
    declared_func = (*collector)->funcs->decl_funcs[i];
    if (strcmp(name, declared_func->name) == 0) {
      yaccError(lineno, "Duplicate function name \"%s\"", name);
      return;
    }
  }

  addDeclaredFunctionToList(&((*collector)->funcs), newDeclaredFunction(name, arguments, return_type));
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
  if (list == NULL || list->size == 0) {
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
  printDeclaredIDList(decl_func->arguments);
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
  if (list == NULL || list->size == 0) {
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
  freeDeclaredIDList(decl_func->arguments);
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
