/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */

// need to be refactored.
const char * TokenTypeStringTable[] = 
{
  [ENDFILE] = "ENDFILE", 
  [ERROR] = "ERROR",
  /* reserved words */
  [IF] = "IF", [ELSE] = "ELSE", [INT] = "INT", [RETURN] = "RETURN", [VOID] = "VOID", [WHILE] = "WHILE",
  /* multicharacter tokens */
  [ID] = "ID", [NUM] = "NUM",
  /* special symbols */
  [PLUS] = "PLUS", [MINUS] = "MINUS", [TIMES] = "TIMES", [OVER] = "OVER",
  [LT] = "LT", [LTEQ] = "LTEQ", [GT] = "GT", [GTEQ] = "GTEQ", [EQ] = "EQ", [NOTEQ] = "NOTEQ",
  [ASSIGN] = "ASSIGN",
  [SEMI] = "SEMI", [COMMA] = "COMMA",
  [LPAREN] = "LPAREN", [RPAREN] = "RPAREN", [LBRACE] = "LBRACE", [RBRACE] = "RBRACE", [LBRACKET] = "LBRACKET", [RBRACKET] = "RBRACKET",
  [COMMENT] = "COMMENT"
};

void printToken( TokenType token, const char* tokenString )
{ 
  if (token == COMMENT)
    return;

  fprintf(listing,"\t%d\t\t",lineno);
  switch (token)
  {
    case ERROR: fprintf(listing, "%-20s\t%-20s\n", "ERROR", tokenString); break;
    case ENDFILE: fprintf(listing, "EOF\n"); break;

    case COMMENT_ERROR: fprintf(listing, "%-20s\t%-20s\n", "ERROR", "Comment Error"); break;

    default:
      fprintf(listing,"%-20s\t%-20s\n", TokenTypeStringTable[token], tokenString);
/*
    case IF: fprintf(listing,"IF\t%s\n", tokenString); break;
    case ELSE: fprintf(listing,"ELSE\t%s\n", tokenString); break;
    case INT: fprintf(listing,"INT\t%s\n", tokenString); break;
    case RETURN: fprintf(listing,"RETURN\t%s\n", tokenString); break;
    case VOID: fprintf(listing,"VOID\t%s\n", tokenString); break;
    case WHILE: fprintf(listing,"WHILE\t%s\n", tokenString); break;

    case NUM: fprintf(listing, "NUM\t%s\n",tokenString); break;
    case ID: fprintf(listing, "ID\t%s\n",tokenString); break;

    default: fprintf(listing,"%s\t%s\n", tokenString, tokenString);*/
  }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}

/* Function newDeclNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newDeclNode(DeclKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = DeclK;
    t->kind.decl = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newTypeNode creates a new expression 
 * node for syntax tree construction
 */
TreeNode * newTypeNode(TypeKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = TypeK;
    t->kind.type = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

static void printOp(TokenType op)
{ switch (op)
    { 
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case TIMES: fprintf(listing,"*\n"); break;
    case OVER: fprintf(listing,"/\n"); break;
    case LT: fprintf(listing,"<\n"); break;
    case LTEQ: fprintf(listing,"<=\n"); break;
    case GT: fprintf(listing,">\n"); break;
    case GTEQ: fprintf(listing,">=\n"); break;
    case EQ: fprintf(listing,"==\n"); break;
    case NOTEQ: fprintf(listing,"!=\n"); break;
    default: /* should never happen */
      fprintf(listing,"Unknown op: %d\n", op);
    }
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{ int i;
  INDENT;
  while (tree != NULL) {
    printSpaces();
    if (tree->nodekind==StmtK)
    { switch (tree->kind.stmt) {
        case CompK:
          fprintf(listing,"Compound Statement\n");
          break;
        case SelectK:
          fprintf(listing,"Select Statement\n");
          break;
        case IterK:
          fprintf(listing,"Iteration Statement\n");
          break;
        case RetK:
          fprintf(listing,"Return Statement\n");
          break;
        default:
          fprintf(listing,"Unknown StmtNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    { switch (tree->kind.exp) {
        case AssignK:
          fprintf(listing,"Assign: \n");
          break;
        case OpK:
          fprintf(listing,"Op: ");
          printOp(tree->attr.op);
          break;
        case ConstK:
          fprintf(listing,"Constant: %d\n", tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n", tree->attr.name);
          break;
        case IdArrK:
          fprintf(listing,"Id Of Array: %s\n", tree->attr.name);
          break;
        case CallK:
          fprintf(listing,"Call Function: %s\n", tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==DeclK)
    { switch (tree->kind.decl) {
        case FuncK:
          fprintf(listing,"Function Declaration: %s\n", tree->attr.name);
          break;
        case VarK:
          fprintf(listing,"Variable Declaration: %s\n", tree->attr.name);
          break;
        case VarArrK:
          if (tree->attr.arrAttr.len != -1)
            fprintf(listing, "Array Declaration: name = %s, size = %d\n", 
                  tree->attr.arrAttr.name, tree->attr.arrAttr.len);
          else
            fprintf(listing, "Array Declaration In Parameter: name = %s\n", tree->attr.arrAttr.name);
          break;
        default:
          fprintf(listing,"Unknown DeclNode kind\n");
          break;
      }
    }
    else if (tree->nodekind==TypeK)
    { 
      switch (tree->attr.typeSpec)
        {
        case INT:
          fprintf(listing, "Type: INT\n");
          break;
        case VOID:
          fprintf(listing, "Type: VOID\n");
          break;
        }
    }
    else fprintf(listing,"Unknown node kind\n");
    for (i=0;i<MAXCHILDREN;i++)
         printTree(tree->child[i]);
    tree = tree->sibling;
  }
  UNINDENT;
}
