/****************************************************/
/* File: cm.l                                       */
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
char token_string[MAXTOKENLEN+1];
%}
letter      [a-zA-Z]
identifier  {letter}+
digit       [0-9]

newline     \n
whitespace  [ \t]+
%%
"else"      { return ELSE; }
"if"        { return IF; }
"int"       { return INT; }
"return"    { return RETURN; }
"void"      { return VOID; }
"while"     { return WHILE; }

"<="        { return LE_OP; }
">="        { return GE_OP; }
"=="        { return EQ_OP; }
"!="        { return NE_OP; }
";"         { return SEMI; }
"}"         { return RBRACE; }
"{"         { return LBRACE; }
","         { return ','; }
"="         { return ASSIGN; }
"("         { return LPAREN; }
")"         { return RPAREN; }
"["         { return '['; }
"]"         { return ']'; }
"-"         { return '-'; }
"+"         { return '+'; }
"*"         { return '*'; }
"/"         { return '/'; }
"<"         { return LT_OP; }
">"         { return GT_OP; }


{identifier}    {return ID;}
{digit}+        {return NUM;}
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
  strncpy(token_string,yytext,MAXTOKENLEN);
  if (TraceScan)
    {
      fprintf(listing,"\t%d\t\t",lineno);
      printToken(currentToken,token_string);
    }
  return currentToken;
}

