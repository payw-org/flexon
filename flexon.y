%{
#include "flexon.h"

Collector *collector;	// collector for the program's all identifiers
int is_global_decl_scope;  // flag for indicating global declaration scope
int is_global_stmt_scope; // flag for indicating global statement scope
int is_returned; // flag for checking whether if function is returned
%}

%union {
  int ival;
  float fval;
  char *sval;
  UniversalType *utval;
  UniversalTypeList *utlval;
  IDList *ilval;
  DeclaredID *dival;
  DeclaredIDList *dilval;
  DeclaredFunction *dfval;
}

/* declare tokens */
%token<sval> Mainprog Function Procedure Begin End If Then Else Nop While Return Print In For Elif
%token<sval> IntType
%token<sval> FloatType
%token<ival> Integer
%token<fval> Float
%token<sval> Comparator
%token<sval> ID

/* Define precedences */
%left Comparator In
%left '+' '-'
%left '*' '/'
%right '=' '!'
%nonassoc FAKE_NO_ELSE
%nonassoc FAKE_NO_RELOP
%nonassoc Elif
%nonassoc Else

/* Define types */
%type<sval> '+' '-' '*' '/'
%type<sval> sign relop addop multop
%type<sval> standard_type
%type<utval> type factor term expression simple_expression
%type<utlval> expression_list actual_parameter_expression
%type<ilval> identifier_list
%type<dival> variable
%type<dfval> procedure_statement

/* Indicate start state */
%start program

%%

program: Mainprog ID ';' declarations subprogram_declarations compound_statement
;

declarations: // epsilon
| declaration declarations
;

declaration: type identifier_list ';' {
  if (is_global_decl_scope) {  // collect global variables
    collectGlobalVars(&collector, $1, $2);
  } else {  // collect local variables
    collectLocalVars(&collector, $1, $2);
  }
}
;

identifier_list: ID {
  // initialize
  $$ = newIDList(yylineno);
  addIDToList(&$$, $1);
}
| ID ',' identifier_list  {
  addIDToList(&$3, $1);
  $$ = $3;
}
;

type: standard_type  {
  $$ = newType($1, -1);
}
| standard_type '[' Integer ']'	{
  $$ = newType($1, $3);
}
;

standard_type: IntType {
  $$ = $1;
}
| FloatType {
  $$ = $1;
}
;

subprogram_declarations: // epsilon
| subprogram_declaration subprogram_declarations
;

subprogram_declaration: subprogram_head declarations compound_statement {
  // deepcopy local vars to DeclaredFunction before removed
  copyLocalVarsToCurrFunc(&collector);

  // initialize arguments and local ids at the end of subprogram declaration
  freeDeclaredIDList(collector->arguments);
  freeDeclaredIDList(collector->local_vars);
  collector->arguments = newDeclaredIDList();
  collector->local_vars = newDeclaredIDList();

  // temporarily set flag which indicate global statements scope
  is_global_stmt_scope = 1;
}
;

subprogram_head: Function ID arguments ':' standard_type ';' {
  is_global_decl_scope = 0;
  is_global_stmt_scope = 0;
  collectFuncs(&collector, $2, $5, yylineno);
}
| Procedure ID arguments ';' {
  is_global_decl_scope = 0;
  is_global_stmt_scope = 0;
  collectFuncs(&collector, $2, NULL, yylineno);
}
;

arguments: // epsilon
| '(' parameter_list ')'
;

parameter_list: parameter
| parameter ';' parameter_list
;

parameter: identifier_list ':' type {
  collectArguments(&collector, $3, $1);
}

compound_statement: Begin statement_list End
;

statement_list: statement
| statement ';' statement_list
;

statement: variable '=' expression
| print_statement
| procedure_statement
| compound_statement
| if_statement
| while_statement
| for_statement
| Return expression
| Nop
;

if_statement: If expression ':' statement elif_statement
;

elif_statement: %prec FAKE_NO_ELSE
| else_statement
| Elif expression ':' statement elif_statement
;

else_statement: Else ':' statement
;

while_statement: While expression ':' statement %prec FAKE_NO_ELSE
| While expression ':' statement else_statement
;

for_statement: For expression In expression ':' statement %prec FAKE_NO_ELSE
| For expression In expression ':' statement else_statement
;

print_statement: Print
| Print '(' expression ')'
;

variable: ID {
  DeclaredID *decl_id;
  if (is_global_stmt_scope) { // global scope
    decl_id = checkIDInGlobalStmt(collector, $1, 0, yylineno);
  } else { // local scope
    decl_id = checkIDInLocalStmt(collector, $1, 0, yylineno);
  }

  $$ = decl_id;
}
| ID '[' expression ']' {
  DeclaredID *decl_id;
  if (is_global_stmt_scope) { // global scope
    decl_id = checkIDInGlobalStmt(collector, $1, 1, yylineno);
  } else { // local scope
    decl_id = checkIDInLocalStmt(collector, $1, 1, yylineno);
  }
}
;

procedure_statement: ID '(' actual_parameter_expression ')'
;

actual_parameter_expression: // epsilon
| expression_list
;

expression_list: expression
| expression ',' expression_list
;

expression: simple_expression %prec FAKE_NO_RELOP
| simple_expression relop simple_expression
;

simple_expression: term {
  $$ = $1;
}
| term addop simple_expression {
  if ($1 == NULL && $3 == NULL) {
    $$ = NULL;
  } else if ($1 != NULL && $3 == NULL) {
    $$ = incompatibleArrayUsageError($1, yylineno);
  } else if ($1 == NULL && $3 != NULL) {
    $$ = incompatibleArrayUsageError($3, yylineno);
  } else {
    if (
      incompatibleArrayUsageError($1, yylineno) == NULL ||
      incompatibleArrayUsageError($3, yylineno) == NULL
    ) {
      $$ = NULL;
    } else {
      if (strcmp($1->type, "int") == 0 && strcmp($3->type, "int") == 0) {
        $$ = newType("int", -1);
      } else {
        $$ = newType("float", -1);
      }
    }
  }
}
;

term: factor {
  $$ = $1;
}
| factor multop term {
  if ($1 == NULL && $3 == NULL) {
    $$ = NULL;
  } else if ($1 != NULL && $3 == NULL) {
    $$ = incompatibleArrayUsageError($1, yylineno);
  } else if ($1 == NULL && $3 != NULL) {
    $$ = incompatibleArrayUsageError($3, yylineno);
  } else {
    if (
      incompatibleArrayUsageError($1, yylineno) == NULL ||
      incompatibleArrayUsageError($3, yylineno) == NULL
    ) {
      $$ = NULL;
    } else {
      if (strcmp($1->type, "int") == 0 && strcmp($3->type, "int") == 0) {
        $$ = newType("int", -1);
      } else {
        $$ = newType("float", -1);
      }
    }
  }
}
;

factor: Integer {
  $$ = newType("int", -1);
}
| Float {
  $$ = newType("float", -1);
}
| variable {
  if ($1 == NULL) {
    $$ = NULL;
  } else {
    $$ = $1->type;
  }
}
| procedure_statement {
  if ($1 == NULL) {
    $$ = NULL;
  } else if ($1->return_type == NULL) {
    yaccError(yylineno, "Procedure \"%s\" not have return value (incompatible usage)", $1->name);
    $$ = NULL;
  } else {
    $$ = newType($1->return_type, -1);
  }
}
| '!' factor {
  $$ = newType("int", -1);
}
| sign factor {
  if ($2 == NULL) {
    $$ = NULL;
  } else {
    $$ = incompatibleArrayUsageError($2, yylineno);
  }
}
;

sign: '+' {
  $$ = $1;
}
| '-' {
  $$ = $1;
}
;

relop: Comparator {
  $$ = $1;
}
| In {
  $$ = $1;
}
;

addop: '+' {
  $$ = $1;
}
| '-' {
  $$ = $1;
}
;

multop: '*' {
  $$ = $1;
}
| '/' {
  $$ = $1;
}
;

%%

int main(int argc, char **argv) {
	// invalid argument
	if (argc != 2) {
		fprintf(stderr, "Invalid command line arguments.");
		return 1;
	}

	FILE *file;

	// invalid file name
	if ((file = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "File is not exist.");
		return 1;
	}

	// initialize global variables
	collector = newCollector();
	is_global_decl_scope = 1;
	is_global_stmt_scope = 1;
	is_returned = 0;

	// start parsing
//	yydebug = 1;
	yyin = file;
	do {
		yyparse();
	} while(!feof(yyin));

	printCollector(collector);
	freeCollector(collector);

	return 0;
}
