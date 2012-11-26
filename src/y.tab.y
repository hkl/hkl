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
  extern HklList* closure_stack;
  extern HklList* pair_stack;
  extern HklList* id_stack;

  void reduce_stat(HklrStatement* stmt)
  {
    if (HKLR.scope_level == 0)
    {
      hklr_statement_exec(stmt);
      hklr_statement_free(stmt);
    }
    else
    {
      hkl_list_push_back((HklList*) stmt_stack->tail->data, stmt);
    }
  }

%}

// Options
%start program
%expect 2
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
  HklPair*        pair;
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
%token HKL_T_COLLECT                       "collect"

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

%type <statement> nobr_stat br_stat br_call nobr_call
%type <expression> expr nobr_variable br_variable nobr_prefix
%type <list> args
%type <pair> pair

%token END 0                               "end of file"

// Precedence

%left HKL_T_OR
%left HKL_T_AND
%left HKL_T_BITWISE_OR
%left HKL_T_BITWISE_XOR
%left HKL_T_BITWISE_AND
%left HKL_T_EQUAL HKL_T_NOT_EQUAL
%left HKL_T_LESS HKL_T_GREATER HKL_T_LESS_EQUAL HKL_T_GREATER_EQUAL
%left HKL_T_PLUS HKL_T_MINUS
%left HKL_T_DIVIDE HKL_T_ASTERISK HKL_T_MOD

%nonassoc UNARY_OPS

// HKL Grammar
%%

program:
  stat_list
  ;

stat_list:
  stat_list1 | stat_list2 | stat_list3 | stat_list4 | empty
  ;

stat_list1:
  stat1
  | stat_list1 stat1 | stat_list2 stat1 | stat_list3 stat1 | stat_list4 stat1
  ;

stat_list2:
  stat2
  | stat_list3 stat2 | stat_list4 stat2
  ;

stat_list3:
  stat3
  | stat_list1 stat3 | stat_list2 stat3 | stat_list3 stat3 | stat_list4 stat3
  ;

stat_list4:
  stat4
  | stat_list3 stat4 | stat_list4 stat4
  ;

stat1:
  nobr_call { reduce_stat($1); };

stat2:
  br_call { reduce_stat($1); };

stat3:
  nobr_stat { reduce_stat($1); };

stat4:
  br_stat { reduce_stat($1); };

nobr_stat:
  nobr_variable
  {
    HklList* list = hkl_list_pop_back(var_stack);
    //printf("var stack size %zu\n", list->size);
    $1->arg[1].list = list;
    $<expression>$ = $1;

    hkl_list_push_back(var_stack, hkl_list_new());
  }
  HKL_T_ASSIGN expr
  {
    $$ = hklr_statement_new(HKL_STMT_ASSIGN, $<expression>2, $4);
  }
  | HKL_T_PUTS expr
  {
    $$ = hklr_statement_new(HKL_STMT_PUTS, $2);
  }
  | HKL_T_HKLR
  {
    $$ = hklr_statement_new(HKL_STMT_HKLR);
  }
  | HKL_T_COLLECT
  {
    $$ = hklr_statement_new(HKL_STMT_COLLECT);
  }
  | HKL_T_WHILE
  {
    hkl_list_push_back(stmt_stack, hkl_list_new());
    HKLR.scope_level++;
  } 
  expr stat_list HKL_T_END
  {
    HKLR.scope_level--;

    $$ = hklr_statement_new(HKL_STMT_WHILE, $3, (HklList*) hkl_list_pop_back(stmt_stack));
  }
  | HKL_T_IF
  {
    hkl_list_push_back(stmt_stack, hkl_list_new());
    HKLR.scope_level++;
  }
  expr stat_list HKL_T_END
  {
    HKLR.scope_level--;

    $$ = hklr_statement_new(HKL_STMT_IF, $3, (HklList*) hkl_list_pop_back(stmt_stack));
  }
  | HKL_T_FUNCTION func_name_list HKL_T_LPAREN id_list HKL_T_RPAREN stat_list HKL_T_END
  | HKL_T_BREAK
  {
    $$ = hklr_statement_new(HKL_STMT_BREAK);
  }
  ;

br_stat:
  br_variable HKL_T_ASSIGN expr

func_name_list:
  HKL_T_ID
  | func_name_list HKL_T_DOT HKL_T_ID
  ;

expr:
  HKL_T_NIL
  {
    $$ = hklr_expression_new(HKL_EXPR_NIL);
  }
  | HKL_T_TRUE
  {
    $$ = hklr_expression_new(HKL_EXPR_INT, 1);
  }
  | HKL_T_FALSE
  {
    $$ = hklr_expression_new(HKL_EXPR_INT, 0);
  }
  | HKL_T_MAYBE
  {
    $$ = hklr_expression_new(HKL_EXPR_MAYBE);
  }
  | HKL_T_INT_CONSTANT
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
  | HKL_T_INT
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_INT);
  }
  | HKL_T_REAL
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_REAL);
  }
  | HKL_T_STRING
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_STRING);
  }
  | HKL_T_HASH
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_HASH);
  }
  | HKL_T_ARRAY
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_ARRAY);
  }
  | HKL_T_FUNC
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_FUNCTION);
  }
  | HKL_T_TYPE
  {
    $$ = hklr_expression_new(HKL_EXPR_TYPE, HKL_TYPE_TYPE);
  }
  | HKL_T_FUNCTION
  {
    hkl_list_push_back(stmt_stack, hkl_list_new());
    hkl_list_push_back(id_stack, hkl_list_new());
    hkl_list_push_back(closure_stack, hkl_tree_new());
    HKLR.scope_level++;
  }
  HKL_T_LPAREN id_list HKL_T_RPAREN stat_list HKL_T_END
  {
    HKLR.scope_level--;

    HklList* args = hkl_list_pop_back(id_stack);

    $$ = hklr_expression_new(HKL_EXPR_FUNCTION, args, hkl_list_pop_back(closure_stack), hkl_list_pop_back(stmt_stack));
  }
  | HKL_T_LBRACKET
  {
    hkl_list_push_back(array_stack, hkl_list_new());
  }
  expr_list HKL_T_RBRACKET
  {
    $$ = hklr_expression_new(HKL_EXPR_ARRAY, hkl_list_pop_back(array_stack));
  }
  | HKL_T_LBRACKET HKL_T_RBRACKET
  {
    $$ = hklr_expression_new(HKL_EXPR_ARRAY, hkl_list_new());
  }
  | HKL_T_LBRACE
  {
    hkl_list_push_back(pair_stack, hkl_list_new());
  }
  pair_list HKL_T_RBRACE
  {
    $$ = hklr_expression_new(HKL_EXPR_HASH, hkl_list_pop_back(pair_stack));
  }
  | HKL_T_LBRACE HKL_T_RBRACE
  {
    $$ = hklr_expression_new(HKL_EXPR_HASH, hkl_list_new());
  }
  | nobr_prefix
  | HKL_T_LPAREN expr HKL_T_RPAREN
  {
    $$ = $2;
  }
  | HKL_T_GETS
  {
    $$ = hklr_expression_new(HKL_EXPR_GETS);
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
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_MOD, $3);
  }
  | expr HKL_T_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_EQUAL, $3);
  }
  | expr HKL_T_NOT_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_NOT_EQUAL, $3);
  }
  | expr HKL_T_LESS expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_LESS, $3);
  }
  | expr HKL_T_GREATER expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_GREATER, $3);
  }
  | expr HKL_T_LESS_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_LESS_EQUAL, $3);
  }
  | expr HKL_T_GREATER_EQUAL expr
  {
    $$ = hklr_expression_new(HKL_EXPR_BINARY, $1, HKL_OP_GREATER_EQUAL, $3);
  }
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
  ;

expr_list:
  expr
  {
    hkl_list_push_back((HklList*) array_stack->tail->data, $1);
  }
  | expr_list HKL_T_COMMA expr
  {
    hkl_list_push_back((HklList*) array_stack->tail->data, $3);
  }
  ;

pair_list:
  pair
  {
    hkl_list_push_back((HklList*) pair_stack->tail->data, $1);
  }
  | pair_list HKL_T_COMMA pair
  {
    hkl_list_push_back((HklList*) pair_stack->tail->data, $3);
  }
  ;

pair:
  HKL_T_STRING_CONSTANT HKL_T_COLON expr
  {
    $$ = hkl_pair_new_from_data($1, $3);
  }
  | HKL_T_ID HKL_T_COLON expr
  {
    $$ = hkl_pair_new_from_data($1, $3);
  }
  ;

nobr_prefix:
  nobr_variable
  {
    HklList* list = hkl_list_pop_back(var_stack);
    //printf("var stack size %zu\n", list->size);
    $1->arg[1].list = list;
    $$ = $1;

    hkl_list_push_back(var_stack, hkl_list_new());
  }
  | nobr_call
  ;

nobr_variable:
  HKL_T_ID
  {
    $$ = hklr_expression_new(HKL_EXPR_VAR, $1, NULL);

    // If there we are in a function definition
    if (closure_stack->head != NULL)
    {
      // Add the id as a closure
      if (hkl_tree_search((HklTree*) closure_stack->tail->data, $1) == NULL)
      {
        hkl_tree_insert((HklTree*) closure_stack->tail->data, $1, NULL);
      }
    }

    // Seed the next variable
    //hkl_list_push_back(var_stack, hkl_list_new());
  }
  //| nobr_prefix HKL_T_LBRACKET expr HKL_T_RBRACKET // index
  | nobr_prefix HKL_T_LBRACKET HKL_T_STRING_CONSTANT HKL_T_RBRACKET
  {
    hkl_list_push_back((HklList*) var_stack->tail->data, hkl_variable_new(HKL_VAR_ID, $3));
  }
  | nobr_prefix HKL_T_DOT HKL_T_ID
  {
    //printf("adding\n");
    hkl_list_push_back((HklList*) var_stack->tail->data, hkl_variable_new(HKL_VAR_ID, $3));
  }
  ;

br_variable:
  HKL_T_LPAREN HKL_T_ID HKL_T_RPAREN
  {
    $$ = hklr_expression_new(HKL_EXPR_VAR, $2, hkl_list_pop_back(var_stack));

    // If there we are in a function definition
    if (closure_stack->head != NULL)
    {
      // Add the id as a closure
      if (hkl_tree_search((HklTree*) closure_stack->tail->data, $2) == NULL)
      {
        hkl_tree_insert((HklTree*) closure_stack->tail->data, $2, NULL);
      }
    }

    // Seed the next variable
    hkl_list_push_back(var_stack, hkl_list_new());
  }
  | HKL_T_LPAREN expr HKL_T_RPAREN HKL_T_LBRACKET expr HKL_T_RBRACKET // call then index
  | HKL_T_LPAREN expr HKL_T_RPAREN HKL_T_DOT HKL_T_ID // call then id
  ;

nobr_call:
  nobr_prefix args
  {
    $$ = hklr_statement_new(HKL_STMT_CALL, $1, $2);
  }
  ;

br_call:
  HKL_T_LPAREN expr HKL_T_RPAREN args
  {
    $$ = hklr_statement_new(HKL_STMT_CALL, $2, $4);
  }
  ;

args:
  HKL_T_LPAREN { hkl_list_push_back(array_stack, hkl_list_new()); } HKL_T_RPAREN
  {
    $$ = hkl_list_pop_back(array_stack);
  }
  | HKL_T_LPAREN { hkl_list_push_back(array_stack, hkl_list_new()); } expr_list HKL_T_RPAREN
  {
    $$ = hkl_list_pop_back(array_stack);
  }
  ;

id_list:
  HKL_T_ID
  {
    hkl_list_push_back((HklList*) id_stack->tail->data, $1);
  }
  | id_list HKL_T_COMMA HKL_T_ID
  {
    hkl_list_push_back((HklList*) id_stack->tail->data, $3);
  }
  | empty
  ;

empty: ;
