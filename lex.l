%option noyywrap

%{

#include <bits/stdc++.h>
#include "SymbolTable.cpp"
#include "y.tab.h"

using namespace std;

#define YYSTYPE SymbolInfo*

void yyerror(char *);

extern int error_count, line_count;
extern SymbolTable symbolTable;
extern FILE *logFile;

%}

Whitespace [ \r\v\t\f]+
Letter [a-zA-Z]
Digit [0-9]
AlphaNumeric [a-zA-Z0-9]

CONST_INT {Digit}+
CONST_FLOAT {Digit}*\.{Digit}+([Ee][+-]?{Digit}+)?|{Digit}+([Ee][+-]?{Digit}+)?
ILL_FORMED_NUM_ERROR {Digit}*\.{Digit}+([Ee][+-]?{Digit}+)?|{Digit}+([Ee][+-]?{Digit}+)?\.{Digit}+
TOO_MANY_DECIMAL_POINTS_ERROR {Digit}*\.{Digit}*\.({Digit}|{Letter}|\.)*
CONST_CHAR '[^\'\n\\]'|"'\\n'"|"'\\t'"|"'\\0'"|"'\\a'"|"'\\\\'"|"'\\r'"|"'\\f'"|"'\\b'"|"'\\v'"|"'\\''"
EMPTY_CHAR_ERROR ''
MULTI_CHAR_ERROR '[^\'\n][^\'\n]+'
UNFINISHED_CHAR_ERROR ('[^\'\n]*)|('\\')
ID ({Letter}|_)({AlphaNumeric}|_)*
STRING \"([^\\\"\n]|(\\.)|(\\\r?\n))*\"
INVALID_IDENTIFIER_ERROR ({AlphaNumeric}|[\._])+
UNFINISHED_STRING_ERROR \"([^\\\"\n]|(\\.)|(\\\r?\n))*
COMMENT (\/\/(.|(\\\r?\n))*)|(\/\*([^\*]|(\*[^\/]))*\*\/)
UNFINISHED_COMMENT_ERROR \/\*([^\*]|(\*[^\/]))*

%%

\n {line_count++;}

{Whitespace} {}

if { return IF; }
for { return FOR; }
do { return DO; }
int { return INT; }
float { return FLOAT; }
void { return VOID; }
switch { return SWITCH;	}
default { return DEFAULT; }
else { return ELSE;	}
while {	return WHILE; }
break { return BREAK; }
char { return CHAR;	}
double { return DOUBLE; }
return { return RETURN; }
case { return CASE; }
continue { return CONTINUE; }
println { return PRINTLN; }

{CONST_INT} {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "int");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return CONST_INT;
}
	
{CONST_FLOAT} {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "float");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return CONST_FLOAT;
}

{ILL_FORMED_NUM_ERROR} {}

{TOO_MANY_DECIMAL_POINTS_ERROR} {}
	
{CONST_CHAR} { return CONST_CHAR; }	
	
{UNFINISHED_CHAR_ERROR} {}	
	
{MULTI_CHAR_ERROR} {}

{EMPTY_CHAR_ERROR} {}
	
"+"|"-" {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "ID");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return ADDOP;	
}
	
"*"|"/"|"%" {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "ID");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return MULOP;	
}

"++" {	return INCOP; }
"--" {	return DECOP; }

	
"<"|"<="|">"|">="|"=="|"!=" {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "ID");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return RELOP;
}
	
"=" { return ASSIGNOP; }
	
"&&"|"||" {
	SymbolInfo *symbolInfo = new SymbolInfo(string(yytext), "ID");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return LOGICOP;
}
		
"!" { return NOT; }
	
"(" { return LPAREN; }
	
")" { return RPAREN; }
	
"{" { return LCURL; }
	
"}" { return RCURL; }
	
"[" { return LTHIRD; }
	
"]" { return RTHIRD; }
	
"," { return COMMA; }
	
";" { return SEMICOLON; }
	
{ID} {
	SymbolInfo* symbolInfo = new SymbolInfo(string(yytext), "ID");
	yylval.syminfo = (YYSTYPE) symbolInfo;
	return ID;
}

{INVALID_IDENTIFIER_ERROR} {}

{STRING} {}

{UNFINISHED_STRING_ERROR} {}

{COMMENT} {}

{UNFINISHED_COMMENT_ERROR} {}

. {}

%%
