%{

#include <stdio.h>      // for scan functions

#include "hklc.h"

extern int yyerror(const char* msg);
%}

%%

[ \t\r]                /* Ignore Whitespace */
"//".*[\n]             /* Ignore Comments */

[\n]+                  return HKL_T_NEWLINE;

"if"                   return HKL_T_IF;
"else"                 return HKL_T_ELSE;
"for"                  return HKL_T_FOR;
"while"                return HKL_T_WHILE;
"end"                  return HKL_T_END;
"return"               return HKL_T_RETURN;

"class"                return HKL_T_CLASS;
"function"             return HKL_T_FUNCTION;
"nil"                  return HKL_T_NIL;
"null"                 return HKL_T_NULL;

[0-9]+ /* Integers */ {

  sscanf(yytext, "%i", &yylval.integer);
  return HKL_T_INTEGER;
}

([0-9]+\.[0-9]+)|([0-9]+\.[0-9]*)|([0-9]*\.[0-9]+) /* Reals */ {

  sscanf(yytext, "%lg", &yylval.real);
  return HKL_T_REAL;
}

\"(\\.|[^\"])*\" {

  yylval.string = hkl_string_new_from_utf8_chunk(yytext + 1, yytext + yyleng - 1);
  return HKL_T_STRING;
}

"self"                 return HKL_T_SELF;

"unique"               return HKL_T_UNIQUE;
"constant"             return HKL_T_CONST;
"prototype"            return HKL_T_PROTOTYPE;
"protected"            return HKL_T_PROTECTED;

"puts"                 return HKL_T_PUTS;
"gets"                 return HKL_T_GETS;

"include"              return HKL_T_INCLUDE;

"("                    return HKL_T_LPAREN;
")"                    return HKL_T_RPAREN;
"{"                    return HKL_T_LBRACE;
"}"                    return HKL_T_RBRACE;
"["                    return HKL_T_LBRACKET;
"]"                    return HKL_T_RBRACKET;
","                    return HKL_T_COMMA;
"."                    return HKL_T_DOT;

.                      yyerror(yytext);

