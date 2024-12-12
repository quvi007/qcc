%{

#include <bits/stdc++.h>
#include "SymbolTable.cpp"
#define yydebug 1

using namespace std;

FILE *in, *fpLog, *fpError;
extern FILE *yyin;

int line_count = 1, error_count = 0;

string currentFunctionReturnType = "error", currentFunctionLabel;

SymbolTable symbolTable(30);

int yyparse(void);
int yylex(void);

int labelCount = 0;

string newLabel() {
    return "label_" + to_string(labelCount++);
}

stack<string> labels;

ofstream asmCode;
string globalVars;

string optimizeCode(string str) {
    // stringstream ss(str);
    // vector<string> lines;
    // string str;
    // while ((getline(ss, str, '\n'))) {
    //     lines.push_back(str);
    // }
    // for (int i = 0; i < lines.size() - 2; ++i) {
    //     if ()
    // }
    return "";
}

void yyerror(const char *s) {
	fprintf(fpError, "Syntax error at line %d : \"%s\" \n", line_count, s);
}

void consoleLog(FILE *fp, const string &rule, const string &token, int line) {
    fprintf(fp, "Line %d: %s\n\n%s\n\n", line, rule.c_str(), token.c_str());
}

void consoleError(FILE *fp1, FILE *fp2, const string &msg, int line) {
    fprintf(fp1, "Error at line %d: %s\n\n", line, msg.c_str());
    fprintf(fp2, "Error at line %d: %s\n\n", line, msg.c_str());
}

string getArrayName(const string &s) {
    string name;
    getline(stringstream(s), name, '[');
    return name;
}

int getArraySize(const string &s) {
    string str;
    stringstream ss(s);
    while (getline(ss, str, '['));
	getline(ss, str, ']');
	ss = stringstream(str);
	int ans; ss >> ans;
	return ans;
}

bool isArray(const string &s) {
    if (s.find("[") == string::npos && s.find("]") == string::npos)
        return false;
    return true;
}

vector<string> tokenizeString(const string &s, char delim) {
    vector<string> tokens;
    stringstream ss(s);
    string str;
    while (getline(ss, str, delim))
        tokens.push_back(str);
    return tokens;
}

bool isInvalidArrayElement(const string &s) {
    string str;
    stringstream ss(s);
    while (getline(ss, str, '['));
	getline(ss, str, ']');
	ss = stringstream(str);
	float ans; ss >> ans;
    return ans != getArraySize(s);
}

vector<Parameter> getParameters(const string &s) {
    vector<Parameter> parameters;
    vector<string> params = tokenizeString(s, ','), sParam;
    for (string param : params) {
        sParam = tokenizeString(param, ' ');
        parameters.push_back(Parameter(sParam[1], sParam[0]));
    }
    return parameters;
}

vector<string> getParameterTypes(const string &s) {
    vector<string> parameterTypes;
    vector<string> params = tokenizeString(s, ','), sParam;
    for (string param : params) {
        sParam = tokenizeString(param, ' ');
        parameterTypes.push_back(sParam[0]);
    }
    return parameterTypes;
}

%}

%define parse.error verbose

%union {
    SymbolInfo* syminfo;
}

%token IF ELSE FOR WHILE DO INT FLOAT DOUBLE CHAR VOID SWITCH RETURN CONTINUE DEFAULT BREAK CASE ASSIGNOP NOT LPAREN RPAREN LCURL RCURL LTHIRD RTHIRD INCOP DECOP SEMICOLON COMMA PRINTLN
%token <syminfo> ADDOP MULOP RELOP LOGICOP
%token <syminfo> ID CONST_INT CONST_FLOAT CONST_CHAR

%type <syminfo> start program unit var_declaration func_declaration
%type <syminfo> type_specifier declaration_list statements statement 
%type <syminfo> func_definition expression_statement expression 
%type <syminfo> logic_expression rel_expression simple_expression
%type <syminfo> term unary_expression factor variable argument_list
%type <syminfo> arguments parameter_list compound_statement if_statement

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

start : program
	{
		$$ = $1;
        consoleLog(fpLog, "start : program", "", line_count);
	}
	;

program : program unit
    {
        string symbolName = $1->getName() + "\n" + $2->getName();
        string symbolType = "program";
        $$ = new SymbolInfo(symbolName, symbolType);
        consoleLog(fpLog, "program : program unit", $$->getName(), line_count);
    }
	| unit
    {
        $$ = $1;
        consoleLog(fpLog, "program : unit", $$->getName(), line_count);
    }
	;
	
unit : var_declaration
        {
            $$ = $1;
            consoleLog(fpLog, "unit : var_declaration", $$->getName(), line_count);
        }
     | func_declaration
        {
            $$ = $1;
            consoleLog(fpLog, "unit : func_declaration", $$->getName(), line_count);
        }
     | func_definition
        {
            $$ = $1;
            consoleLog(fpLog, "unit : func_definition", $$->getName(), line_count);
        }
     ;
     
func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON
        {
            string returnType = $1->getName();
            string funcName = $2->getName();
            string parameters = $4->getName();

            vector<string> parameterTypes = getParameterTypes(parameters);
            vector<Parameter> parameterList(parameterTypes.size());
            for (int i = 0; i < parameterList.size(); ++i)
                parameterList[i] = Parameter(" ", parameterTypes[i]);

            SymbolInfo *symbolInfo = symbolTable.lookUp(funcName);
            if (symbolInfo) {
                error_count++;
                consoleError(fpError, fpLog, "Redeclaration of function '" + funcName +  "'", line_count);
            } else {
                SymbolInfo s;
                s.setDefined(false);
                s.setAsFunction(funcName, returnType, parameterList);
                symbolTable.insert(s);
            }
            string symbolName = returnType + " " + funcName + "(" + $4->getName() +  ");";
            string symbolType = "func_declaration";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON", $$->getName(), line_count);
            
        }       
		| type_specifier ID LPAREN RPAREN SEMICOLON
        {
            string returnType = $1->getName();
            string funcName = $2->getName();
            SymbolInfo *symbolInfo = symbolTable.lookUp(funcName);
            vector<Parameter> parameterList;

            if (symbolInfo) {
                error_count++;
                consoleError(fpError, fpLog, "Redeclaration of function '" + funcName +  "'", line_count);
            } else {
                SymbolInfo s;
                s.setDefined(false);
                s.setAsFunction(funcName, returnType, parameterList);
                symbolTable.insert(s);
            }
            string symbolName = returnType + " " + funcName + "();";
            string symbolType = "func_declaration";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON", $$->getName(), line_count);
        }
		;
		 
func_definition : type_specifier ID LPAREN parameter_list RPAREN
        {
            string returnType = currentFunctionReturnType = $1->getName();
            string funcName = $2->getName();
            vector<Parameter> parameterList = getParameters($4->getName());
            currentFunctionLabel = $2->getName();
            SymbolInfo *symbolInfo = symbolTable.lookUp(funcName);
            
            int currOffset = symbolTable.getCurrentScopeTable()->getLocalOffset();
            
            if (symbolInfo) {
                if (symbolInfo->isFunction()) {
                    if (symbolInfo->isDefined()) {
                        error_count++;
                        consoleError(fpError, fpLog, "Redefinition of function '" + funcName + "'", line_count);
                    } else {
                        bool definitionIsIncompatible = false;
                        if (returnType != symbolInfo->getType()) {
                            error_count++;
                            definitionIsIncompatible = true;
                            consoleError(fpError, fpLog, "Function return type does not with declaration", line_count);
                        }
                        
                        if (symbolInfo->getParameterList().size() != parameterList.size()) {
                            error_count++;
                            definitionIsIncompatible = true;
                            consoleError(fpError, fpLog, "Number of parameters is not compatible with declaration", line_count);
                        }

                        vector<Parameter> declaredParameterList = symbolInfo->getParameterList();
                        if (declaredParameterList.size() == parameterList.size()) {
                            for (int i = 0; i < declaredParameterList.size(); ++i) {
                                if (parameterList[i].getType() != declaredParameterList[i].getType()) {
                                    error_count++;
                                    definitionIsIncompatible = true;
                                    consoleError(fpError, fpLog, "Type mismatch of function parameter '" + parameterList[i].getName() + "'", line_count);
                                }
                            }
                        }

                        SymbolInfo s;
                        s.setAsFunction(funcName, returnType, parameterList);
                        s.setDefined(true);
                        symbolTable.remove(funcName);
                        symbolTable.insert(s);
                        symbolTable.enterScope();
                        for (int i = 0; i < parameterList.size(); ++i) {
                            if (!symbolTable.insert(parameterList[i].getName(), parameterList[i].getType())) {
                                error_count++;
                                consoleError(fpError, fpLog, "Duplicate parameter name '" + parameterList[i].getName() + "'", line_count);
                            }
                        }
                    }
                } else {
                    symbolTable.enterScope();
                    error_count++;
                    consoleError(fpError, fpLog, "Identifier '" + symbolInfo->getName() + "' is not a function", line_count);
                }
            } else {
                SymbolInfo s;
                s.setAsFunction(funcName, returnType, parameterList);
                s.setDefined(true);
                symbolTable.insert(s);
                symbolTable.enterScope();
                for (int i = 0; i < parameterList.size(); ++i) {
                    if (!symbolTable.insert(parameterList[i].getName(), parameterList[i].getType())) {
                        error_count++;
                        consoleError(fpError, fpLog, "Duplicate parameter name '" + parameterList[i].getName() + "'", line_count);
                    }
                }

                for (int i = parameterList.size() - 1; i >= 0; --i) {
                    SymbolInfo *symbolInfo = symbolTable.lookUp(parameterList[i].getName());
                    symbolInfo->setOffset(symbolTable.getCurrentScopeTable()->getLocalOffset() + 4 + (parameterList.size() - 1 - i) * 2);
                }
                if (funcName == "main")
                    asmCode << "main_func PROC\n";
                else
                    asmCode << funcName + " PROC\n";
                asmCode << "PUSH BP\n";
                asmCode << "MOV BP, SP\n";
            }
            symbolTable.getCurrentScopeTable()->setLocalOffset(currOffset);
        }
        compound_statement
        {
            string symbolName = $1->getName() + " " + $2->getName() + "(" + $4->getName() + ")" + $7->getName() + "\n";
            string symbolType = "func_definition";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement", $$->getName(), line_count);
            
            vector<Parameter> parameterList = getParameters($4->getName());

            asmCode << "label_" << currentFunctionLabel << ":\n";
            asmCode << "MOV SP, BP\n";
            asmCode << "POP BP\n";
            asmCode << "RET " + to_string((parameterList.size()) * 2) + "\n";
        
            asmCode << $2->getName() + " ENDP\n";
        }
		| type_specifier ID LPAREN RPAREN
        {
            currentFunctionLabel = $2->getName();
            string returnType = currentFunctionReturnType = $1->getName();
            string funcName = $2->getName();

            SymbolInfo *symbolInfo = symbolTable.lookUp(funcName);

            int currOffset = symbolTable.getCurrentScopeTable()->getLocalOffset();

            if (symbolInfo) {
                if (symbolInfo->isDefined()) {
                    error_count++;
                    consoleError(fpError, fpLog, "Redefinition of function '" + funcName + "'", line_count);
                } else {
                    bool definitionIsIncompatible = false;
                    if (returnType != symbolInfo->getType()) {
                        error_count++;
                        definitionIsIncompatible = true;
                        consoleError(fpError, fpLog, "Function return type does not with declaration", line_count);
                    }
                    
                    if (symbolInfo->getParameterList().size() != 0) {
                        error_count++;
                        definitionIsIncompatible = true;
                        consoleError(fpError, fpLog, "Number of parameters is not compatible with declaration", line_count);
                    }

                    if (!definitionIsIncompatible) {
                        symbolTable.remove(funcName);
                        SymbolInfo s;
                        vector<Parameter> parameterList;
                        s.setAsFunction(funcName, returnType, parameterList);
                        s.setDefined(true);
                        symbolTable.insert(s);
                    }
                }
            } else {
                SymbolInfo s;
                vector<Parameter> parameterList;
                s.setAsFunction(funcName, returnType, parameterList);
                s.setDefined(true);
                symbolTable.insert(s);
            }
            symbolTable.enterScope();

            if (funcName == "main")
                asmCode << "main_func PROC\n";
            else
                asmCode << funcName + " PROC\n";
            
            asmCode << "PUSH BP\n";
            asmCode << "MOV BP, SP\n";

            symbolTable.getCurrentScopeTable()->setLocalOffset(currOffset);
        }
        compound_statement
        {
            string symbolName = $1->getName() + " " + $2->getName() + "()" + $6->getName() + "\n";
            string symbolType = "func_definition";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "func_definition : type_specifier ID LPAREN RPAREN compound_statement", $$->getName(), line_count);
            
            asmCode << "label_" << currentFunctionLabel << ":\n";
            asmCode << "MOV SP, BP\n";
            asmCode << "POP BP\n";
            asmCode << "RET\n";

            if ($2->getName() == "main")
                asmCode << "main_func ENDP\n";
            else
                asmCode << $2->getName() + " ENDP\n";
        }
 		;


parameter_list  : parameter_list COMMA type_specifier ID
        {
            string symbolName = $1->getName() + "," + $3->getName() + " " + $4->getName();
            string symbolType = "parameter_list";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "parameter_list : parameter_list COMMA type_specifier ID", $$->getName(), line_count);
        }
		| parameter_list COMMA type_specifier
        {
            string symbolName = $1->getName() + "," + $3->getName();
            string symbolType = "parameter_list";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "parameter_list : parameter_list COMMA type_specifier", $$->getName(), line_count);
        }
 		| type_specifier ID
        {
            string symbolName = $1->getName() + " " + $2->getName();
            string symbolType = "parameter_list";
            $$ = new SymbolInfo(symbolName, symbolType);
			consoleLog(fpLog, "parameter_list : type_specifier ID", $$->getName(), line_count);
        }
		| type_specifier
        {
            $$ = $1;
            consoleLog(fpLog, "parameter_list : type_specifier", $$->getName(), line_count);
        }
 		;

 		
compound_statement : LCURL statements RCURL
            {
                string symbolName = "{\n" + $2->getName() + "\n}";
                string symbolType = "compound_statement";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "compound_statement : LCURL statements RCURL", $$->getName(), line_count);
                symbolTable.printAllScopeTables(fpLog);
                asmCode << "ADD SP, " + to_string(symbolTable.getCurrentScopeTable()->getVariableCount() * 2) + "\n"; 
                symbolTable.exitScope();
            }
 		    | LCURL RCURL
 		    {
                string symbolName = "{\n}";
                string symbolType = "compound_statement";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "compound_statement : LCURL RCURL", $$->getName(), line_count);
            }
            ;
 		    
var_declaration : type_specifier declaration_list SEMICOLON
            {
                string varType = $1->getName(), declarationList = $2->getName();
                if (varType == "void") {
                    error_count++;
                    consoleError(fpError, fpLog, "Variable type cannot be void", line_count);
                } else {
                    vector<string> varList = tokenizeString(declarationList, ',');
                    for (int i = 0; i < varList.size(); ++i) {
                        string var = varList[i];
                        SymbolInfo symbolInfo;
                        if (isArray(var))
                            symbolInfo.setAsArray(getArrayName(var), varType, getArraySize(var));
                        else symbolInfo = SymbolInfo(var, varType);
                        
                        symbolInfo.setOffset(symbolTable.getCurrentScopeTable()->getLocalOffset() - 2);
                        symbolTable.getCurrentScopeTable()->decreaseLocalOffset(2);

                        if (!symbolTable.insert(symbolInfo)) {
                            error_count++;
                            consoleError(fpError, fpLog, "Multiple declaration of variable '" + symbolInfo.getName() + "'", line_count);
                        } else {
                            string scopeId = symbolTable.getCurrentScopeTable()->getId();
                            if (scopeId == "1") {
                                if (symbolInfo.isVariable()) {
                                    globalVars += "PUSH AX\n";
                                    symbolTable.getCurrentScopeTable()->increaseVariableCount(1);
                                } else if (symbolInfo.isArray()) {
                                    for (int j = 0; j < symbolInfo.getSize(); ++j) {
                                        globalVars += "PUSH AX\n";
                                        symbolTable.getCurrentScopeTable()->increaseVariableCount(1);
                                    }
                                    symbolTable.getCurrentScopeTable()->decreaseLocalOffset((symbolInfo.getSize() - 1) * 2);
                                }
                            } else {
                                if (symbolInfo.isVariable()) {
                                    asmCode << "PUSH AX\n";
                                    symbolTable.getCurrentScopeTable()->increaseVariableCount(1);
                                } else if (symbolInfo.isArray()) {
                                    for (int j = 0; j < symbolInfo.getSize(); ++j) {
                                        asmCode << "PUSH AX\n";
                                        symbolTable.getCurrentScopeTable()->increaseVariableCount(1);
                                    }
                                    symbolTable.getCurrentScopeTable()->decreaseLocalOffset((symbolInfo.getSize() - 1) * 2);
                                }
                            }
                        }
                    }
                }

                string symbolName = $1->getName() + " " + $2->getName() + ";";
                string symbolType = "var_declaration";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "var_declaration : type_specifier declaration_list SEMICOLON", $$->getName(), line_count);
            }
 		 ;
 		 
type_specifier	: INT
        {
            string symbolName = "int";
            string symbolType = "int";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "type_specifier : INT", $$->getName(), line_count);
        }
 		| FLOAT
        {
            string symbolName = "float";
            string symbolType = "int";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "type_specifier : FLOAT", $$->getName(), line_count);
        }
 		| VOID
        {
            string symbolName = "void";
            string symbolType = "void";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "type_specifier : VOID", $$->getName(), line_count);
        }
 		;
 		
declaration_list : declaration_list COMMA ID
            {
                string symbolName = $1->getName() + "," + $3->getName();
                string symbolType = "declaration_list";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "declaration_list : declaration_list COMMA ID", $$->getName(), line_count);
            }
 		  | declaration_list COMMA ID LTHIRD CONST_INT RTHIRD
            {
                string symbolName = $1->getName() + "," + $3->getName() + "[" + $5->getName() + "]";
                string symbolType = "declaration_list";
				$$ = new SymbolInfo(symbolName, symbolType);
				consoleLog(fpLog, "declaration_list : declaration_list COMMA ID LTHIRD CONST_INT RTHIRD", $$->getName(), line_count);
            }
 		  | ID
            {
                $$ = $1;
                consoleLog(fpLog, "declaration_list : ID", $$->getName(), line_count);
            }
 		  | ID LTHIRD CONST_INT RTHIRD
            {
                string symbolName = $1->getName() + "[" + $3->getName() + "]";
                string symbolType = "declaration_list";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "declaration_list : ID LTHIRD CONST_INT RTHIRD", $$->getName(), line_count);
            }
 		  ;
 		  
statements : statement
        {
            $$ = $1;
            consoleLog(fpLog, "statements : statement", $$->getName(), line_count);
        }
	   | statements statement
        {
            string symbolName = $1->getName() + "\n" + $2->getName();
            string symbolType = "statements";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "statements : statements statement", $$->getName(), line_count);
        }
	   ;
	   
statement : var_declaration
        {
            $$ = $1;
			consoleLog(fpLog, "statement : var_declaration", $$->getName(), line_count);
        }
	  | expression_statement
        {
            $$ = $1;
			consoleLog(fpLog, "statement : expression_statement", $$->getName(), line_count);
        }
	  | { 
            int currOffset = symbolTable.getCurrentScopeTable()->getLocalOffset();
            symbolTable.enterScope();
            symbolTable.getCurrentScopeTable()->setLocalOffset(currOffset);
        } 
      compound_statement
        {   
            $$ = $2;
            consoleLog(fpLog, "statement : compound_statement", $$->getName(), line_count);
        }
	  | FOR LPAREN expression_statement {
            labels.push(newLabel());
            asmCode << labels.top() + ":\n";
      } expression_statement {
            asmCode << "CMP AX, 0\n";
            labels.push(newLabel());
            asmCode << ";FOR\n";
            asmCode << "JE " + labels.top() + "\n";
            labels.push(newLabel());
            asmCode << "JNE " + labels.top() + "\n";
            labels.push(newLabel());
            asmCode << labels.top() + ":\n";
      } expression { asmCode << "POP AX\n"; } RPAREN {
            string lbl4, lbl3, lbl2, lbl1;
            lbl4 = labels.top();
            labels.pop();
            lbl3 = labels.top();
            labels.pop();
            lbl2 = labels.top();
            labels.pop();
            lbl1 = labels.top();
            labels.pop();
            asmCode << "JMP " + lbl1 + "\n";
            asmCode << lbl3 + ":\n";
            labels.push(lbl2);
            labels.push(lbl4);
      } statement
        {
            string symbolName = "for(" + $3->getName() + $5->getName() + $7->getName() + ")" + $11->getName();
            string symbolType = "statement";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement", $$->getName(), line_count);

            asmCode << "JMP " + labels.top() + "\n";
            labels.pop();
            asmCode << labels.top() + ":\n";
            labels.pop();
        }
	  | if_statement %prec LOWER_THAN_ELSE
        {
            $$ = $1;
            consoleLog(fpLog, "statement : IF LPAREN expression RPAREN statement", $$->getName(), line_count);   
            asmCode << labels.top() + ":\n";
            labels.pop();
        } 
        | if_statement ELSE {
            string lbl = labels.top();
            labels.pop();
            asmCode << labels.top() << ":\n";
            labels.pop();
            labels.push(lbl);
        } statement {
            asmCode << labels.top() << ":\n";
            labels.pop();
        }

	  | WHILE LPAREN 
        {   
            labels.push(newLabel());
            asmCode << labels.top() + ":\n";
        } expression RPAREN {
            asmCode << "POP AX\n";
            asmCode << "CMP AX, 0\n";
            labels.push(newLabel());
            asmCode << "JE " + labels.top() + "\n";
      } statement
        {
            string symbolName = "while(" + $4->getName() + ")" + $7->getName();
            string symbolType = "statement";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "statement : WHILE LPAREN expression RPAREN statement", $$->getName(), line_count);   

            string endingLabel = labels.top();
            labels.pop();
            string startingLabel = labels.top();
            labels.pop();
            asmCode << "JMP " + startingLabel + "\n";
            asmCode << endingLabel + ":\n";
        }
	  | PRINTLN LPAREN expression RPAREN SEMICOLON
        {

            SymbolInfo* symbolInfo = symbolTable.lookUp($3->getName());
            // if (symbolInfo) {
            //     if (!symbolInfo->isVariable()) {
            //         error_count++;
            //         consoleError(fpError, fpLog, "Only a variable can be inside printf", line_count);
            //     }
            //     // asmCode << "MOV AX, [BP + " + to_string(symbolInfo->getOffset()) + "]\n";
            //     // asmCode << "MOV XX, AX\n";
            //     // asmCode << "CALL OUTPUT_X\n";
                
            // } else {
            //     error_count++;
            //     consoleError(fpError, fpLog, "Variable '" + $3->getName() + "' undeclared", line_count);
            // }
            string symbolName = "printf(" + $3->getName() + ");";
            string symbolType = "statement";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "statement : PRINTLN LPAREN ID RPAREN SEMICOLON", $$->getName(), line_count);
        
            asmCode << "POP AX\n";
            asmCode << "MOV XX, AX\n";
            asmCode << "CALL OUTPUT_X\n";
        }
	  | RETURN expression SEMICOLON
        {
            if (currentFunctionReturnType == "void") {
                error_count++;
                consoleError(fpError, fpLog, "Function type void cannot have a return statement", line_count);
                currentFunctionReturnType = "error";
            }

            string symbolName = "return " + $2->getName() + ";";
            string symbolType = "statement";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "statement : RETURN expression SEMICOLON", $$->getName(), line_count);
            asmCode << "POP DX\n";
            asmCode << "JMP label_" << currentFunctionLabel << "\n";
        }
	  ;
	  
if_statement : IF LPAREN expression RPAREN {
            asmCode << "POP AX\n";
            asmCode << "CMP AX, 0\n";
            labels.push(newLabel());
            asmCode << "JE " + labels.top() + "\n";
      } statement {
            string symbolName = "if(" + $3->getName() + ")" + $6->getName();
            string symbolType = "statement";
            $$ = new SymbolInfo(symbolName, symbolType);
            labels.push(newLabel());
            asmCode << "JMP " + labels.top() + "\n";
      };

expression_statement 	: SEMICOLON			
            {
                string symbolName = ";";
                string symbolType = "SEMICOLON";
                $$ = new SymbolInfo(symbolName, symbolType);
            }
			| expression SEMICOLON 
            {
                string symbolName = $1->getName() + ";";
                string symbolType = "expression_statement";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "expression_statement : expression SEMICOLON", $$->getName(), line_count);

                asmCode << "POP AX\n";
            }
			;
	  

expression : logic_expression
        {
            $$ = $1;
            consoleLog(fpLog, "expression : logic_expression", $$->getName(), line_count);
        }
	   | variable ASSIGNOP logic_expression
        {
            SymbolInfo *op1ref = $1, *op2ref = $3;
            string op1 = op1ref->getName(), op2 = op2ref->getName(), op1Type = op1ref->getType(), op2Type = op2ref->getType();
            if (op1Type != op2Type && !(op1Type == "float" && op2Type == "int")) {
                if (op1Type == "error" || op2Type == "error") {
                    if (op1ref->isArray()) {
                        error_count++;
                        consoleError(fpError, fpLog, "Type mismatch. Variable '" + op1 + "' is an array", line_count);
                    } else if (op2ref->isArray()) {
                        error_count++;
                        consoleError(fpError, fpLog, "Type mismatch. Variable '" + op2 + "' is an array", line_count);
                    }
                } else {
                    error_count++;
                    consoleError(fpError, fpLog, "Type mismatch of variable '" + op1 + "'", line_count);
                }
            }
            string symbolName = op1 + "=" + op2;
            string symbolType = "expression";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "expression : variable ASSIGNOP logic_expression", $$->getName(), line_count);    
            
            string varName = getArrayName($1->getName());
            SymbolInfo *symbolInfo = symbolTable.lookUp(varName);
            string varAddress;
            if (isArray($1->getName())) {
                int arrayIndex = getArraySize($1->getName());    
                varAddress = "BP + " + to_string(symbolInfo->getOffset() - arrayIndex * 2);
            } else {
                varAddress = "BP + " + to_string(symbolInfo->getOffset());
            }

            asmCode << "POP BX\n";
            asmCode << "POP AX\n";
            asmCode << "MOV [" + varAddress + "], BX\n";
            asmCode << "PUSH [" + varAddress + "]\n";
        }
	   ;
			
logic_expression : rel_expression
            {
                $$ = $1;
                consoleLog(fpLog, "logic_expression : rel_expression", $$->getName(), line_count);
            }
		 | rel_expression LOGICOP rel_expression
            {
                string op1 = $1->getName(), op2 = $3->getName(), opSym = $2->getName(), op1Type = $1->getType(), op2Type = $3->getType(), returnType = "int";
                if (op1Type != "int" || op2Type != "int") {
                    error_count++;
                    returnType = "error";
                    consoleError(fpError, fpLog, "Both operands of " + opSym + " must be int type", line_count);
                }
                string symbolName = op1 + opSym + op2;
                string symbolType = returnType;
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "logic_expression : rel_expression LOGICOP rel_expression", $$->getName(), line_count);    
                
                asmCode << "POP BX\n";
                asmCode << "POP AX\n";

                if (opSym == "&&") {
                    asmCode << "AND AX, BX\n";
                    asmCode << "PUSH AX\n";
                } else if (opSym == "||") {
                    asmCode << "OR AX, BX\n";
                    asmCode << "PUSH AX\n";
                }
            }
		 ;
			
rel_expression	: simple_expression
        {
            $$ = $1;
            consoleLog(fpLog, "rel_expression : simple_expression", $$->getName(), line_count);
        }
		| simple_expression RELOP simple_expression	
        {
            string symbolName = $1->getName() + $2->getName() + $3->getName();
            string symbolType = "int";
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "rel_expression : simple_expression RELOP simple_expression", $$->getName(), line_count);

            string opSym = $2->getName();

            asmCode << "POP BX\n";
            asmCode << "POP AX\n";
            asmCode << "CMP AX, BX\n";
            string lbl1 = newLabel(), lbl2 = newLabel();
            if (opSym == "<") {
                asmCode << "JNL " + lbl1 + "\n";
            } else if (opSym == "<=") {
                asmCode << "JNLE " + lbl1 + "\n";
            } else if (opSym == ">") {
                asmCode << "JNG " + lbl1 + "\n";
            } else if (opSym == ">=") {
                asmCode << "JNGE " + lbl1 + "\n";
            } else if (opSym == "==") {
                asmCode << "JNE " + lbl1 + "\n";
            } else if (opSym == "!=") {
                asmCode << "JE " + lbl1 + "\n";
            }

            asmCode << "PUSH 1\n";
            asmCode << "JMP " + lbl2 + "\n";
            asmCode << lbl1 + ":\n";
            asmCode << "PUSH 0\n";
            asmCode << lbl2 + ":\n";
        }
		;
				
simple_expression : term 
            {
                $$ = $1;
                consoleLog(fpLog, "simple_expression : term", $$->getName(), line_count);
            }
		  | simple_expression ADDOP term
            {
                string op1 = $1->getName(), op2 = $3->getName(), op1Type = $1->getType(), op2Type = $3->getType(), opSym = $2->getName(), returnType = "int";
                if (op1Type == "float" || op2Type == "float") returnType = "float";
                string symbolName = op1 + opSym + op2;
                string symbolType = returnType;
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "simple_expression : simple_expression ADDOP term", $$->getName(), line_count);

                asmCode << "POP BX\n";
                asmCode << "POP AX\n";
                if (opSym == "+") {
                    asmCode << "ADD AX, BX\n";
                } else if (opSym == "-") {
                    asmCode << "SUB AX, BX\n";
                }
                asmCode << "PUSH AX\n";
            }
		  ;
					
term :	unary_expression
        {
            $$ = $1;
            consoleLog(fpLog, "term : unary_expression", $$->getName(), line_count);
        }
     |  term MULOP unary_expression
        {
            string op1 = $1->getName(), op2 = $3->getName(), op1Type = $1->getType(), op2Type = $3->getType(), opSym = $2->getName(), returnType = "error";

            if (opSym == "*" || opSym == "/") {
                if (op1Type == "float" || op2Type == "float") {
                    returnType = "float";
                } else {
                    returnType = "int";
                }
                if (opSym == "/" && op2 == "0") {
                    error_count++;
                    returnType = "error";
                    consoleError(fpError, fpLog, "Division by zero not allowed", line_count);
                }
            } else if (opSym == "%") {
                if (op1Type == "int" && op2Type == "int") {
                    returnType = "int";
                    if (op2 == "0") {
                        error_count++;
                        returnType = "error";
                        consoleError(fpError, fpLog, "Modulus using zero", line_count);
                    }
                } else {
                    error_count++;
                    returnType = "error";
                    consoleError(fpError, fpLog, "Non-integer operand(s) on modulus operator", line_count);
                }
            } else {
                returnType = "Undeclared";
            }

            string symbolName = op1 + opSym + op2;
            string symbolType = returnType;
            $$ = new SymbolInfo(symbolName, symbolType);
            consoleLog(fpLog, "term : term MULOP unary_expression", $$->getName(), line_count);
        
            asmCode << "POP BX\n";
            asmCode << "POP AX\n";

            if (opSym == "*") {
                asmCode << "MUL BX\n"; 
            } else if (opSym == "/") {
                asmCode << "MOV DX, 0\n";
                asmCode << "DIV BX\n";
            } else if (opSym == "%") {
                asmCode << "MOV DX, 0\n";
                asmCode << "DIV BX\n";
                asmCode << "MOV AX, DX\n";
            }

            asmCode << "PUSH AX\n";
        }
     ;

unary_expression : ADDOP unary_expression
            {
                string symbolName = $1->getName() + $2->getName();
                string symbolType = $2->getType();
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "unary_expression : ADDOP unary_expression", $$->getName(), line_count);

                string opSym = $1->getName();
                if (opSym == "-") {
                    asmCode << "POP AX\n";
                    asmCode << "NEG AX\n";
                    asmCode << "PUSH AX\n";
                }
            }
		 | NOT unary_expression 
            {
                string symbolName = "!" + $2->getName();
                string symbolType = $2->getType();
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "unary_expression : NOT unary_expression", $$->getName(), line_count);
            
                asmCode << "POP AX\n";
                asmCode << "NOT AX\n";
                asmCode << "PUSH AX\n";
            }
		 | factor
            {
                $$ = $1;
                consoleLog(fpLog, "unary_expression : factor", $$->getName(), line_count);
            } 
		 ;
	
factor  : variable
    {
        $$ = $1;
        consoleLog(fpLog, "factor : variable", $$->getName(), line_count);
    }
	| ID LPAREN argument_list RPAREN
    {
        string returnType = "undeclared";
        SymbolInfo *symbolInfo = symbolTable.lookUp($1->getName());
        vector<string> argNames = tokenizeString($3->getName(), ',');
        vector<string> argTypes = tokenizeString($3->getType(), ',');
                
        if (symbolInfo) {
            if (symbolInfo->isFunction()) {
                returnType = symbolInfo->getType();

                vector<Parameter> parameterList = symbolInfo->getParameterList();
                if (argNames.size() > parameterList.size()) {
                    error_count++;
                    consoleError(fpError, fpLog, "Too many arguments in function call", line_count);
                } else if (argNames.size() < parameterList.size()) {
                    error_count++;
                    consoleError(fpError, fpLog, "Too few arguments in function call", line_count);
                } else {
                    for (int i = 0; i < argTypes.size(); ++i) {
                        if (argTypes[i] != parameterList[i].getType()) {
                            error_count++;
                            consoleError(fpError, fpLog, "argument of type '" + argTypes[i] + "' is incompatible with parameter of type '" + parameterList[i].getType() + "' for function '" + symbolInfo->getName() + "')", line_count);
                        }
                    }
                }
            } else {
                error_count++;
                consoleError(fpError, fpLog, "Expression preceding parentheses of apparent call must have function type", line_count);
            }
        } else {
            error_count++;
            consoleError(fpError, fpLog, "Undeclared function '" + $1->getName() + "' called", line_count);
        }
        string symbolName = $1->getName() + "(" + $3->getName() + ")";
        string symbolType = returnType;
        $$ = new SymbolInfo(symbolName, symbolType);
        consoleLog(fpLog, "factor : ID LPAREN argument_list RPAREN", $$->getName(), line_count);
        asmCode << "CALL " + $1->getName() + "\n";
        asmCode << "PUSH DX\n";
    }
	| LPAREN expression RPAREN
    {
        string symbolName = "(" + $2->getName() + ")";
        string symbolType = $2->getType();
        $$ = new SymbolInfo(symbolName, symbolType);
        consoleLog(fpLog, "factor : LPAREN expression RPAREN", $$->getName(), line_count);
    }
	| CONST_INT
    {
        $$ = $1;
        consoleLog(fpLog, "factor : CONST_INT", $$->getName(), line_count);
        asmCode << "PUSH " + $$->getName() + "\n";
    }
	| CONST_FLOAT
    {
        $$ = $1;
        consoleLog(fpLog, "factor : CONST_FLOAT", $$->getName(), line_count);
    }
	| variable INCOP
    {
        string symbolName = $1->getName() + "++";
        string symbolType = $1->getType();
        $$ = new SymbolInfo(symbolName, symbolType);
        consoleLog(fpLog, "factor : variable INCOP", $$->getName(), line_count);

        string varName = getArrayName($1->getName());
        SymbolInfo *symbolInfo = symbolTable.lookUp(varName);
        string varAddress;
        if (isArray($1->getName())) {
            int arrayIndex = getArraySize($1->getName());    
            varAddress = "BP + " + to_string(symbolInfo->getOffset() - arrayIndex * 2);
        } else {
            varAddress = "BP + " + to_string(symbolInfo->getOffset());
        }

        asmCode << "POP AX\n";
        asmCode << "PUSH AX\n";
        asmCode << "INC AX\n";
        asmCode << "MOV [" + varAddress + "], AX\n";
    }
	| variable DECOP
    {
        string symbolName = $1->getName() + "--";
        string symbolType = $1->getType();
        $$ = new SymbolInfo(symbolName, symbolType);
        consoleLog(fpLog, "factor : variable DECOP", $$->getName(), line_count);

        string varName = getArrayName($1->getName());
        SymbolInfo *symbolInfo = symbolTable.lookUp(varName);
        string varAddress;
        if (isArray($1->getName())) {
            int arrayIndex = getArraySize($1->getName());    
            varAddress = "BP + " + to_string(symbolInfo->getOffset() - arrayIndex * 2);
        } else {
            varAddress = "BP + " + to_string(symbolInfo->getOffset());
        }

        asmCode << "POP AX\n";
        asmCode << "PUSH AX\n";
        asmCode << "DEC AX\n";
        asmCode << "MOV [" + varAddress + "], AX\n";
    }
	;
	

variable : ID
        {
            SymbolInfo *symbolInfo = symbolTable.lookUp($1->getName());
            if (symbolInfo) {
                if (symbolInfo->isArray()) {
                    $$ = new SymbolInfo();
                    $$->setAsArray(symbolInfo->getName(), "error", symbolInfo->getSize());
                } else {
                    string symbolName = symbolInfo->getName();
                    string symbolType = symbolInfo->getType();
                    $$ = new SymbolInfo(symbolName, symbolType);

                    asmCode << "PUSH [BP + " + to_string(symbolInfo->getOffset()) + "]\n";
                }
            } else {
                error_count++;
                consoleError(fpError, fpLog, "Identifier '" + $1->getName() + "' is undefined", line_count);
                string symbolName = $1->getName();
                string symbolType = "error";
                $$ = new SymbolInfo(symbolName, symbolType);
            }
            consoleLog(fpLog, "variable : ID", $$->getName(), line_count);
        }
	 | ID LTHIRD expression RTHIRD
        {
            string expressionName = $1->getName();
            SymbolInfo *symbolInfo = symbolTable.lookUp(expressionName);
            if (symbolInfo) {
                string returnType = symbolInfo->getType();
                if (symbolInfo->isArray()) {
                    if ($3->getType() != "int") {
                        error_count++;
                        returnType = "error";
                        consoleError(fpError, fpLog, "Array index must have integral type", line_count);
                    }
                    string symbolName = expressionName + "[" + $3->getName() + "]";
                    string symbolType = returnType;
                    $$ = new SymbolInfo(symbolName, symbolType);
                    
                    asmCode << "POP BX\n";
                    asmCode << "SHL BX, 1\n";
                    asmCode << "MOV AX, " + to_string(symbolInfo->getOffset()) + "\n";
                    asmCode << "SUB AX, BX\n";
                    asmCode << "MOV SI, AX\n";
                    asmCode << "PUSH [BP + SI]\n";
                } else {
                    error_count++;
                    consoleError(fpError, fpLog, "Variable '" + expressionName + "' is not an array", line_count);
                    string symbolName = expressionName + "[" + $3->getName() + "]";
                    string symbolType = "error";
                    $$ = new SymbolInfo(symbolName, symbolType);    
                }
            } else {
                error_count++;
                consoleError(fpError, fpLog, "Identifier '" + expressionName + "' is undefined", line_count);
                string symbolName = expressionName + "[" + $3->getName() + "]";
                string symbolType = "error";
                $$ = new SymbolInfo(symbolName, symbolType);
            }
            consoleLog(fpLog, "variable : ID LTHIRD expression RTHIRD", $$->getName(), line_count);
        }
	 ;
	 

argument_list : arguments
			  {
                $$ = $1;
                consoleLog(fpLog, "argument_list : arguments", $$->getName(), line_count);
              }
              |
              {
                string symbolName = "";
                string symbolType = "";
                $$ = new SymbolInfo(symbolName, symbolType);
                consoleLog(fpLog, "argument_list : ", $$->getName(), line_count);
              }
			  ;
	
arguments : arguments COMMA logic_expression
            {
                $$ = new SymbolInfo($1->getName() + "," + $3->getName(), $1->getType() + "," + $3->getType());
                consoleLog(fpLog, "arguments : arguments COMMA logic_expression", $$->getName(), line_count);
            }
	        | logic_expression
            {
                $$ = $1;
                consoleLog(fpLog, "arguments : logic_expression", $$->getName(), line_count);
            }
	        ;





%%

int main(int argc, char *argv[]) {
    in = fopen(argv[1], "r");
	if (!in) {
		printf("Cannot Open Input File.\n");
		exit(1);
	}

    asmCode.open("code.asm");
    
    asmCode << ".MODEL SMALL\n.STACK 100H\n.DATA\n\nB DW 10\nXX DW ?\nXX_TMP DW ?\nCR EQU 0DH\nLF EQU 0AH\nNEG_FLAG DB 0\n\n.CODE\n\n";

    string boilerPlate = "PRINT_NEW_LINE PROC\nMOV AH, 2\nMOV DL, CR\nINT 21H\nMOV DL, LF\nINT 21H\nRET\nPRINT_NEW_LINE ENDP\n\nOUTPUT_X PROC \\
    \nCMP XX, 0\nJNL POS__\nNEG XX\nMOV DL, '-'\nMOV AH, 2\nINT 21H\nPOS__:\nMOV AX, XX\nMOV XX_TMP, AX\nMOV CX, 0\n \\
    LOOP_OUTPUT_X_STACK_PUSH: \nMOV AX, XX_TMP\nMOV DX, 0\nDIV B\nADD DX, '0'\nPUSH DX\nINC CX\nMOV XX_TMP, AX\nCMP XX_TMP, 0\n \\
    JNE LOOP_OUTPUT_X_STACK_PUSH\nLOOP_OUTPUT_X_STACK_POP:\nPOP DX\nMOV AH, 2\nINT 21H\nLOOP LOOP_OUTPUT_X_STACK_POP\nCALL PRINT_NEW_LINE\n \\
    RET\nOUTPUT_X ENDP\n\n";
    asmCode << boilerPlate << "\n";

    fpLog = fopen("log.txt", "w");
    fpError = fopen("error.txt", "w");
	yyin = in;
	yyparse();
	symbolTable.printAllScopeTables(fpLog);
	fprintf(fpLog, "Total lines: %d\nTotal errors: %d\n", line_count, error_count);
	fclose(fpLog);
	fclose(fpError);


    asmCode << "\n\nMAIN PROC\nMOV AX, @DATA\nMOV DS, AX\nMOV BP, SP\n";
    asmCode << globalVars << "\n";
    asmCode << "CALL main_func\nMOV AH, 4CH\nINT 21H\nMAIN ENDP\nEND MAIN\n";

    if (error_count)
        asmCode.open("code.asm");

    asmCode.close();

    ifstream f("code.asm"); //taking file as inputstream
    string finalAsm;
    if(f) {
      ostringstream ss;
      ss << f.rdbuf(); // reading data
      finalAsm = ss.str();
    }
    
    finalAsm = optimizeCode(finalAsm);
    ofstream optimizedAsmCodeFile;
    optimizedAsmCodeFile.open("optimized_code.asm");

    optimizedAsmCodeFile << finalAsm;
    optimizedAsmCodeFile.close();
	return 0;
}