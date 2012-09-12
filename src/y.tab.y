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

%token HKL_T_NEWLINE

%token HKL_T_IF
%token HKL_T_ELSE
%token HKL_T_FOR
%token HKL_T_WHILE
%token HKL_T_END
%token HKL_T_RETURN
%token HKL_T_BREAK
%token HKL_T_ASSERT
%token HKL_T_INCLUDE

%token HKL_T_CLASS
%token HKL_T_FUNCTION
%token HKL_T_NIL
%token HKL_T_NULL

%token HKL_T_SELF

%token HKL_T_UNIQUE
%token HKL_T_CONST
%token HKL_T_PROTOTYPE
%token HKL_T_PROTECTED

%token HKL_T_TRUE
%token HKL_T_FALSE

%token HKL_T_TYPE_INT
%token HKL_T_TYPE_REAL
%token HKL_T_TYPE_STRING
%token HKL_T_TYPE_ARRAY
%token HKL_T_TYPE_HASH

%token HKL_T_PUTS
%token HKL_T_GETS

%token HKL_T_LPAREN
%token HKL_T_RPAREN
%token HKL_T_LBRACE
%token HKL_T_RBRACE
%token HKL_T_LBRACKET
%token HKL_T_RBRACKET
%token HKL_T_COLON
%token HKL_T_COMMA
%token HKL_T_DOT

// Operators
%token HKL_T_NOT
%token HKL_T_BITWISE_NOT
%token HKL_T_INCREMENT
%token HKL_T_DECREMENT

%token HKL_T_OR
%token HKL_T_AND
%token HKL_T_LESS_EQUAL
%token HKL_T_GREATER_EQUAL
%token HKL_T_LESS
%token HKL_T_GREATER
%token HKL_T_EQUAL
%token HKL_T_NOT_EQUAL

%token HKL_T_PLUS
%token HKL_T_MINUS
%token HKL_T_ASTERISK
%token HKL_T_DIVIDE
%token HKL_T_MOD
%token HKL_T_BITWISE_AND
%token HKL_T_BITWISE_OR
%token HKL_T_BITWISE_XOR
%token HKL_T_TYPE_OF
%token HKL_T_INSTANCE_OF
%token HKL_T_RANGE

%token HKL_T_ASSIGN
%token HKL_T_PLUS_ASSIGN
%token HKL_T_MINUS_ASSIGN
%token HKL_T_ASTERISK_ASSIGN
%token HKL_T_DIVIDE_ASSIGN
%token HKL_T_MOD_ASSIGN
%token HKL_T_BITWISE_AND_ASSIGN
%token HKL_T_BITWISE_OR_ASSIGN
%token HKL_T_BITWISE_XOR_ASSIGN

%token <string>  HKL_T_ID
%token <integer> HKL_T_INTEGER_CONSTANT
%token <real>    HKL_T_REAL_CONSTANT
%token <string>  HKL_T_STRING_CONSTANT

// Precedence
%nonassoc HKL_T_ELSE

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

%nonassoc UNARY_OPS

// HKL Grammar
%%

program:
  stmt_list

stmt_list:
  stmt_list stmt HKL_T_NEWLINE
  | empty

stmt:
  puts_stmt
  | if_stmt
  | for_stmt
  | while_stmt
  | return_stmt
  | break_stmt
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
  HKL_T_FOR HKL_T_LPAREN expression HKL_T_COMMA expression HKL_T_RPAREN stmt_list HKL_T_END

while_stmt:
  HKL_T_WHILE HKL_T_LPAREN expression HKL_T_RPAREN stmt_list HKL_T_END

return_stmt:
  HKL_T_RETURN expression

break_stmt:
  HKL_T_BREAK

assert_stmt:
  HKL_T_ASSERT expression

include_stmt:
  HKL_T_INCLUDE expression

class_stmt:
  object_modifier_list HKL_T_CLASS HKL_T_ID stmt_list HKL_T_END

function_stmt:
  object_modifier_list HKL_T_FUNCTION HKL_T_ID HKL_T_LPAREN parameter_list HKL_T_RPAREN stmt_list HKL_T_END

assign_stmt:
  object HKL_T_ASSIGN expression
  | object HKL_T_PLUS_ASSIGN expression
  | object HKL_T_MINUS_ASSIGN expression
  | object HKL_T_ASTERISK_ASSIGN expression
  | object HKL_T_DIVIDE_ASSIGN expression
  | object HKL_T_MOD_ASSIGN expression
  | object HKL_T_BITWISE_AND_ASSIGN expression
  | object HKL_T_BITWISE_OR_ASSIGN expression
  | object HKL_T_BITWISE_XOR_ASSIGN expression

expression:
  primary_expression
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
  | object HKL_T_TYPE_OF object_type
  | HKL_T_NOT expression %prec UNARY_OPS
  | HKL_T_BITWISE_NOT expression %prec UNARY_OPS
  | HKL_T_INCREMENT expression %prec UNARY_OPS
  | HKL_T_DECREMENT expression %prec UNARY_OPS

primary_expression:
  object
  | HKL_T_INTEGER_CONSTANT
  | HKL_T_REAL_CONSTANT
  | HKL_T_STRING_CONSTANT
  | HKL_T_GETS
  | HKL_T_TRUE
  | HKL_T_FALSE
  | HKL_T_NIL
  | HKL_T_NULL
  | HKL_T_LBRACKET array_list HKL_T_RBRACKET /* array */
  | HKL_T_LBRACE hash_list HKL_T_RBRACE /* hash */

array_list:
  array_list HKL_T_COMMA expression
  | expression
  | empty

hash_list:
  hash_list HKL_T_COMMA key_value_pair
  | key_value_pair
  | empty

key_value_pair:
  expression HKL_T_COLON expression

object:
  object_modifier_list optional_self object_id

object_id:
  HKL_T_ID /* integer, real or string */
  | HKL_T_ID HKL_T_LBRACKET expression HKL_T_RBRACKET/* array */
  | HKL_T_ID range /* array range*/
  | HKL_T_ID HKL_T_LBRACE expression HKL_T_RBRACE /* hash */

optional_self:
  HKL_T_SELF HKL_T_DOT
  | empty

object_type:
  HKL_T_TYPE_INT
  | HKL_T_TYPE_REAL
  | HKL_T_TYPE_STRING
  | HKL_T_TYPE_ARRAY
  | HKL_T_TYPE_HASH

object_modifier_list:
  object_modifier_list object_modifier
  | empty

object_modifier:
  HKL_T_UNIQUE
  | HKL_T_CONST
  | HKL_T_PROTOTYPE
  | HKL_T_PROTECTED

range:
  HKL_T_LBRACKET expression HKL_T_RANGE expression HKL_T_RBRACKET

parameter_list:
  HKL_T_ID
  | parameter_list HKL_T_COMMA HKL_T_ID
  | empty

empty: // Do nothing yo

