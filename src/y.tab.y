%{

  #include <stdio.h>

  #include "hkl_string.h"

  // These will be defined at link time
  extern int yylex();
  extern int yyerror(char const*);
%}

// Verbose Errors
%error-verbose

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

%token HKL_T_IF                            "if"
%token HKL_T_ELSE                          "else"
%token HKL_T_FOR                           "for"
%token HKL_T_WHILE                         "while"   
%token HKL_T_END                           "end"
%token HKL_T_RETURN                        "return"
%token HKL_T_BREAK                         "break"
%token HKL_T_CONTINUE                      "continue"
%token HKL_T_ASSERT                        "assert"
%token HKL_T_INCLUDE                       "include"

%token HKL_T_CLASS                         "class"
%token HKL_T_FUNCTION                      "function"
%token HKL_T_NIL                           "nil"
%token HKL_T_NULL                          "null"
%token HKL_T_INT                           "integer"
%token HKL_T_REAL                          "real"
%token HKL_T_STRING                        "string"
%token HKL_T_ARRAY                         "array"
%token HKL_T_HASH                          "hash"
%token HKL_T_INSTANCE                      "instance"
%token HKL_T_SWITCH                        "switch"
%token HKL_T_CASE                          "case"

%token HKL_T_SELF                          "self"

%token HKL_T_UNIQUE                        "unique"
%token HKL_T_CONSTANT                      "constant"
%token HKL_T_PROTOTYPE                     "prototype"
%token HKL_T_PROTECTED                     "protected"
%token HKL_T_LOCAL                         "local"
%token HKL_T_GLOBAL                        "global"

%token HKL_T_TRUE                          "true"
%token HKL_T_FALSE                         "false"

%token HKL_T_PUTS                          "puts"
%token HKL_T_GETS                          "gets"

%token HKL_T_LPAREN                        "("
%token HKL_T_RPAREN                        ")"
%token HKL_T_LBRACE                        "{"
%token HKL_T_RBRACE                        "}"
%token HKL_T_LBRACKET                      "["
%token HKL_T_RBRACKET                      "]"
%token HKL_T_COLON                         ":"
%token HKL_T_COMMA                         ","
%token HKL_T_DOT                           "."

%token HKL_T_INCREMENT                     "++"
%token HKL_T_DECREMENT                     "--"
%token HKL_T_OR                            "||"
%token HKL_T_AND                           "&&"
%token HKL_T_LESS_EQUAL                    "<="
%token HKL_T_GREATER_EQUAL                 ">="
%token HKL_T_LESS                          "<"
%token HKL_T_GREATER                       ">"
%token HKL_T_EQUAL                         "=="
%token HKL_T_NOT_EQUAL                     "!="
%token HKL_T_PLUS_ASSIGN                   "+="
%token HKL_T_MINUS_ASSIGN                  "-="
%token HKL_T_ASTERISK_ASSIGN               "*="
%token HKL_T_DIVIDE_ASSIGN                 "/="
%token HKL_T_MOD_ASSIGN                    "%="
%token HKL_T_BITWISE_AND_ASSIGN            "&="
%token HKL_T_BITWISE_OR_ASSIGN             "|="
%token HKL_T_BITWISE_XOR_ASSIGN            "^="
%token HKL_T_BITWISE_NOT_ASSIGN            "~="

%token HKL_T_ASSIGN                        "="
%token HKL_T_NOT                           "!"
%token HKL_T_PLUS                          "+"
%token HKL_T_MINUS                         "-"
%token HKL_T_ASTERISK                      "*"
%token HKL_T_DIVIDE                        "/"
%token HKL_T_MOD                           "%"
%token HKL_T_BITWISE_AND                   "&"
%token HKL_T_BITWISE_OR                    "|"
%token HKL_T_BITWISE_XOR                   "^"
%token HKL_T_BITWISE_NOT                   "~"
%token HKL_T_TYPE_OF                       "type_of"
%token HKL_T_INSTANCE_OF                   "instance_of"
%token HKL_T_RANGE                         ".."

%token <string>  HKL_T_ID                  "identifier"
%token <integer> HKL_T_INT_CONSTANT        "integer constant"
%token <real>    HKL_T_REAL_CONSTANT       "real constant"
%token <string>  HKL_T_STRING_CONSTANT     "string literal"

%token END 0                               "end of file"

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
  | switch_stmt

puts_stmt:
  HKL_T_PUTS HKL_T_STRING_CONSTANT
  {
    printf("%s", hkl_string_get_utf8($2));
  }

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
  | no_call_init_assign

function_stmt:
  qualifier_list HKL_T_FUNCTION nocall_variable HKL_T_LPAREN id_list HKL_T_RPAREN stmt_list HKL_T_END

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

switch_stmt:
  HKL_T_SWITCH HKL_T_LPAREN expression HKL_T_RPAREN case_list HKL_T_END

case_list:
  case case_list
  | empty

case:
  HKL_T_CASE HKL_T_INT_CONSTANT HKL_T_COLON stmt_list

init_assign:
  qualifier_list variable optional_init

no_call_init_assign:
  qualifier_list nocall_variable optional_init

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
  | expression HKL_T_TYPE_OF type
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
  | HKL_T_SELF
  | variable
  | hash
  | array
  | inline_function
  | inline_class

type:
  HKL_T_INT
  | HKL_T_REAL
  | HKL_T_STRING
  | HKL_T_HASH
  | HKL_T_ARRAY
  | HKL_T_FUNCTION
  | HKL_T_CLASS
  | HKL_T_INSTANCE

variable:
  object_list

nocall_variable:
  nocall_object_list

object_list:
  object_list HKL_T_DOT object
  | object

nocall_object_list:
  nocall_object_list HKL_T_DOT nocall_object
  | nocall_object

object:
  HKL_T_ID action_list 

nocall_object:
  HKL_T_ID nocall_action_list

action_list:
  action_list action
  | empty

nocall_action_list:
  nocall_action_list index
  | empty

action:
  index
  | call

index:
  HKL_T_LBRACKET expression HKL_T_RBRACKET
  | HKL_T_LBRACKET expression HKL_T_RANGE expression HKL_T_RBRACKET

call:
  HKL_T_LPAREN expression_list HKL_T_RPAREN

hash:
  HKL_T_LBRACE key_val_list HKL_T_RBRACE

key_val_list:
  key_val key_val_more
  | empty

key_val_more:
  HKL_T_COMMA key_val_list
  | empty

key_val:
  HKL_T_ID optional_value
  | HKL_T_STRING_CONSTANT optional_value
  | HKL_T_INT_CONSTANT optional_value
  | HKL_T_REAL_CONSTANT optional_value

optional_value:
  HKL_T_COLON expression
  | empty

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
