/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%option noyywrap
%{
#include "../globals.h"
#include "../util.h"
#include "../scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

/*
  arithmetic ++ --> "+" "+"
             -- --> "-" "-"
  bitwise << --> "<", "<"
  assignment   "="|"+="|"-="|"*="|"/="|"%="|"<<="|">>="|"&="|"^="|"|="
  may be not essential
 */
%}
signed      "+"|"-"
digit       [0-9]
exp         e{signed}?{digit}+
int         {signed}?{digit}+{exp}?
float       {signed}?{digit}+"."{digit}*{exp}?f?
letter      [a-zA-Z_]
identifier  {letter}[a-zA-Z_]*
newline     \n
whitespace  [ \t]+

keyword1    register|signed|static|unsigned|volatile|auto|const|extern
keyword2    struct|union|enum
keyword3    break|case|switch|while|continue|default|for|goto|if|do|else
keyword4    char|double|float|int|long|short|void
keyword5    typdef|sizeof|return
keyword     {keyword1}|{keyword2}|{keyword3}|{keyword4}|{keyword5}

logical     ">"|"<"|">="|"<="|"&&"|"||"|"!"
arithmetic  "+"|"-"|"*"|"/"|"%"
bitwise     "&"|"|"|"^"|"~"
assignment  "="
operator    {bitwise}|{logical}|{arithmetic}|{assignment}
symbol      ";"|","|"?"|":"

paren       "("|")"|"["|"]"|"{"|"}"
%%
{paren}         {return PAREN;}
{operator}      {return OPERATOR;}
{keyword}       {return KEYWORD;}

{symbol}        {return SYMBOL;}
{int}|{float}   {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}

"/*"            {
                  register char c;
                  for ( ; ; )
                  {
                    while ( (c = input()) != '*' && c != EOF)
                      {
                        if (c == '\n') lineno++;
                      }
                    if (c == '*')
                      {
                        while ( (c = input()) == '*');
                        if (c == '/') break;
                      }
                    if (c == EOF) break;
                  }
                }
"//"[^\n]       {/* eat up one-line comments */}

.               {return ERROR;}
%%

TokenType getToken(void)
{
  static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
    {
      firstTime = FALSE;
      lineno++;
      yyin = source;
      yyout = listing;
    }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan)
    {
      fprintf(listing,"\t%d\t\t",lineno);
      printToken(currentToken,tokenString);
    }
  return currentToken;
}

