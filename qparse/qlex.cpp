#include "qlex.h"

int lineCount = 1;
int errorCount = 0;


int nKeywords = 32;
char *keywords[] = {"auto", "break", "case", "char", "const", "continue",
                    "default", "do", "double", "else", "enum", "extern",
                    "float", "for", "goto", "if", "int", "long", "register",
                    "return", "short", "signed", "sizeof", "static", "struct",
                    "switch", "typedef", "union", "unsigned", "void", "volatile",
                    "while"};

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

int isKeyword(char *str) {
    for (int i = 0; i < nKeywords; ++i) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *upper(char *str) {
    char *upperStr = (char *) malloc(strlen(str) + 1);
    int i;
    for (i = 0; i < strlen(str); ++i) {
        upperStr[i] = toupper(str[i]);
    }
    upperStr[i] = '\0';
    return upperStr;
}

vector<Token> getTokens() {
    vector<Token> inputTokens;
    Token EM("$");

    Token SEMICOLON(";");
    Token COMMA(",");
    Token QUESTION("?");
    Token COLON(":");
    
    Token NOT("!");
    Token OROR("||");
    Token ANDAND("&&");
    
    Token INVERT("~");
    Token OR("|");
    Token XOR("^");
    Token AND("&");
    
    Token EQEQ("==");
    Token NEQ("!=");

    Token EQ("=");
    Token PLUSEQ("+=");
    Token MINUSEQ("-=");
    Token ANDEQ("&=");
    Token XOREQ("^=");
    Token OREQ("|=");
    
    Token LT("<");
    Token GT(">");
    Token LEQ("<=");
    Token GEQ(">=");

    Token PLUS("+");
    Token MINUS("-");

    Token LPAREN("(");
    Token RPAREN(")");

    Token INCOP("++");
    Token DECOP("--");

    Token LTHIRD("[");
    Token RTHIRD("]");

    Token LCURL("{");
    Token RCURL("}");

    Token DOT(".");
    Token PDOP("->");

    char *str = (char *) malloc(BUFSIZ + 1);
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
            lexeme = (char *) malloc(MAX_TOKEN_LENGTH);
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
                inputTokens.push_back(QUESTION);
                curr_state = 0;
            } else if (ch == ':') {
                inputTokens.push_back(COLON);
                curr_state = 0;
            } else if (ch == '=') {
                curr_state = 11;
                lexeme[k++] = ch;
            } else if (ch == '(') {
                inputTokens.push_back(LPAREN);
                curr_state = 0;
            } else if (ch == ')') {
                inputTokens.push_back(RPAREN);
                curr_state = 0;
            } else if (ch == '{') {
                inputTokens.push_back(LCURL);
                curr_state = 0;
            } else if (ch == '}') {
                inputTokens.push_back(RCURL);
                curr_state = 0;
            } else if (ch == '[') {
                inputTokens.push_back(LTHIRD);
                curr_state = 0;
            } else if (ch == ']') {
                inputTokens.push_back(RTHIRD);
                curr_state = 0;
            } else if (ch == ',') {
                inputTokens.push_back(COMMA);
                curr_state = 0;
            } else if (ch == ';') {
                inputTokens.push_back(SEMICOLON);
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
                inputTokens.push_back(INVERT);
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
                inputTokens.push_back(Token("CONST_INT", lexeme));
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
                inputTokens.push_back(DOT);
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
                inputTokens.push_back(Token("CONST_FLOAT", lexeme));
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
                inputTokens.push_back(Token("CONST_FLOAT", lexeme));
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
                    inputTokens.push_back(Token(upperStr));
                    free(upperStr);
                } else {
                    inputTokens.push_back(Token("ID", lexeme));
                }
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 11) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(EQEQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(EQ);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 12) {
            if (ch == '&') {
                lexeme[k++] = ch;
                inputTokens.push_back(ANDAND);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(ANDEQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(AND);
                i--;
                curr_state = 0;
            } 
        } else if (curr_state == 13) {
            if (ch == '|') {
                lexeme[k++] = ch;
                inputTokens.push_back(OROR);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(OREQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(OR);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 14) {
            if (ch == '+') {
                lexeme[k++] = ch;
                inputTokens.push_back(INCOP);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(PLUSEQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(PLUS);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 15) {
            if (ch == '-') {
                lexeme[k++] = ch;
                inputTokens.push_back(DECOP);
                curr_state = 0;
            } else if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(MINUSEQ);
                curr_state = 0;
            } else if (ch == '>') {
                lexeme[k++] = ch;
                inputTokens.push_back(PDOP);
                curr_state = 0;
            } else {
                inputTokens.push_back(MINUS);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 16) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(Token(lexeme));
                curr_state = 0;
            } else {
                inputTokens.push_back(Token(lexeme));
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 17) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(XOREQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(XOR);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 18) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(LEQ);
                curr_state = 0;
            } else if (ch == '<') {
                lexeme[k++] = ch;
                curr_state = 20;
            } else {
                inputTokens.push_back(LT);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 19) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(GEQ);
                curr_state = 0;
            } else if (ch == '>') {
                lexeme[k++] = ch;
                curr_state = 20;
            } else {
                inputTokens.push_back(GT);
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 20) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(Token(lexeme));
                curr_state = 0;
            } else {
                inputTokens.push_back(Token(lexeme));
                i--;
                curr_state = 0;
            }
        } else if (curr_state == 21) {
            if (ch == '=') {
                lexeme[k++] = ch;
                inputTokens.push_back(NEQ);
                curr_state = 0;
            } else {
                inputTokens.push_back(NOT);
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
                inputTokens.push_back(Token(lexeme));
                curr_state = 0;
            } else {
                inputTokens.push_back(Token(lexeme));
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
                inputTokens.push_back(Token("CONST_STR", lexeme));
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
                inputTokens.push_back(Token("CONST_STR", lexeme));
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

    inputTokens.push_back(EM);

    return inputTokens;
}