%{
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

// Hkl Token List
%token HKL_T_IF
%token HKL_T_ELSE
%token HKL_T_FOR
%token HKL_T_WHILE
%token HKL_T_END

%token HKL_T_CLASS
%token HKL_T_FUNCTION
%token HKL_T_NIL
%token HKL_T_NULL

%token <integer> HKL_T_INT
%token <real>    HKL_T_REAL
%token <string>  HKL_T_STRING

%token HKL_T_SELF

%token HKL_T_UNIQUE
%token HKL_T_CONST
%token HKL_T_PROTOTYPE
%token HKL_T_PROTECTED

%token HKL_T_PUTS
%token HKL_T_GETS

%token HKL_T_INCLUDE

%token HKL_T_LPAREN
%token HKL_T_RPAREN
%token HKL_T_LBRACE
%token HKL_T_RBRACE
%token HKL_T_LBRACKET
%token HKL_T_RBRACKET
%token HKL_T_COMMA
%token HKL_T_DOT

// HKL Grammar
%%

program:
  empty;

empty: ; // Do nothing yo

