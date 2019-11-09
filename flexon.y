%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;

#define YYDEBUG 1

void yyerror(const char* s);
%}

/* declare tokens */
%token Integer Float
%token Keyword
%token Operator
%token Delimiter
%token Newline
%token ID

%left '>' ">=" '<' "<=" "==" "!=" "in"
%left '+' '-'
%left '*' '/'
%right '='

%nonassoc NO_ELSE
%nonassoc "elif"
%nonassoc "else"

%%

program: "mainprog" ID ';' declarations subprogram_declarations compound_statement
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

standard_type: "int"
		| "float"
;

subprogram_declarations: // epsilon
			| subprogram_declaration subprogram_declarations
;

subprogram_declaration: subprogram_head declarations compound_statement
;

subprogram_head: "function" ID arguments ':' standard_type ';'
		| "procedure" ID arguments ';'
;

arguments: // epsilon
	| '(' parameter_list ')'
;

parameter_list: identifier_list ':' type
		| identifier_list ':' type ';' parameter_list
;

compound_statement: "begin" statement_list "end"
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
	| "return" expression
	| "nop"
;

if_statement: "if" expression ':' statement elif_statement
;

elif_statement: %prec NO_ELSE
		| else_statement
		| "elif" expression ':' statement elif_statement
;

else_statement: "else" ':' statement
;

while_statement: "while" expression ':' statement %prec NO_ELSE
		| "while" expression ':' statement else_statement
;

for_statement: "for" expression "in" expression ':' statement %prec NO_ELSE
		| "for" expression "in" expression ':' statement else_statement
;

print_statement: "print"
		| "print" '(' expression ')'
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

relop: '>'
	| ">="
	| '<'
	| "<="
	| "=="
	| "!="
	| "in"
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

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}
