%{

  #include <stdio.h> // For puts and gets

  #include "hkl_string.h"

  // These will be defined at link time
  extern int yylex();
  extern int yyerror(char const*);
%}

// YYSUNION Definition
%union
{
  int        integer;
  double     real;
  HklString* string;
}

/*
  Hkl Token List
*/

%token HKL_T_IF;
%token HKL_T_ELSE;
%token HKL_T_FOR;
%token HKL_T_WHILE;
%token HKL_T_END;
%token HKL_T_RETURN;
%token HKL_T_BREAK;
%token HKL_T_CONTINUE
%token HKL_T_ASSERT;
%token HKL_T_INCLUDE;

%token HKL_T_CLASS;
%token HKL_T_FUNCTION;
%token HKL_T_NIL;
%token HKL_T_NULL;
%token HKL_T_INT;
%token HKL_T_REAL;
%token HKL_T_STRING;
%token HKL_T_ARRAY;
%token HKL_T_HASH;

%token HKL_T_SELF;

%token HKL_T_UNIQUE;
%token HKL_T_CONSTANT;
%token HKL_T_PROTOTYPE;
%token HKL_T_PROTECTED;
%token HKL_T_LOCAL;
%token HKL_T_GLOBAL;

%token HKL_T_TRUE;
%token HKL_T_FALSE;

%token HKL_T_PUTS;
%token HKL_T_GETS;

%token HKL_T_LPAREN;
%token HKL_T_RPAREN;
%token HKL_T_LBRACE;
%token HKL_T_RBRACE;
%token HKL_T_LBRACKET;
%token HKL_T_RBRACKET;
%token HKL_T_COLON;
%token HKL_T_COMMA;
%token HKL_T_DOT;

%token HKL_T_INCREMENT;
%token HKL_T_DECREMENT;
%token HKL_T_OR;
%token HKL_T_AND;
%token HKL_T_LESS_EQUAL;
%token HKL_T_GREATER_EQUAL;
%token HKL_T_LESS;
%token HKL_T_GREATER;
%token HKL_T_EQUAL;
%token HKL_T_NOT_EQUAL;
%token HKL_T_PLUS_ASSIGN;
%token HKL_T_MINUS_ASSIGN;
%token HKL_T_ASTERISK_ASSIGN;
%token HKL_T_DIVIDE_ASSIGN;
%token HKL_T_MOD_ASSIGN;
%token HKL_T_BITWISE_AND_ASSIGN;
%token HKL_T_BITWISE_OR_ASSIGN;
%token HKL_T_BITWISE_XOR_ASSIGN;
%token HKL_T_BITWISE_NOT_ASSIGN;

%token HKL_T_ASSIGN;
%token HKL_T_NOT;
%token HKL_T_PLUS;
%token HKL_T_MINUS;
%token HKL_T_ASTERISK;
%token HKL_T_DIVIDE;
%token HKL_T_MOD;
%token HKL_T_BITWISE_AND;
%token HKL_T_BITWISE_OR;
%token HKL_T_BITWISE_XOR;
%token HKL_T_BITWISE_NOT;
%token HKL_T_TYPE_OF;
%token HKL_T_INSTANCE_OF;
%token HKL_T_RANGE;

%token <string>  HKL_T_ID
%token <integer> HKL_T_INT_CONSTANT
%token <real>    HKL_T_REAL_CONSTANT
%token <string>  HKL_T_STRING_CONSTANT

// Precedence
%nonassoc UNARY_OPS

%left HKL_T_OR
%left HKL_T_AND
%left HKL_T_BITWISE_OR
%left HKL_T_BITWISE_XOR
%left HKL_T_BITWISE_AND
%left HKL_T_EQUAL HKL_T_NOT_EQUAL
%left HKL_T_LESS HKL_T_GREATER HKL_T_LESS_EQUAL HKL_T_GREATER_EQUAL
%left HKL_T_PLUS HKL_T_MINUS
%left HKL_T_DIVIDE HKL_T_ASTERISK HKL_T_MOD
%left HKL_T_TYPE_OF HKL_T_INSTANCE_OF

// HKL Grammar
%%

program:
  stmt_list

stmt_list:
  stmt_list stmt
  | empty

stmt:
  puts_stmt
  | if_stmt
  | for_stmt
  | while_stmt
  | return_stmt
  | break_stmt
  | continue_stmt
  | assert_stmt
  | include_stmt
  | class_stmt
  | function_stmt
  | assign_stmt

puts_stmt:
  HKL_T_PUTS expression

if_stmt:
  HKL_T_IF HKL_T_LPAREN expression HKL_T_RPAREN stmt_list HKL_T_END
  | HKL_T_IF HKL_T_LPAREN expression HKL_T_RPAREN stmt_list HKL_T_ELSE stmt_list HKL_T_END

for_stmt:
  HKL_T_FOR HKL_T_LPAREN HKL_T_RPAREN stmt_list HKL_T_END

while_stmt:
  HKL_T_WHILE HKL_T_LPAREN expression HKL_T_RPAREN stmt_list HKL_T_END

return_stmt:
  HKL_T_RETURN expression

break_stmt:
  HKL_T_BREAK

continue_stmt:
  HKL_T_CONTINUE

assert_stmt:
  HKL_T_ASSERT expression

include_stmt:
  HKL_T_INCLUDE expression

class_stmt:
  qualifier_list HKL_T_CLASS variable class_content_list HKL_T_END

class_content_list:
  class_content_list class_content
  | empty

class_content:
  class_stmt
  | function_stmt
  | init_assign

function_stmt:
  qualifier_list HKL_T_FUNCTION variable HKL_T_LPAREN id_list HKL_T_RPAREN stmt_list HKL_T_END

assign_stmt:
  init_assign
  | qualifier_list variable HKL_T_PLUS_ASSIGN expression
  | qualifier_list variable HKL_T_MINUS_ASSIGN expression
  | qualifier_list variable HKL_T_ASTERISK_ASSIGN expression
  | qualifier_list variable HKL_T_DIVIDE_ASSIGN expression
  | qualifier_list variable HKL_T_MOD_ASSIGN expression
  | qualifier_list variable HKL_T_BITWISE_AND_ASSIGN expression
  | qualifier_list variable HKL_T_BITWISE_OR_ASSIGN expression
  | qualifier_list variable HKL_T_BITWISE_XOR_ASSIGN expression
  | qualifier_list variable HKL_T_BITWISE_NOT_ASSIGN expression

init_assign:
  qualifier_list variable optional_init

qualifier_list:
  qualifier qualifier_list
  | empty

qualifier:
  HKL_T_UNIQUE
  | HKL_T_CONSTANT
  | HKL_T_PROTOTYPE
  | HKL_T_PROTECTED
  | HKL_T_LOCAL
  | HKL_T_GLOBAL

optional_init:
  HKL_T_ASSIGN expression
  | empty

expression:
  HKL_T_LPAREN expression HKL_T_RPAREN
  | primary_expression
  | expression HKL_T_OR expression
  | expression HKL_T_AND expression
  | expression HKL_T_LESS_EQUAL expression
  | expression HKL_T_GREATER_EQUAL expression
  | expression HKL_T_LESS expression
  | expression HKL_T_GREATER expression
  | expression HKL_T_EQUAL expression
  | expression HKL_T_NOT_EQUAL expression
  | expression HKL_T_PLUS expression
  | expression HKL_T_MINUS expression
  | expression HKL_T_ASTERISK expression
  | expression HKL_T_DIVIDE expression
  | expression HKL_T_MOD expression
  | expression HKL_T_BITWISE_AND expression
  | expression HKL_T_BITWISE_OR expression
  | expression HKL_T_BITWISE_XOR expression
  | HKL_T_NOT expression %prec UNARY_OPS
  | HKL_T_BITWISE_NOT expression %prec UNARY_OPS
  | HKL_T_MINUS expression %prec UNARY_OPS
  | HKL_T_INCREMENT expression %prec UNARY_OPS
  | HKL_T_DECREMENT expression %prec UNARY_OPS

primary_expression:
  HKL_T_INT_CONSTANT
  | HKL_T_REAL_CONSTANT
  | HKL_T_STRING_CONSTANT
  | HKL_T_GETS
  | HKL_T_TRUE
  | HKL_T_FALSE
  | HKL_T_NIL
  | HKL_T_NULL
  | variable
  | hash
  | array
  | inline_function
  | inline_class

variable:
  object_list

object_list:
  object_list HKL_T_DOT object
  | object

object:
  HKL_T_ID indicies_list

indicies_list:
  indicies_list index
  | empty

index:
  HKL_T_LBRACKET expression HKL_T_RBRACKET
  | HKL_T_LBRACKET expression HKL_T_RANGE HKL_T_RBRACKET

hash:
  HKL_T_LBRACE expression_list HKL_T_RBRACKET

array:
  HKL_T_LBRACKET expression_list HKL_T_RBRACKET

expression_list:
  expression expression_more
  | empty

expression_more:
  HKL_T_COMMA expression_list
  | empty 

inline_function:
  HKL_T_FUNCTION HKL_T_LPAREN id_list HKL_T_RPAREN stmt_list HKL_T_END

inline_class:
  HKL_T_CLASS class_content HKL_T_END

id_list:
  HKL_T_ID id_more
  | empty

id_more:
  HKL_T_COMMA id_list
  | empty

empty: // Do nothing yo

