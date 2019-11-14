%{
#include "flexon.h"
%}

%union {
	int ival;
	float fval;
	char *sval;
	UniversalType *utval;
}

/* declare tokens */
%token<sval> Mainprog Function Procedure Begin End If Then Else Nop While Return Print In For Elif
%token<sval> IntType
%token<sval> FloatType
%token<ival> Integer
%token<fval> Float
%token<sval> Comparator
%token<sval> ID

%left Comparator In
%left '+' '-'
%left '*' '/'
%right '=' '!'

%nonassoc FAKE_NO_ELSE
%nonassoc FAKE_NO_RELOP
%nonassoc Elif
%nonassoc Else

%type<sval> '+' '-' '*' '/'
%type<sval> sign relop addop multop
%type<sval> standard_type
%type<utval> type

%start program

%%

program: Mainprog ID ';' declarations subprogram_declarations compound_statement
;

declarations:	// epsilon
		| type identifier_list ';' declarations
;

identifier_list: ID
		| ID ',' identifier_list
;

type: standard_type			{ $$ = newType($1, 0); }
	| standard_type '[' Integer ']'	{ $$ = newType($1, $3); }
;

standard_type: IntType		{ $$ = $1; }
		| FloatType	{ $$ = $1; }
;

subprogram_declarations: // epsilon
			| subprogram_declaration subprogram_declarations
;

subprogram_declaration: subprogram_head declarations compound_statement
;

subprogram_head: Function ID arguments ':' standard_type ';'
		| Procedure ID arguments ';'
;

arguments: // epsilon
	| '(' parameter_list ')'
;

parameter_list: identifier_list ':' type
		| identifier_list ':' type ';' parameter_list
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

sign: '+'	{ $$ = $1; }
	| '-' 	{ $$ = $1; }
;

relop: Comparator	{ $$ = $1; }
	| In		{ $$ = $1; }
;

addop: '+'	{ $$ = $1; }
	| '-'	{ $$ = $1; }
;

multop: '*'	{ $$ = $1; }
	| '/'	{ $$ = $1; }
;

%%

int main(int argc, char **argv) {
	// invalid argument
	if (argc != 2) {
		printf("Invalid command line arguments.");
		return 1;
	}

	FILE *file;

	// invalid file name
	if ((file = fopen(argv[1], "r")) == NULL) {
		printf("File is not exist.");
		return 1;
	}

	yydebug = 1;
	yyin = file;
	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}
