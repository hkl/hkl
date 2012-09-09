/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     HKL_T_NEWLINE = 258,
     HKL_T_IF = 259,
     HKL_T_ELSE = 260,
     HKL_T_FOR = 261,
     HKL_T_WHILE = 262,
     HKL_T_END = 263,
     HKL_T_RETURN = 264,
     HKL_T_CLASS = 265,
     HKL_T_FUNCTION = 266,
     HKL_T_NIL = 267,
     HKL_T_NULL = 268,
     HKL_T_INTEGER = 269,
     HKL_T_REAL = 270,
     HKL_T_STRING = 271,
     HKL_T_SELF = 272,
     HKL_T_UNIQUE = 273,
     HKL_T_CONST = 274,
     HKL_T_PROTOTYPE = 275,
     HKL_T_PROTECTED = 276,
     HKL_T_PUTS = 277,
     HKL_T_GETS = 278,
     HKL_T_INCLUDE = 279,
     HKL_T_LPAREN = 280,
     HKL_T_RPAREN = 281,
     HKL_T_LBRACE = 282,
     HKL_T_RBRACE = 283,
     HKL_T_LBRACKET = 284,
     HKL_T_RBRACKET = 285,
     HKL_T_COMMA = 286,
     HKL_T_DOT = 287
   };
#endif
/* Tokens.  */
#define HKL_T_NEWLINE 258
#define HKL_T_IF 259
#define HKL_T_ELSE 260
#define HKL_T_FOR 261
#define HKL_T_WHILE 262
#define HKL_T_END 263
#define HKL_T_RETURN 264
#define HKL_T_CLASS 265
#define HKL_T_FUNCTION 266
#define HKL_T_NIL 267
#define HKL_T_NULL 268
#define HKL_T_INTEGER 269
#define HKL_T_REAL 270
#define HKL_T_STRING 271
#define HKL_T_SELF 272
#define HKL_T_UNIQUE 273
#define HKL_T_CONST 274
#define HKL_T_PROTOTYPE 275
#define HKL_T_PROTECTED 276
#define HKL_T_PUTS 277
#define HKL_T_GETS 278
#define HKL_T_INCLUDE 279
#define HKL_T_LPAREN 280
#define HKL_T_RPAREN 281
#define HKL_T_LBRACE 282
#define HKL_T_RBRACE 283
#define HKL_T_LBRACKET 284
#define HKL_T_RBRACKET 285
#define HKL_T_COMMA 286
#define HKL_T_DOT 287




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 14 "y.tab.y"

  int        integer;
  double     real;
  HklString* string;



/* Line 2068 of yacc.c  */
#line 122 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


