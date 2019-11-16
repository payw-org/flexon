%{
#include "flexon.h"

DeclaredIDList *global_ids;  // global id list
DeclaredIDList *local_ids;  // local id list
int end_of_global_decl;  // flag for detecting the end of global declaration

DeclaredFunctionList *functions; // function & procedure list
%}

%union {
  int ival;
  float fval;
  char *sval;
  UniversalType *utval;
  IDList *ilval;
  DeclaredIDList *dilval;
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
%type<utval> type
%type<ilval> identifier_list
%type<dilval> parameter_list arguments

/* Indicate start state */
%start program

%%

program: Mainprog ID ';' declarations subprogram_declarations compound_statement
;

declarations: // epsilon
| type identifier_list ';' declarations	{
  int i;
  if (end_of_global_decl == 0) {  // add to global ids
    for (i = 0; i < $2->size; i++) {
      global_ids = addDeclaredIDToList(global_ids, newDeclaredID($2->ids[i], $1));
    }
  } else {  // add to local ids
    for (i = 0; i < $2->size; i++) {
      local_ids = addDeclaredIDToList(local_ids, newDeclaredID($2->ids[i], $1));
    }
  }
}
;

identifier_list: ID {
  // initialize
  $$ = NULL;
  $$ = addIDToList($$, $1);
}
| ID ',' identifier_list  {
  $$ = addIDToList($3, $1);
}
;

type: standard_type  {
  $$ = newType($1, 0);
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
  // initialize local id list at the end of subprogram declaration
  freeDeclaredIDList(local_ids);
  local_ids = NULL;
}
;

subprogram_head: Function ID arguments ':' standard_type ';' {
  end_of_global_decl = 1;
  functions = addDeclaredFunctionToList(functions, newDeclaredFunction($2, $3, $5));
}
| Procedure ID arguments ';' {
  end_of_global_decl = 1;
  functions = addDeclaredFunctionToList(functions, newDeclaredFunction($2, $3, NULL));
}
;

arguments: {
  $$ = NULL;
}  // epsilon
| '(' parameter_list ')' {
  $$ = $2;
}
;

parameter_list: identifier_list ':' type {
  // initialize
  $$ = NULL;
  for (int i = 0; i < $1->size; i++) {
    $$ = addDeclaredIDToList($$, newDeclaredID($1->ids[i], $3));
  }
}
| identifier_list ':' type ';' parameter_list {
  $$ = $5;
  for (int i = 0; i < $1->size; i++) {
    $$ = addDeclaredIDToList($$, newDeclaredID($1->ids[i], $3));
  }
}
;

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

variable: ID
| ID '[' expression ']'
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

simple_expression: term
| term addop simple_expression
;

term: factor
| factor multop term
;

factor: Integer
| Float
| variable
| procedure_statement
| '!' factor
| sign factor
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
	global_ids = NULL;
	local_ids = NULL;
	functions = NULL;
	end_of_global_decl = 0;

	// start parsing
	yydebug = 1;
	yyin = file;
	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}
