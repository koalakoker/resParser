%option noyywrap nodefault yylineno nounput noinput
%{
    # include "calc.h"
    # include "calc.tab.h"
    void yyparse(void);
    char inpVal[256];
%}

/* float exponent */

EXP ([Ee][-+]?[0-9]+)
%%
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
