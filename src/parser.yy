%define api.value.type { ParserValue }

%code requires {
#include <iostream>
#include <vector>
#include <string>

#include "parser_util.hh"
#include "symbol.hh"

}

%code {

#include <cstdlib>

extern int yylex();
extern int yyparse();

extern NodeStmts* final_values;

SymbolTable symbol_table;

int yyerror(std::string msg);

}

%token TPLUS TDASH TSTAR TSLASH
%token <lexeme> TINT_LIT TIDENT
%token INT TLET TDBG TIF TELSE
%token TSCOL TLCURLY TRCURLY TLPAREN TRPAREN TEQUAL TQUES TCAL 


%type <node> Expr Stmt
%type <stmts> Program StmtList

%left TPLUS TDASH TQUES
%left TSTAR TSLASH 

%%

Program :                
        { final_values = nullptr; }
        | StmtList
        { final_values = $1; }
	    ;

StmtList : Stmt                
         { $$ = new NodeStmts(); $$->push_back($1); }
	     | StmtList Stmt 
         { $$->push_back($2); }
	     ;

Stmt : TLET TIDENT TEQUAL Expr TSCOL
     {
        
            symbol_table.insert($2);

            $$ = new NodeAssn($2, $4);
        
     }
     |TIDENT TEQUAL Expr TSCOL
     {
        if(symbol_table.contains($1)) {
            $$ = new NodeUpdt($1, $3);
        } else {
            // variable not declared error
            yyerror("variable not declared.\n");
        }
     }
     | TDBG Expr TSCOL
     { 
        $$ = new NodeDebug($2);
     }
     | TIF Expr TLCURLY Program TRCURLY TELSE TLCURLY Program TRCURLY
     {
        $$ = new NodeConditional($2,$4,$8);
        //$$ = new NodeCond();
     }
     ;


Expr : TINT_LIT               
     { $$ = new NodeInt(stoi($1)); }
     | TIDENT
     { 
        if(symbol_table.contains($1))
            $$ = new NodeIdent($1); 
        else
            yyerror("using undeclared variable.\n");
     }
     | Expr TPLUS Expr
     { $$ = new NodeBinOp(NodeBinOp::PLUS, $1, $3); }
     | Expr TDASH Expr
     { $$ = new NodeBinOp(NodeBinOp::MINUS, $1, $3); }
     | Expr TSTAR Expr
     { $$ = new NodeBinOp(NodeBinOp::MULT, $1, $3); }
     | Expr TSLASH Expr
     { $$ = new NodeBinOp(NodeBinOp::DIV, $1, $3); }
     | Expr TQUES Expr TCAL Expr
     { $$ = new NodeBinOp(NodeBinOp::DIV, $1, $5); } 
     | TLPAREN Expr TRPAREN { $$ = $2; }
     ;

%%

int yyerror(std::string msg) {
    std::cerr << "Error! " << msg << std::endl;
    exit(1);
}