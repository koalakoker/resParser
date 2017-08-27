/* calculator with AST */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "calc.h"
    #include "keywordcode.h"
    int yylex(void);
    char retVal[255];
    keyWordCode_t kwc;
%}

%union 
{
    struct ast *a;
    double d;
    int kwc;
}

/* declare tokens */ 
%token <d> NUMBER 
%token EOL
%token <kwc> KEYWORD

%type <a> exp factor term

%%
calclist: /* nothing */ 
  | calclist exp EOL { 
    sprintf(retVal, "%g", eval($2));
    treefree($2);
  }
  | calclist EOL { } /* blank line or a comment */
  | calclist KEYWORD EOL { kwc = $2 };
  ;

exp: factor
  | exp '+' factor { $$ = newast('+', $1,$3); }
  | exp '-' factor { $$ = newast('-', $1,$3);} 
  ;

factor: term
  | factor '*' term { $$ = newast('*', $1,$3); } 
  | factor '/' term { $$ = newast('/', $1,$3); } 
  ;

term: NUMBER { $$ = newnum($1); }
  | '|' term { $$ = newast('|', $2, NULL); } 
  | '(' exp ')' { $$ = $2; }
  | '-' term { $$ = newast('M', $2, NULL); } ;
%%
