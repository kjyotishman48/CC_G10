%option noyywrap

%{
#include "parser.hh"
#include <string>

extern int yyerror(std::string msg);
%}

%%

"+"       { return TPLUS; }
"-"       { return TDASH; }
"*"       { return TSTAR; }
"/"       { return TSLASH; }

";"       { return TSCOL; }
"("       { return TLPAREN; }
")"       { return TRPAREN; }
"{"       { return TLCURLY; }
"}"       { return TRCURLY; }
"="       { return TEQUAL; }
"?"       { return TQUES; }
":"       { return TCAL; }
"if"      { return TIF; }
"else"    { return TELSE; }
"dbg"     { return TDBG; }
"let"     { return TLET; }
[0-9]+    { yylval.lexeme = std::string(yytext); return TINT_LIT; }
[a-zA-Z]+ { yylval.lexeme = std::string(yytext); return TIDENT; }
[ \t\n]   { /* skip */ }
.         { yyerror("unknown char"); }

%%

std::string token_to_string(int token, const char *lexeme) {
    std::string s;
    std::string t = "EQUAL";
    std::string t1 = "CUR";
    std::string t2 = "LY";
    std::string t4 = "PAREN";
    std::string t3 = t1+t2;
    switch (token) {
        case TPLUS: s = "TPLUS"; break;
        case TDASH: s = "TDASH"; break;
        case TSTAR: s = "TSTAR"; break;
        case TSLASH: s = "TSLASH"; break;
        case TSCOL: s = "TSCOL"; break;
        case TLPAREN: s = "TL"+t4; break;
        case TRPAREN: s = "TR"+t4; break;
        case TLCURLY: s = "TL"+t3; break;
        case TRCURLY: s = "TR"+t3; break;
        case TEQUAL: s = "T"+t; break;
        
        case TIF: s = "TIF"; break;
        case TELSE: s = "TELSE"; break;
        case TDBG: s = "TDBG"; break;
        case TLET: s = "TLET"; break;
        case TQUES: s = "TQUES"; break;
        case TCAL: s = "TCAL"; break;
        case TINT_LIT: s = "TINT_LIT"; s.append("  ").append(lexeme); break;
        case TIDENT: s = "TIDENT"; s.append("  ").append(lexeme); break;
    }

    return s;
}