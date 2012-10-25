%{

#include <stdio.h>      // for scan functions

#include "hkl.h"

extern int yyerror(const char* msg);

// This is used for building strings
HklString* string_buf = NULL;

%}

/* Exclusive start state for strings */
%x stringbuild

%%

"//".*[\n]             yylineno++;/* Ignore Comments */
[ \t\r]                /* Ignore Whitespace */
[\n]                   yylineno++;

"if"                   return HKL_T_IF;
"else"                 return HKL_T_ELSE;
"for"                  return HKL_T_FOR;
"while"                return HKL_T_WHILE;
"end"                  return HKL_T_END;
"return"               return HKL_T_RETURN;
"break"                return HKL_T_BREAK;
"continue"             return HKL_T_CONTINUE;
"assert"               return HKL_T_ASSERT;
"include"              return HKL_T_INCLUDE;
"case"				         return HKL_T_CASE;
"switch"               return HKL_T_SWITCH;
"default"              return HKL_T_DEFAULT;

"class"                return HKL_T_CLASS;
"function"             return HKL_T_FUNCTION;
"nil"                  return HKL_T_NIL;
"null"                 return HKL_T_NULL;

"int"                  return HKL_T_INT;
"real"                 return HKL_T_REAL;
"string"               return HKL_T_STRING;
"array"                return HKL_T_ARRAY;
"hash"                 return HKL_T_HASH;
"instance"             return HKL_T_INSTANCE;

"self"                 return HKL_T_SELF;

"unique"               return HKL_T_UNIQUE;
"constant"             return HKL_T_CONSTANT;
"prototype"            return HKL_T_PROTOTYPE;
"protected"            return HKL_T_PROTECTED;
"local"                return HKL_T_LOCAL;
"global"               return HKL_T_GLOBAL;

"true"                 return HKL_T_TRUE;
"false"                return HKL_T_FALSE;

"puts"                 return HKL_T_PUTS;
"gets"                 return HKL_T_GETS;
"hklr"                 return HKL_T_HKLR;

"("                    return HKL_T_LPAREN;
")"                    return HKL_T_RPAREN;
"{"                    return HKL_T_LBRACE;
"}"                    return HKL_T_RBRACE;
"["                    return HKL_T_LBRACKET;
"]"                    return HKL_T_RBRACKET;
":"                    return HKL_T_COLON;
","                    return HKL_T_COMMA;
"."                    return HKL_T_DOT;

"++"                   return HKL_T_INCREMENT;
"--"                   return HKL_T_DECREMENT;
"||"                   return HKL_T_OR;
"&&"                   return HKL_T_AND;
"<="                   return HKL_T_LESS_EQUAL;
">="                   return HKL_T_GREATER_EQUAL;
"<"                    return HKL_T_LESS;
">"                    return HKL_T_GREATER;
"=="                   return HKL_T_EQUAL;
"!="                   return HKL_T_NOT_EQUAL;
"+="                   return HKL_T_PLUS_ASSIGN;
"-="                   return HKL_T_MINUS_ASSIGN;
"*="                   return HKL_T_ASTERISK_ASSIGN;
"/="                   return HKL_T_DIVIDE_ASSIGN;
"%="                   return HKL_T_MOD_ASSIGN;
"&="                   return HKL_T_BITWISE_AND_ASSIGN;
"|="                   return HKL_T_BITWISE_OR_ASSIGN;
"^="                   return HKL_T_BITWISE_XOR_ASSIGN;
"~="                   return HKL_T_BITWISE_NOT_ASSIGN;

"="                    return HKL_T_ASSIGN;
"!"                    return HKL_T_NOT;
"+"                    return HKL_T_PLUS;
"-"                    return HKL_T_MINUS;
"*"                    return HKL_T_ASTERISK;
"/"                    return HKL_T_DIVIDE;
"%"                    return HKL_T_MOD;
"&"                    return HKL_T_BITWISE_AND;
"|"                    return HKL_T_BITWISE_OR;
"^"                    return HKL_T_BITWISE_XOR;
"~"                    return HKL_T_BITWISE_NOT;
".."                   return HKL_T_RANGE;

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.string = hkl_string_new_from_utf8(yytext);
  return HKL_T_ID;
}

[0-9]+ {
  sscanf(yytext, "%i", &yylval.integer);
  return HKL_T_INT_CONSTANT;
}

([0-9]+\.[0-9]+) {
  sscanf(yytext, "%lg", &yylval.real);
  return HKL_T_REAL_CONSTANT;
}

\"                         { BEGIN stringbuild; string_buf = hkl_string_new(); }
<stringbuild>[^\\"]*       { hkl_string_cat_utf8(string_buf, yytext); }
<stringbuild>\\n           { hkl_string_cat_utf8(string_buf, "\n"); }
<stringbuild>\\\n          { /* ignore backslashed newlines */ }
<stringbuild>\\t           { hkl_string_cat_utf8(string_buf, "\t"); }
<stringbuild>\\\\          { hkl_string_cat_utf8(string_buf, "\\"); }
<stringbuild>\\\"          { hkl_string_cat_utf8(string_buf, "\""); }
<stringbuild>\"            { yylval.string = string_buf; BEGIN 0; return HKL_T_STRING_CONSTANT; }

<stringbuild>\\.           yyerror(yytext);

.                          yyerror(yytext);
