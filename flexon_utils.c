#include "flexon.h"
#include "flexon.tab.h"

void yywarning(const char *s) {
  fprintf(stdout, "Warning: %s\n", s);
}

void yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
  exit(1);
}
