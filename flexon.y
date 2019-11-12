%{
#include <stdio.h>
#include <stdlib.h>

#define YYDEBUG 1

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yywarning(const char *s);
void yyerror(const char *s);
%}

/* declare tokens */
%token Mainprog Function Procedure Begin End If Then Else Nop While Return Print In For Elif
%token IntType FloatType
%token Integer Float
%token Comparator
%token Newline
%token ID

%left Comparator In
%left '+' '-'
%left '*' '/'
%right '=' '!'

%nonassoc NO_ELSE
%nonassoc Elif
%nonassoc Else

%%

program: Mainprog ID ';' declarations subprogram_declarations compound_statement
;

declarations:	// epsilon
		| type identifier_list ';' declarations
;

identifier_list: ID
		| ID ',' identifier_list
;

type: standard_type
	| standard_type '[' Integer ']'
;

standard_type: IntType
		| FloatType
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

elif_statement: %prec NO_ELSE
		| else_statement
		| Elif expression ':' statement elif_statement
;

else_statement: Else ':' statement
;

while_statement: While expression ':' statement %prec NO_ELSE
		| While expression ':' statement else_statement
;

for_statement: For expression In expression ':' statement %prec NO_ELSE
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

expression: simple_expression relop simple_expression
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

sign: '+'
	| '-'
;

relop: Comparator
	| In
;

addop: '+'
	| '-'
;

multop: '*'
	| '/'
;

%%

int main(int argc, char **argv) {
	// Invalid argument
	if (argc != 2) {
		printf("Invalid command line arguments.");
		return 1;
	}

	FILE *file;

	// Invalid file name
	if ((file = fopen(argv[1], "r")) == NULL) {
		printf("File is not exist.");
		return 1;
	}

//	// check file size
//	fseek(file, 0, SEEK_END);
//	long file_size = ftell(file);
//	fseek(file, 0, SEEK_SET);
//
//	// read the file at once
//	char *code = (char*)malloc(file_size + 1);
//	fread(code, 1, file_size, file);
//	fclose(file);
//
//	// set end of string
//	code[file_size] = 0;

	yydebug = 1;

	yyin = file;
	do {
		yyparse();
	} while(!feof(yyin));

	return 0;
}

void yywarning(const char *s) {
	fprintf(stdout, "Warning: %s\n", s);
}

void yyerror(const char *s) {
	fprintf(stderr, "Error: %s\n", s);
	exit(1);
}
