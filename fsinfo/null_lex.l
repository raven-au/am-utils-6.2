%{
#include "null_gram.h"
#define YY_NO_INPUT

void yyerror(const char *fmt, ...) {}
int yywrap(void) { return 0; }
%}
%option nounput
%%
