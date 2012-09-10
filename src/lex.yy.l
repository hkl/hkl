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

"puts"                 return HKL_T_PUTS;
"gets"                 return HKL_T_GETS;

"include"              return HKL_T_INCLUDE;

"assert"               return HKL_T_ASSERT;

"="                    return HKL_T_ASSIGN;

"("                    return HKL_T_LPAREN;
")"                    return HKL_T_RPAREN;
"{"                    return HKL_T_LBRACE;
"}"                    return HKL_T_RBRACE;
"["                    return HKL_T_LBRACKET;
"]"                    return HKL_T_RBRACKET;
","                    return HKL_T_COMMA;
"."                    return HKL_T_DOT;

"!"                    return HKL_T_NOT;
"||"                   return HKL_T_OR;
"&&"                   return HKL_T_AND;
"<="                   return HKL_T_LESS_EQUAL;
">="                   return HKL_T_GREATER_EQUAL;
"<"                    return HKL_T_LESS;
">"                    return HKL_T_GREATER;
"=="                   return HKL_T_EQUAL;
"!="                   return HKL_T_NOT_EQUAL;

"+"                    return HKL_T_PLUS;
"-"                    return HKL_T_MINUS;
"*"                    return HKL_T_ASTERISK;
"/"                    return HKL_T_DIVIDE;
"%"                    return HKL_T_MOD;

"+="                   return HKL_T_PLUS_EQUAL;
"-="                   return HKL_T_MINUS_EQUAL;
"*="                   return HKL_T_ASTERISK_EQUAL;
"/="                   return HKL_T_DIVIDE_EQUAL;
"%="                   return HKL_T_MOD_EQUAL;

"~"                    return HKL_T_BITWISE_NOT;
"&"                    return HKL_T_BITWISE_AND;
"|"                    return HKL_T_BITWISE_OR;
"^"                    return HKL_T_BITWISE_XOR;

"&="                   return HKL_T_BITWISE_AND_EQUAL;
"|="                   return HKL_T_BITWISE_OR_EQUAL;
"^="                   return HKL_T_XOR_EQUAL;

"++"                   return HKL_T_INCREMENT;
"--"                   return HKL_T_DECREMENT;

.                      yyerror(yytext);

