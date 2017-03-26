/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for C-                         */
/* Compiler Construction: Principles and Practice   */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "scan.h"
/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}

"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}

"<"             {return LT;}
"<="            {return LTEQ;}
">"             {return GT;}
">="            {return GTEQ;}
"=="            {return EQ;}
"!="            {return NOTEQ;}

"="             {return ASSIGN;}
";"             {return SEMI;}
","             {return COMMA;}

"("             {return LPAREN;}
")"             {return RPAREN;}
"{"             {return LBRACE;}
"}"             {return RBRACE;}
"["             {return LBRACKET;}
"]"             {return RBRACKET;}

{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
"/*"            { char c;
                  int status = 0; // 0 : normal, 1 : wait end signal.
                  do
                    {
                      c = input();
                      switch (c)
                        {
                        case '*':
                          status = 1;
                          break;
                        case '/':
                          if (status == 1)
                            return COMMENT;
                          break;
                        case '\n':
                          ++lineno;
                          status = 0;
                          break;
                        default:
                          /* skip */
                          status = 0;
                        }
                    }
                  while (c != EOF);
                  return COMMENT_ERROR;
                     /* 


                      while ((c = input()) != '*' && c != EOF)
                      if (c == '*')
                        {
                          while ((c = input()) == '*')
                            ;
                          if (c == '/')
                            break;
                        }
                      if (c == EOF)
                        {
                          //error("EOF in comment");
                          break;
                        }
                    }
                    */
                }
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

