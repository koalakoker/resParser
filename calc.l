%option noyywrap nodefault yylineno nounput noinput
%{
    #include "calc.h"
    #include "calc.tab.h"
    #include "keywordcode.h"

    void yyparse(void);
    char inpVal[256];
%}

/* float exponent */

EXP ([Ee][-+]?[0-9]+)
%%
"usage" { yylval.kwc = key_Usage; return KEYWORD;}
"clearhistory" { yylval.kwc = key_ClearHistory; return KEYWORD;}
"clearraw" { yylval.kwc = key_ClearRaw; return KEYWORD;}
"e12" { yylval.kwc = key_E12; return KEYWORD;}
"e24" { yylval.kwc = key_E24; return KEYWORD;}
"clear"|"del"|"delete" { yylval.kwc = key_Clear; return KEYWORD;}
"list"|"ls" { yylval.kwc = key_List; return KEYWORD;}
"draw"|"plot" { yylval.kwc = key_Plot; return KEYWORD;}
"+" |
"-" |
"*" |
"/" |
"=" |
"|" |
"," |
";" |
"(" |
")"     { return yytext[0]; }

 /* comparison ops */
">"     { yylval.fn = 1; return CMP; }
"<"     { yylval.fn = 2; return CMP; }
"<>"    { yylval.fn = 3; return CMP; }
"=="    { yylval.fn = 4; return CMP; }
">="    { yylval.fn = 5; return CMP; }
"<="    { yylval.fn = 6; return CMP; }

 /* keywords */

"if"    { return IF; }
"then"  { return THEN; }
"else"  { return ELSE; }
"while" { return WHILE; }
"do"    { return DO; }
"let"   { return LET;}

 /* built in functions */
"sqrt"  { yylval.fn = B_sqrt; return FUNC; }
"exp"   { yylval.fn = B_exp; return FUNC; }
"log"   { yylval.fn = B_log; return FUNC; }
"print" { yylval.fn = B_print; return FUNC; }

 /* debug hack */
"debug"[0-9]+ { debug = atoi(&yytext[5]); printf("debug set to %d\n", debug); }

 /* names */
[a-zA-Z][a-zA-Z0-9]*  { yylval.s = lookup(yytext); return NAME; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.d = atof(yytext); return NUMBER; }

"//".*  
[ \t]   /* ignore white space */ 
\\\n    printf("c> "); /* ignore line continuation */
"\n"    { return EOL; }

.   { yyerror("Mystery character %c\n", *yytext); }
%%

void yyinit(void)
{
    YY_BUFFER_STATE bp;
    bp = yy_scan_string(inpVal);
    yy_switch_to_buffer(bp);
    yyparse();
    yy_delete_buffer(bp);
}
