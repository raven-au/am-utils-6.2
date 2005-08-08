/*
 * Copyright (c) 1997-2005 Erez Zadok
 * Copyright (c) 2005 Daniel P. Ottavio
 * Copyright (c) 1990 Jan-Simon Pendry
 * Copyright (c) 1990 Imperial College of Science, Technology & Medicine
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Jan-Simon Pendry at Imperial College, London.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgment:
 *      This product includes software developed by the University of
 *      California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * File: am-utils/amd/sun_map_tok.l
 *
 */

%{

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif /* HAVE_CONFIG_H */
#include <am_defs.h>
#include <amd.h>
#include <sun_map_parse.h>

/*
 * We need to configure lex to parse from a string
 * instead of a file. Each version of lex has it's
 * own way of doing this (sigh).
 */
#ifdef FLEX_SCANNER

/* assign the buffer to parse */
void sun_map_tok_setbuff(const char* buff);

/* buffer that contains the string to parse */
const char *sun_map_tok_buff = NULL;

/* current position of the buffer */
const char *sun_map_tok_pos = NULL;

/* size of the buffer */
const char *sun_map_tok_end = NULL;

/* copies the current position + maxsize into buff */
int sun_map_yyinput(char *buff, int maxsize);

# undef YY_INPUT
# define YY_INPUT(buff,result,maxsize) (result = sun_map_yyinput(buff,maxsize))
#else /* not FLEX_SCANNER */
# warning "Currently flex is the only supported version of lex."

#endif /* not FLEX_SCANNER */

int sun_map_line = 1;
int sun_map_tokpos = 1;

%}

WORD_REX       ([A-Za-z0-9_/&".""$"=""-"]+)|"*"
COMMENT_REX    ^#.*\n
WSPACE_REX     [ \t]*
NEWLINE_REX    [ \t]*\n
CONTINUE_REX   "\\"\n

/*
 * This option causes Solaris lex to fail.  Use flex.  See BUGS file
 * no need to use yyunput()
 */
%option nounput

/* allocate more output slots so lex scanners don't run out of mem */
%o 1024

%%

{WORD_REX}      {
                  sun_map_tokpos += yyleng;
                  strncpy(yylval.strval,yytext,sizeof(yylval.strval));
                  yylval.strval[sizeof(yylval.strval) - 1] = '\0';
                  return WORD;
                }

{WSPACE_REX}    {
                  sun_map_tokpos += yyleng;
                  return WSPACE;
                }

{NEWLINE_REX}   {
                  sun_map_tokpos = 0;
                  sun_map_line++;
                  return NEWLINE;
                }

{CONTINUE_REX}  {
                  sun_map_tokpos = 0;
                  sun_map_line++;
                }

{COMMENT_REX}   {
                  sun_map_line++;
                }

.               {
                  return yytext[0];
                }

%%


int
sun_map_error(const char* s)
{
  plog(XLOG_ERROR,"sun2amd : parsing error : line %d, column %d\n",
       sun_map_line,sun_map_tokpos);
  return 1;
}


int
sun_map_wrap(void)
{
  return 1;
}


#ifdef FLEX_SCANNER
void
sun_map_tok_setbuff(const char* buff)
{
  sun_map_tok_end = buff + strlen(buff);
  sun_map_tok_pos = buff;
  sun_map_tok_buff = buff;
}


int
sun_map_yyinput(char *buff, int maxsize)
{
  int size = MIN(maxsize, (sun_map_tok_end - sun_map_tok_pos));
  if (size > 0) {
    memcpy(buff,sun_map_tok_pos,size);
    sun_map_tok_pos += size;
  }

  return size;
}
#endif /* FLEX_SCANNER */
