%{

  #include <stdio.h>

  #include "hkl_string.h"
  #include "hklr.h"
  #include "hklr_expression.h"
  #include "hklr_statement.h"
  #include "hkl_variable.h"

  #define YYDEBUG 0

  // These will be defined at link time
  extern int yylex();
  extern int yyerror(char const*);
  extern uint32_t qualifier_builder;

  extern HklList* stmt_stack;
  extern HklList* var_stack;
  extern HklList* array_stack;
%}

// Verbose Errors
%error-verbose

// YYSUNION Definition
%union
{
  int             integer;
  double          real;
  HklFlag         flag;
  HklString*      string;
  HklrStatement*  statement;
  HklrExpression* expression;
  HklList*        list;
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

%token HKL_T_CLASS                         "class"
%token HKL_T_FUNCTION                      "function"
%token HKL_T_NIL                           "nil"
%token HKL_T_INT                           "integer"
%token HKL_T_REAL                          "real"
%token HKL_T_STRING                        "string"
%token HKL_T_ARRAY                         "array"
%token HKL_T_HASH                          "hash"
%token HKL_T_TYPE                          "type"
%token HKL_T_INSTANCE                      "instance"
%token HKL_T_FUNC                          "func"
%token HKL_T_SWITCH                        "switch"
%token HKL_T_CASE                          "case"
%token HKL_T_DEFAULT                       "default"

%token HKL_T_SELF                          "self"

%token HKL_T_UNIQUE                        "unique"
%token HKL_T_CONSTANT                      "constant"
%token HKL_T_PROTOTYPE                     "prototype"
%token HKL_T_PROTECTED                     "protected"
%token HKL_T_LOCAL                         "local"
%token HKL_T_GLOBAL                        "global"

%token HKL_T_TRUE                          "true"
%token HKL_T_FALSE                         "false"
%token HKL_T_MAYBE                         "maybe"

%token HKL_T_PUTS                          "puts"
%token HKL_T_GETS                          "gets"
%token HKL_T_HKLR                          "hklr"
%token HKL_T_TYPEOF                        "typeof"

%token HKL_T_LPAREN                        "("
%token HKL_T_RPAREN                        ")"
%token HKL_T_LBRACE                        "{"
%token HKL_T_RBRACE                        "}"
%token HKL_T_LBRACKET                      "["
%token HKL_T_RBRACKET                      "]"
%token HKL_T_COLON                         ":"
%token HKL_T_SEMICOLON                     ";"
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
%token HKL_T_RANGE                         ".."
%token HKL_T_CRUNCH                        "#"

%token <string>  HKL_T_ID                  "identifier"
%token <integer> HKL_T_INT_CONSTANT        "integer constant"
%token <real>    HKL_T_REAL_CONSTANT       "real constant"
%token <string>  HKL_T_STRING_CONSTANT     "string literal"

%type <statement> stmt
%type <statement> stmt_explicit
%type <statement> puts_stmt
%type <statement> hklr_stmt
%type <statement> assign_stmt
%type <statement> if_stmt
%type <statement> while_stmt

%type <list> array

%type <expression> expr
%type <expression> optional_init
%type <expression> primary_expr
%type <expression> variable
%type <expression> type

%type <flag> qualifier_list

%token END 0                               "end of file"

// Precedence

%right HKL_T_ASSIGN
%left HKL_T_OR
%left HKL_T_AND
%left HKL_T_BITWISE_OR
%left HKL_T_BITWISE_XOR
%left HKL_T_BITWISE_AND
%left HKL_T_EQUAL HKL_T_NOT_EQUAL
%left HKL_T_LESS HKL_T_GREATER HKL_T_LESS_EQUAL HKL_T_GREATER_EQUAL
%left HKL_T_PLUS HKL_T_MINUS
%left HKL_T_DIVIDE HKL_T_ASTERISK HKL_T_MOD

%left HKL_T_DOT          // member    
%left HKL_T_LBRACKET     // index
%left HKL_T_LPAREN       // function call

%nonassoc UNARY_OPS

// HKL Grammar
%%

program:
  stmt_list

stmt_list:
  stmt_list stmt_explicit
  {
    // if we made a statement, and are in scope 0,
    // then execute the statement

    if (HKLR.scope_level == 0)
    {
      hklr_statement_exec($2);

      // clean up the statement as we dont need it anymore
      hklr_statement_free($2);
    }
    else
    {
      // add the statement to the current stmt_list
      hkl_list_push_back((HklList*) stmt_stack->tail->data, $2);
    }
  }
  | empty

stmt_explicit:
  stmt HKL_T_SEMICOLON
  | stmt

stmt:
  puts_stmt
  | if_stmt
  | for_stmt
  | while_stmt
  | return_stmt
  | break_stmt
  | continue_stmt
  | assert_stmt
  | class_stmt
  | function_stmt
  | assign_stmt
  | hklr_stmt
  | call_stmt
  //| switch_stmt

puts_stmt:
  HKL_T_PUTS expr
  {
    $$ = hklr_statement_new(HKL_STMT_PUTS, $2);
  }

if_stmt:
  HKL_T_IF { hkl_list_push_back(stmt_stack, hkl_list_new()); HKLR.scope_level++; } 
  expr stmt_list HKL_T_END
  {
    HKLR.scope_level--;

    $$ = hklr_statement_new(HKL_STMT_IF, $3,
          (HklList*) hkl_list_pop_back(stmt_stack));
  }

/*  | HKL_T_IF { hkl_list_push_back(stmt_stack, hkl_list_new()); HKLR.scope_level++; } 
  HKL_T_LPAREN expr HKL_T_RPAREN stmt_list HKL_T_ELSE stmt_list HKL_T_END
  {
    HKLR.scope_level--;

    $$ = hklr_statement_new(HKL_STMT_IF, $4,
          (HklList*) hkl_list_pop_back(stmt_stack));
  }
*/

for_stmt:
  HKL_T_FOR HKL_T_LPAREN HKL_T_RPAREN stmt_list HKL_T_END

while_stmt:
  HKL_T_WHILE { hkl_list_push_back(stmt_stack, hkl_list_new()); HKLR.scope_level++; }
  expr stmt_list HKL_T_END
  {
    HKLR.scope_level--;

    $$ = hklr_statement_new(HKL_STMT_WHILE, $3,
          (HklList*) hkl_list_pop_back(stmt_stack));
  }

return_stmt:
  HKL_T_RETURN expr

break_stmt:
  HKL_T_BREAK

continue_stmt:
  HKL_T_CONTINUE

assert_stmt:
  HKL_T_ASSERT expr

class_stmt:
  qualifier_list HKL_T_CLASS variable class_content_list HKL_T_END

class_content_list:
  class_content_list class_content
  | empty

class_content:
  class_stmt
  | function_stmt
  | qualifier_list variable optional_init

function_stmt:
  qualifier_list HKL_T_FUNCTION variable HKL_T_LPAREN id_list HKL_T_RPAREN stmt_list HKL_T_END
  | HKL_T_FUNCTION variable HKL_T_LPAREN id_list HKL_T_RPAREN stmt_list HKL_T_END

assign_stmt:
  qualifier_list variable optional_init
  | variable HKL_T_ASSIGN expr
  {
    $$ = hklr_statement_new(HKL_STMT_ASSIGN, $1, $3);
  }
  | variable HKL_T_PLUS_ASSIGN expr
  | variable HKL_T_MINUS_ASSIGN expr
  | variable HKL_T_ASTERISK_ASSIGN expr
  | variable HKL_T_DIVIDE_ASSIGN expr
  | variable HKL_T_MOD_ASSIGN expr
  | variable HKL_T_BITWISE_AND_ASSIGN expr
  | variable HKL_T_BITWISE_OR_ASSIGN expr
  | variable HKL_T_BITWISE_XOR_ASSIGN expr
  | variable HKL_T_BITWISE_NOT_ASSIGN expr

call_stmt:
  variable HKL_T_LPAREN expr_list HKL_T_RPAREN

hklr_stmt:
  HKL_T_HKLR
  {
    $$ = hklr_statement_new(HKL_STMT_HKLR);
  }

// The statemens in a switch should be stored in a normal
// statement list, but use a hklhash to map to the "case nodes"
// in the switch list. This gives us O(logn) jumps and normal waterfall
// execution 
switch_stmt:
  HKL_T_SWITCH HKL_T_LPAREN expr HKL_T_RPAREN case_list HKL_T_END

case_list:
  case_list case
  | default_case

case:
  HKL_T_CASE expr HKL_T_COLON stmt_list

default_case:
  HKL_T_DEFAULT HKL_T_COLON stmt_list
  | empty 

qualifier_list:
  qualifier_list qualifier {

    uint32_t temp = qualifier_builder;
    qualifier_builder = 0;
    $$ = temp;
  }
  | qualifier {

    uint32_t temp = qualifier_builder;
    qualifier_builder = 0;
    $$ = temp;
  }

qualifier:
  HKL_T_UNIQUE
  {
    qualifier_builder |= HKL_FLAG_UNIQUE;
  }
  | HKL_T_CONSTANT
  {
    qualifier_builder |= HKL_FLAG_CONST;
  }
  | HKL_T_LOCAL
  {
    qualifier_builder |= HKL_FLAG_LOCAL;
  }
  | HKL_T_GLOBAL
  {
    qualifier_builder |= HKL_FLAG_GLOBAL;
  }

optional_init:
  HKL_T_ASSIGN expr
  {
    $$ = $2;
  }
  | empty
  {
    $$ = hklr_expression_new(HKL_EXPR_NIL);
  }

expr:
  HKL_T_LPAREN expr HKL_T_RPAREN
  {
    $$ = $2;
  }
  | primary_expr
  | expr HKL_T_OR expr
  {

  }
  | expr HKL_T_AND expr
  | expr HKL_T_LESS_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_LESS_EQUAL, $3);
  }
  | expr HKL_T_GREATER_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_GREATER_EQUAL, $3);
  }
  | expr HKL_T_LESS expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_LESS, $3);
  }
  | expr HKL_T_GREATER expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_GREATER, $3);
  }
  | expr HKL_T_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_EQUAL, $3);
  }
  | expr HKL_T_NOT_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_NOT_EQUAL, $3);
  }
  | expr HKL_T_PLUS expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_PLUS, $3);  
  }
  | expr HKL_T_MINUS expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_MINUS, $3);  
  }
  | expr HKL_T_ASTERISK expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_ASTERISK, $3);
  }
  | expr HKL_T_DIVIDE expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_DIVIDE, $3);
  }
  | expr HKL_T_MOD expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_DIVIDE, $3);
  }
  | expr HKL_T_ASSIGN expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_ASSIGN, $3);
  }
  | expr HKL_T_BITWISE_AND expr
  | expr HKL_T_BITWISE_OR expr
  | expr HKL_T_BITWISE_XOR expr
  | HKL_T_NOT expr %prec UNARY_OPS
  | HKL_T_BITWISE_NOT expr %prec UNARY_OPS

  | HKL_T_MINUS expr %prec UNARY_OPS
  {
    $$ = hklr_expression_new(HKL_EXPR_UNARY, HKL_OP_UNARY_MINUS, $2);
  }

  | HKL_T_CRUNCH expr %prec UNARY_OPS
  {
    $$ = hklr_expression_new(HKL_EXPR_UNARY, HKL_OP_SIZE, $2);
  }
  | HKL_T_TYPEOF expr %prec UNARY_OPS
  {
    $$ = hklr_expression_new(HKL_EXPR_UNARY, HKL_OP_TYPEOF, $2);
  }
  | HKL_T_INCREMENT expr %prec UNARY_OPS
  | HKL_T_DECREMENT expr %prec UNARY_OPS

primary_expr:
  HKL_T_INT_CONSTANT
  {
    $$ = hklr_expression_new(HKL_EXPR_INT, $1);
  }
  | HKL_T_REAL_CONSTANT
  {
    $$ = hklr_expression_new(HKL_EXPR_REAL, $1);
  }
  | HKL_T_STRING_CONSTANT
  {
    $$ = hklr_expression_new(HKL_EXPR_STRING, $1);
  }
  | HKL_T_GETS
  {
    $$ = hklr_expression_new(HKL_EXPR_GETS);
  }
  | HKL_T_MAYBE
  {
    $$ = hklr_expression_new(HKL_EXPR_MAYBE);
  }
  | HKL_T_TRUE
  {
    $$ = hklr_expression_new(HKL_EXPR_INT, 1);
  }
  | HKL_T_FALSE
  {
    $$ = hklr_expression_new(HKL_EXPR_INT, 0);
  }
  | HKL_T_NIL
  {
    $$ = hklr_expression_new(HKL_EXPR_NIL);
  }
  | HKL_T_SELF
  | variable HKL_T_LPAREN expr_list HKL_T_RPAREN
  | variable
  | hash
  | type
  | array
  {
    $$ = hklr_expression_new(HKL_EXPR_ARRAY, $1);
  }
  | inline_function
  | inline_class

type:
  HKL_T_INT { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_INT); }
  | HKL_T_TYPE { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_TYPE); }
  | HKL_T_REAL { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_REAL); }
  | HKL_T_STRING { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_STRING); }
  | HKL_T_HASH { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_HASH); }
  | HKL_T_ARRAY { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_ARRAY); }
  | HKL_T_FUNC { $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_FUNCTION); }

variable:
  HKL_T_ID { hkl_list_push_back(var_stack, hkl_list_new()); } variable_more
  {
    $$ = hklr_expression_new(HKL_EXPR_VAR, $1, hkl_list_pop_back(var_stack));
  }

variable_more:
  variable_more HKL_T_DOT HKL_T_ID
  {
    hkl_list_push_back((HklList*) var_stack->tail->data, hkl_variable_new(HKL_VAR_ID, $3));
  }
  | variable_more HKL_T_LBRACKET expr HKL_T_RBRACKET
  {
    hkl_list_push_back((HklList*) var_stack->tail->data, hkl_variable_new(HKL_VAR_INDEX, $3));
  }
  | empty

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
  HKL_T_COLON expr
  | empty

array:
  HKL_T_LBRACKET { hkl_list_push_back(array_stack, hkl_list_new()); } expr_list HKL_T_RBRACKET
  {
    $$ = hkl_list_pop_back(array_stack);
  }

expr_list:
  expr expr_more
  {
    // add the statement to the current expr_list
    hkl_list_push_back((HklList*) array_stack->tail->data, $1);
  }
  | empty

expr_more:
  HKL_T_COMMA expr_list
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
