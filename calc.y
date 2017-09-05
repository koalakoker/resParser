/* calculator with AST */
%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "calc.h"
    #include "keywordcode.h"

    int yylex(void);

    char retVal[255];
    keyWordCode_t kwc;
    char* symbol;
    int newAssignVar;
    int newAssignFunc;

    struct symlist *symListRet;
%}

%union 
{
    struct ast *a;
    double d;
    int kwc;
    struct symbol *s;   /* which symbol */
    struct symlist *sl;
    int fn;     /* which function */
}

/* declare tokens */ 
%token <d> NUMBER
%token <s> NAME
%token <fn> FUNC
%token EOL
%token <kwc> KEYWORD

%token IF THEN ELSE WHILE DO LET

%nonassoc <fn> CMP
%right '='
%left '+' '-'
%left '*' '/'
%nonassoc '|' UMINUS
%nonassoc KEYWORD

%type <a> exp stmt list explist
%type <sl> symlist

%start calclist

%%

stmt: IF exp THEN list           { $$ = newflow('I', $2, $4, NULL); }
   | IF exp THEN list ELSE list  { $$ = newflow('I', $2, $4, $6); }
   | WHILE exp DO list           { $$ = newflow('W', $2, $4, NULL); }
   | exp
;

list: /* nothing */ { $$ = NULL; }
   | stmt ';' list { if ($3 == NULL)
                  $$ = $1;
                      else
      $$ = newast('L', $1, $3);
                    }
   ;

exp: exp CMP exp          { $$ = newcmp($2, $1, $3); }
   | exp '+' exp          { $$ = newast('+', $1,$3); }
   | exp '-' exp          { $$ = newast('-', $1,$3);}
   | exp '*' exp          { $$ = newast('*', $1,$3); }
   | exp '/' exp          { $$ = newast('/', $1,$3); }
   | '|' exp              { $$ = newast('|', $2, NULL); }
   | '(' exp ')'          { $$ = $2; }
   | '-' exp %prec UMINUS { $$ = newast('M', $2, NULL); }
   | NUMBER               { $$ = newnum($1); }
   | FUNC '(' explist ')' { $$ = newfunc($1, $3); }
   | NAME                 { $$ = newref($1); }
   | NAME '=' exp         { $$ = newasgn($1, $3); newAssignVar = 1; symbol = $1->name; }
   | NAME '(' explist ')' { $$ = newcall($1, $3); }
;

explist: exp
 | exp ',' explist  { $$ = newast('L', $1, $3); }
;
symlist: NAME       { $$ = newsymlist($1, NULL); }
 | NAME ',' symlist { $$ = newsymlist($1, $3); }
;

calclist: /* nothing */
  | calclist stmt EOL {
    if(debug) dumpast($2, 0);
     sprintf(retVal, "%g", eval($2));
     treefree($2);
    }
  | calclist LET NAME '(' symlist ')' '=' list EOL {
                       dodef($3, $5, $8);
                       sprintf(retVal, "Defined %s", $3->name);
                       newAssignFunc = 1; symbol = $3->name;
                       symListRet = $5;
                       }
  | calclist KEYWORD NAME EOL { kwc = $2; symbol = $3->name; yyerror("Keyword + Name"); }
  | calclist KEYWORD EOL { kwc = $2; yyerror("Keyword"); }
  | calclist error EOL { yyerrok; }
 ;
%%
