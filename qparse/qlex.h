#ifndef QLEX_H
#define QLEX_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>

#include "token.h"

int isWS(char);
int isDigit(char);
int isLowerLetter(char);
int isUpperLetter(char);
int isLetter(char);
int isLetter_(char);
int isKeyword(char *);
char *upper(char *);

#define MAX_TOKEN_LENGTH 100

vector<Token> getTokens();

#endif