#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int isWS(char);
int isDigit(char);
int isLowerLetter(char);
int isUpperLetter(char);
int isLetter(char);
int isLetter_(char);
int isKeyword(char *);
char *upper(char *);

int lineCount = 1;
int errorCount = 0;

int isWS(char ch) {
    if (ch == '\n') {
        lineCount++;
    }
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0';
}

int isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

int isLowerLetter(char ch) {
    return ch >= 'a' && ch <= 'z';
}

int isUpperLetter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

int isLetter(char ch) {
    return isLowerLetter(ch) || isUpperLetter(ch);
}

int isLetter_(char ch) {
    return isLetter(ch) || ch == '_';
}

int nKeywords = 32;
char *keywords[] = {"auto", "break", "case", "char", "const", "continue",
                    "default", "do", "double", "else", "enum", "extern",
                    "float", "for", "goto", "if", "int", "long", "register",
                    "return", "short", "signed", "sizeof", "static", "struct",
                    "switch", "typedef", "union", "unsigned", "void", "volatile",
                    "while"};

int isKeyword(char *str) {
    for (int i = 0; i < nKeywords; ++i) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *upper(char *str) {
    char *upperStr = malloc(strlen(str) + 1);
    int i;
    for (i = 0; i < strlen(str); ++i) {
        upperStr[i] = toupper(str[i]);
    }
    upperStr[i] = '\0';
    return upperStr;
}

#define MAX_TOKEN_LENGTH 100

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "qcc: fatal error: no input files\n");
        fprintf(stderr, "compilation terminated.\n");
        fprintf(stderr, "Usage: %s <input>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    freopen(argv[1], "r", stdin);
    freopen("out", "w", stdout);
    freopen("log", "w", stderr);
    
    char *str = malloc(BUFSIZ + 1);
    memset(str, 0, BUFSIZ + 1);

    fread(str, BUFSIZ, 1, stdin);

    int curr_state = 0;

    char *lexeme;
    int k = 0;

    int alloc = 0;

    for (int i = 0; i <= strlen(str); ++i) {
        char ch = str[i];
        if (curr_state == 0) {
            if (alloc) {
                free(lexeme);
            }
            lexeme = malloc(MAX_TOKEN_LENGTH);
            alloc = 1;
            memset(lexeme, 0, sizeof(lexeme));
            k = 0;

            if (isWS(ch)) {
                curr_state = 0;
            } else if (isDigit(ch)) {
                curr_state = 1;
                lexeme[k++] = ch;
            } else if (ch == '.') {
                curr_state = 3;
                lexeme[k++] = ch;
            } else if (isLetter_(ch)) {
                curr_state = 10;
                lexeme[k++] = ch;
            } else if (ch == '+') {
                curr_state = 14;
                lexeme[k++] = ch;
            } else if (ch == '-') {
                curr_state = 15;
                lexeme[k++] = ch;
            } else if (ch == '*') {
                curr_state = 16;
                lexeme[k++] = ch;
            } else if (ch == '/') {
                curr_state = 22;
                lexeme[k++] = ch;
            } else if (ch == '%') {
                curr_state = 16;
                lexeme[k++] = ch;
            } else if (ch == '^') {
                lexeme[k++] = ch;
                curr_state = 17;
            } else if (ch == '&') {
                lexeme[k++] = ch;
                curr_state = 12;
            } else if (ch == '|') {
                lexeme[k++] = ch;
                curr_state = 13;
            } else if (ch == '?') {
                printf("<QUESTION>");
                curr_state = 0;
            } else if (ch == ':') {
                printf("<COLON>");
                curr_state = 0;
            } else if (ch == '=') {
                curr_state = 11;
                lexeme[k++] = ch;
            } else if (ch == '(') {
                printf("<LPAREN>");
                curr_state = 0;
            } else if (ch == ')') {
                printf("<RPAREN>");
                curr_state = 0;
            } else if (ch == '{') {
                printf("<LCURL>");
                curr_state = 0;
            } else if (ch == '}') {
                printf("<RCURL>");
                curr_state = 0;
            } else if (ch == '[') {
                printf("<LTHIRD>");
                curr_state = 0;
            } else if (ch == ']') {
                printf("<RTHIRD>");
                curr_state = 0;
            } else if (ch == ',') {
                printf("<COMMA>");
                curr_state = 0;
            } else if (ch == ';') {
                printf("<SEMICOLON>");
                curr_state = 0;
            } else if (ch == '<') {
                lexeme[k++] = ch;
                curr_state = 18;
            } else if (ch == '>') {
                lexeme[k++] = ch;
                curr_state = 19;
            } else if (ch == '!') {
                lexeme[k++] = ch;
                curr_state = 21;
            } else if (ch == '~') {
                printf("<INVERT>");
                curr_state = 0;
            } else if (ch == '\'') {
                lexeme[k++] = ch;
                curr_state = 27;
            } else if (ch == '\"') {
                lexeme[k++] = ch;
                curr_state = 30;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 1) {
            if (isDigit(ch)) {
                curr_state = 1;
                lexeme[k++] = ch;
            } else if (ch == '.') {
                curr_state = 3;
                lexeme[k++] = ch;
            } else if (ch == 'e' || ch == 'E') {
                curr_state = 5;
                lexeme[k++] = ch;
            } else if (!isDigit(ch) && !isLetter_(ch)) {
                printf("<CONST_INT, %s>", lexeme);
                --i;
                curr_state = 0;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 3) {
            if (isDigit(ch)) {
                curr_state = 4;
                lexeme[k++] = ch;
            } else {
                printf("<DOT>");
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 4) {
            if (isDigit(ch)) {
                curr_state = 4;
                lexeme[k++] = ch;
            } else if (ch == 'e' || ch == 'E') {
                curr_state = 5;
                lexeme[k++] = ch;
            } else if (ch == '.') {
                lexeme[k++] = ch;
                curr_state = 9;
            } else if (!isDigit(ch) && !isLetter_(ch)) {
                printf("<CONST_FLOAT, %s>", lexeme);
                --i;
                curr_state = 0;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 5) {
            if (isDigit(ch)) {
                curr_state = 7;
                lexeme[k++] = ch;
            } else if (ch == '+' || ch == '-') {
                curr_state = 8;
                lexeme[k++] = ch;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 7) {
            if (isDigit(ch)) {
                curr_state = 7;
                lexeme[k++] = ch;
            } else if (ch == '.') {
                lexeme[k++] = ch;
                curr_state = 9;
            } else if (!isDigit(ch) && !isLetter_(ch)) {
                printf("<CONST_FLOAT, %s>", lexeme);
                --i;
                curr_state = 0;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 8) {
            if (isDigit(ch)) {
                curr_state = 7;
                lexeme[k++] = ch;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 9) {
            int errorLine = lineCount;
            if (isWS(ch) || ch == ';') {
                fprintf(stderr, "Line %d: invalid token %s\n", errorLine, lexeme);
                errorCount++;
                i--;
                curr_state = 0;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 10) {
            if (isLetter_(ch) || isDigit(ch)) {
                curr_state = 10;
                lexeme[k++] = ch;
            } else {
                if (isKeyword(lexeme)) {
                    char *upperStr = upper(lexeme);
                    printf("<%s>", upperStr);
                    free(upperStr);
                } else {
                    printf("<ID, %s>", lexeme);
                }
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 11) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<RELOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<ASSIGNOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 12) {
            if (ch == '&') {
                lexeme[k++] = ch;
                printf("<LOGICOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<BITOP, %s>", lexeme);
                i--;
                curr_state = 0;
            } 
        } else if (curr_state == 13) {
            if (ch == '|') {
                lexeme[k++] = ch;
                printf("<LOGICOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<BITOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 14) {
            if (ch == '+') {
                lexeme[k++] = ch;
                printf("<INCOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<ADDOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 15) {
            if (ch == '-') {
                lexeme[k++] = ch;
                printf("<INCOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<ADDOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 16) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<MULOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 17) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<BITOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 18) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<RELOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '<') {
                lexeme[k++] = ch;
                curr_state = 20;
            } else {
                printf("<RELOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 19) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<RELOP, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '>') {
                lexeme[k++] = ch;
                curr_state = 20;
            } else {
                printf("<RELOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 20) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<SHIFTOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 21) {
            if (ch == '=') {
                lexeme[k++] = ch;
                printf("<RELOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<NOT>");
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 22) {
            if (ch == '*') {
                curr_state = 25;
            } else if (ch == '/') {
                curr_state = 23;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                printf("<ASSIGNOP, %s>", lexeme);
                curr_state = 0;
            } else {
                printf("<MULOP, %s>", lexeme);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 23) {
            if (ch == '\\') {
                curr_state = 24;
            } else if (ch == '\n') {
                lineCount++;
                curr_state = 0;
            } else {
                curr_state = 23;
            }
        } else if (curr_state == 24) {
            curr_state = 23;
        } else if (curr_state == 25) {
            if (ch == '*') {
                curr_state = 26;
            } else {
                curr_state = 25;
            }
        } else if (curr_state == 26) {
            if (ch == '/') {
                curr_state = 0;
            } else {
                curr_state = 25;
            }
        } else if (curr_state == 27) {
            if (ch == '\'') {
                lexeme[k++] = ch;
                curr_state = 9;
            } else if (ch == '\\') {
                lexeme[k++] = ch;
                curr_state = 28;
            } else {
                lexeme[k++] = ch;
                curr_state = 29;
            }
        } else if (curr_state == 28) {
            if (ch == 'n' || ch == 't' || ch == '\\' || ch == '\'' || ch == '\"' || ch == 'a' || ch == 'f' 
            || ch == 'r' || ch == 'b' || ch == 'v' || ch == '?' || ch == '0') {
                lexeme[k++] = ch;
                curr_state = 29;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 29) {
            if (ch == '\'') {
                lexeme[k++] = ch;
                printf("<CONST_CHAR, %s>", lexeme);
                curr_state = 0;
            } else if (ch >= '0' && ch <= '7') {
                lexeme[k++] = ch;
                curr_state = 32;
            } else {
                lexeme[k++] = ch;
                curr_state = 9;
            }
        } else if (curr_state == 30) {
            if (ch == '\\') {
                curr_state = 31;
            } else if (ch == '\"') {
                lexeme[k++] = ch;
                printf("<CONST_STR, %s>", lexeme);
                curr_state = 0;
            } else if (ch == '\n') {
                curr_state = 9;
            } else {
                lexeme[k++] = ch;
                curr_state = 30;
            }
        } else if (curr_state == 31) {
            if (ch == '\n') {
                lineCount++;
                curr_state = 30;
            } else if (ch == 'n' || ch == 't' || ch == '\\' || ch == '\'' || ch == '\"' || ch == 'a' || ch == 'f' 
            || ch == 'r' || ch == 'b' || ch == 'v' || ch == '?' || ch == '0') {
                lexeme[k++] = '\\';
                lexeme[k++] = ch;
                curr_state = 30;
            } else {
                lexeme[k++] = '\\';
                lexeme[k++] = ch;
                curr_state = 9;
            }
        }
    }

    if (alloc) {
        free(lexeme);
    }

    free(str);

    fprintf(stderr, "Total lines: %d\n", lineCount);
    fprintf(stderr, "Total errors: %d\n", errorCount);

    return 0;
}