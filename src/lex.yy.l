%{

#include <assert.h>
#include <stdio.h>      // for scan functions

#include "hkl.h"

extern int yyerror(const char* msg);

// This is used for building strings
HklString* string_buf = NULL;

%}

/* Exclusive start state for strings */
%x stringbuild
%x includebuild
%x includename

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
"load"                 return HKL_T_LOAD;
"collect"              return HKL_T_COLLECT;

"include"              BEGIN includebuild;
<includebuild>[ \t\r]  /* Ignore Whitespace */
<includebuild>[\n]     yylineno++;
<includebuild>\"       { BEGIN(includename); string_buf = hkl_string_new(); }
<includebuild>.        return yyerror(yytext);

<includename>[^\\"]*   { hkl_string_cat_utf8(string_buf, yytext); }
<includename>\"        {
                         yyin = fopen(string_buf->utf8_data, "r");

                         assert(yyin != NULL);

                         yypush_buffer_state(yy_create_buffer(yyin, YY_BUF_SIZE));

                         hkl_string_free(string_buf);

                         BEGIN 0; 
                       }
<includename>.         return yyerror(yytext);

"case"				         return HKL_T_CASE;
"switch"               return HKL_T_SWITCH;
"default"              return HKL_T_DEFAULT;

"class"                return HKL_T_CLASS;
"function"             return HKL_T_FUNCTION;
"nil"                  return HKL_T_NIL;

"int"                  return HKL_T_INT;
"real"                 return HKL_T_REAL;
"string"               return HKL_T_STRING;
"array"                return HKL_T_ARRAY;
"hash"                 return HKL_T_HASH;
"type"                 return HKL_T_TYPE;
"instance"             return HKL_T_INSTANCE;
"func"                 return HKL_T_FUNC;

"self"                 return HKL_T_SELF;

"unique"               return HKL_T_UNIQUE;
"constant"             return HKL_T_CONSTANT;
"prototype"            return HKL_T_PROTOTYPE;
"protected"            return HKL_T_PROTECTED;
"local"                return HKL_T_LOCAL;
"global"               return HKL_T_GLOBAL;

"true"                 return HKL_T_TRUE;
"false"                return HKL_T_FALSE;
"maybe"                return HKL_T_MAYBE;

"puts"                 return HKL_T_PUTS;
"gets"                 return HKL_T_GETS;
"hklr"                 return HKL_T_HKLR;
"typeof"               return HKL_T_TYPEOF;

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
"as"                   return HKL_T_AS;

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.string = hkl_string_new_from_utf8(yytext);
  return HKL_T_ID;
}

[0-9]+ {
  sscanf(yytext, "%i", &yylval.integer);
  return HKL_T_INT_CONSTANT;
}

[0-9]*\.?[0-9]+([eE][\-\+]?[0-9]+)? {
  sscanf(yytext, "%lf", &yylval.real);
  return HKL_T_REAL_CONSTANT;
}

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
"#"                    return HKL_T_CRUNCH;
".."                   return HKL_T_RANGE;


\"                         { BEGIN stringbuild; string_buf = hkl_string_new(); }
<includebuild>[\n]         { hkl_string_cat_utf8(string_buf, "\n"); yylineno++; }
<stringbuild>[^\\"]*       { hkl_string_cat_utf8(string_buf, yytext); }
<stringbuild>\\n           { hkl_string_cat_utf8(string_buf, "\n"); }
<stringbuild>\\\n          { /* ignore backslashed newlines */ }
<stringbuild>\\t           { hkl_string_cat_utf8(string_buf, "\t"); }
<stringbuild>\\\\          { hkl_string_cat_utf8(string_buf, "\\"); }
<stringbuild>\\\"          { hkl_string_cat_utf8(string_buf, "\""); }
<stringbuild>\"            { yylval.string = string_buf; string_buf = NULL; BEGIN 0; return HKL_T_STRING_CONSTANT; }

<stringbuild>\\.           return yyerror(yytext);

<<EOF>>                    {
                             yypop_buffer_state();
                 
                             if (YY_CURRENT_BUFFER == NULL)
                             {
                              yyterminate();
                             }
                           }

.                          return yyerror(yytext);
