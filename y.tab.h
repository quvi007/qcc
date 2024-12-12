/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    DO = 262,                      /* DO  */
    INT = 263,                     /* INT  */
    FLOAT = 264,                   /* FLOAT  */
    DOUBLE = 265,                  /* DOUBLE  */
    CHAR = 266,                    /* CHAR  */
    VOID = 267,                    /* VOID  */
    SWITCH = 268,                  /* SWITCH  */
    RETURN = 269,                  /* RETURN  */
    CONTINUE = 270,                /* CONTINUE  */
    DEFAULT = 271,                 /* DEFAULT  */
    BREAK = 272,                   /* BREAK  */
    CASE = 273,                    /* CASE  */
    ASSIGNOP = 274,                /* ASSIGNOP  */
    NOT = 275,                     /* NOT  */
    LPAREN = 276,                  /* LPAREN  */
    RPAREN = 277,                  /* RPAREN  */
    LCURL = 278,                   /* LCURL  */
    RCURL = 279,                   /* RCURL  */
    LTHIRD = 280,                  /* LTHIRD  */
    RTHIRD = 281,                  /* RTHIRD  */
    INCOP = 282,                   /* INCOP  */
    DECOP = 283,                   /* DECOP  */
    SEMICOLON = 284,               /* SEMICOLON  */
    COMMA = 285,                   /* COMMA  */
    PRINTLN = 286,                 /* PRINTLN  */
    ADDOP = 287,                   /* ADDOP  */
    MULOP = 288,                   /* MULOP  */
    RELOP = 289,                   /* RELOP  */
    LOGICOP = 290,                 /* LOGICOP  */
    ID = 291,                      /* ID  */
    CONST_INT = 292,               /* CONST_INT  */
    CONST_FLOAT = 293,             /* CONST_FLOAT  */
    CONST_CHAR = 294,              /* CONST_CHAR  */
    LOWER_THAN_ELSE = 295          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define DO 262
#define INT 263
#define FLOAT 264
#define DOUBLE 265
#define CHAR 266
#define VOID 267
#define SWITCH 268
#define RETURN 269
#define CONTINUE 270
#define DEFAULT 271
#define BREAK 272
#define CASE 273
#define ASSIGNOP 274
#define NOT 275
#define LPAREN 276
#define RPAREN 277
#define LCURL 278
#define RCURL 279
#define LTHIRD 280
#define RTHIRD 281
#define INCOP 282
#define DECOP 283
#define SEMICOLON 284
#define COMMA 285
#define PRINTLN 286
#define ADDOP 287
#define MULOP 288
#define RELOP 289
#define LOGICOP 290
#define ID 291
#define CONST_INT 292
#define CONST_FLOAT 293
#define CONST_CHAR 294
#define LOWER_THAN_ELSE 295

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 99 "1805007.y"

    SymbolInfo* syminfo;

#line 151 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
