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
[A-z]+ {return VARSIDENT;}
[A-z]+"(" {return FUNCIDENT;}
" " {return SEPARATOR;}
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")" { return yytext[0]; }
[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? { yylval.d = atof(yytext); return NUMBER; }
\n { return EOL; }
"//".* 
[ \t] { /* Ignore */ } 
. { yyerror("Mystery character %c\n", *yytext); }
%%

void yyinit(void)
{
    YY_BUFFER_STATE bp;
    bp = yy_scan_string(inpVal);
    yy_switch_to_buffer(bp);
    yyparse();
    yy_delete_buffer(bp);
}
