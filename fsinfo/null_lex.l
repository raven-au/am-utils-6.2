%{
#include "null_gram.h"

void yyerror(const char *fmt, ...) {}
int yywrap(void) { return 0; }
%}
%%
