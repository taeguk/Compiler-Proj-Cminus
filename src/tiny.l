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
%}
keywords    else|if|int|return|void|while
arithmetic  "+"|"-"|"*"|"/"
logical     "<"|"<="|">"|">="|"=="|"!="
paren       "("|")"|"["|"]"|"{"|"}"
special_sym {arithmetic}|{logical}|{paren}|";"|","|"="

letter      [a-zA-Z]
identifier  {letter}+
digit       [0-9]

newline     \n
whitespace  [ \t]+
%%
{keywords}      {return KEYWORD;}
{special_sym}   {return SYMBOL;}
{identifier}    {return ID;}
{digit}         {return NUM;}
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

