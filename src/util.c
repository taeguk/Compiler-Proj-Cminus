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
void printToken( TokenType token, const char* token_string )
{
  switch (token)
    {
    case ELSE:  case IF:
    case INT:   case RETURN:
    case VOID:  case WHILE:
        {
          int i;
          int diff = 'A' - 'a';
          for (i = 0; token_string[i]; ++i)
            fprintf(listing, "%c",token_string[i]+diff);
          if (strlen(token_string) < 6)
            fprintf(listing, "\t");
          fprintf(listing, "\t%s\n", token_string);
        } break;

    case LE_OP: case GE_OP:
    case EQ_OP: case NE_OP:
    case LT_OP: case GT_OP:
    case SEMI: case RBRACE:
    case LBRACE: case ',':
    case ASSIGN:
    case LPAREN: case RPAREN:
    case '[': case ']':
    case '-': case '+':
    case '*': case '/':
      fprintf(listing,
              "%s\t\t%s\n", token_string, token_string);
      break;
    case NUM:
      fprintf(listing,
              "NUM\t\t%s\n",token_string);
      break;
    case ID:
      fprintf(listing,
              "ID\t\t%s\n",token_string);
      break;
    case ERROR:
      fprintf(listing,
              "ERROR\t\t%s\n",token_string);
      break;
    case ENDFILE:
      fprintf(listing, "EOF\n");
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
    }
}

void print_op_token(TokenType token)
{
  switch (token)
    {
    case LE_OP:
      printf("<=\n");
      break;
    case LT_OP:
      printf("<\n");
      break;
    case GE_OP:
      printf(">=\n");
      break;
    case GT_OP:
      printf(">\n");
      break;
    case EQ_OP:
      printf("==\n");
      break;
    case NE_OP:
      printf("!=\n");
      break;
    case '+':
      printf("+\n");
      break;
    case '-':
      printf("-\n");
      break;
    case '*':
      printf("*\n");
      break;
    case '/':
      printf("/\n");
      break;
    default:
      printf("Not assigned token\n"
             "Please Note print_op_token function in util.c"
             );
      break;
    }
}
/* Function new_stmt_node creates a new statement
 * node for syntax tree construction
 */
TreeNode * new_stmt_node(StmtKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else
    {
      for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
      t->sibling = NULL;
      t->nodekind = StmtK;
      t->kind.stmt = kind;
      t->lineno = lineno;
    }
  return t;
}

/* Function new_exp_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_exp_node(ExpKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n",lineno);
  else
    {
      for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
      t->sibling = NULL;
      t->nodekind = ExpK;
      t->kind.exp = kind;
      t->lineno = lineno;
      t->type = Void;
    }
  return t;
}

/* Function new_decl_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_decl_node(DeclKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n",lineno);
  else
    {
      for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
      t->sibling = NULL;
      t->nodekind = DeclK;
      t->kind.decl = kind;
      t->lineno = lineno;
    }
  return t;
}

/* Function new_param_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_param_node(ParamKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n",lineno);
  else
    {
      for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
      t->sibling = NULL;
      t->nodekind = ParamK;
      t->kind.param = kind;
      t->lineno = lineno;
    }
  return t;
}

/* Function new_type_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_type_node(TypeKind kind)
{
  TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  int i;
  if (t==NULL)
    fprintf(listing, "Out of memory error at line %d\n",lineno);
  else
    {
      for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
      t->sibling = NULL;
      t->nodekind = TypeK;
      t->kind.param = kind;
      t->lineno = lineno;
    }
  return t;
}

/* Function copy_string allocates and makes a new
 * copy of an existing string
 */
char * copy_string(char * s)
{
  int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else
    strncpy(t, s, n);
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
{
  int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
/* TODO: Need to edit for each kind */
void printTree( TreeNode * tree )
{
  int i;
  INDENT;
  while (tree != NULL)
    {
      printSpaces();
      switch (tree->nodekind)
        {
        case StmtK:
          switch (tree->kind.stmt)
            {
            case IfK:
              fprintf(listing, "If\n");
              break;
            case IterK:
              fprintf(listing, "While\n");
              break;
            case RetK:
              fprintf(listing, "Return\n");
              break;
            case CompK:
              fprintf(listing, "Compound Statement\n");
              break;
            default:
              fprintf(listing,"Unknown ExpNode kind\n");
              break;
            }
          break; /* End of nodekind == StmtK */

        case ExpK:
          switch (tree->kind.exp)
            {
            case OpK:
              fprintf(listing, "Op: ");
              print_op_token(tree->attr.op);
              break;
            case ConstK:
              fprintf(listing, "Const: %d\n",tree->attr.val);
              break;
            case IdK:
              fprintf(listing, "Id: %s\n",tree->attr.name);
              break;
            case ArrIdK:
              fprintf(listing, "Array Id\n");
              break;
            case CallK:
              fprintf(listing, "CallK\n");
              break;
            case AssignK:
              fprintf(listing, "Assign to: %s\n", tree->attr.name);
              break;
            default:
              fprintf(listing,"Unknown ExpNode kind\n");
              break;
            }
          break; /* End of nodekind == ExpK */

        case DeclK:
          switch (tree->kind.decl)
            {
              case FuncK:
                fprintf(listing, "Function Declaration\n");
                break;
              case VarK:
                fprintf(listing, "Variable Declaration\n");
                break;
              case ArrK:
                fprintf(listing, "Variable Array Declaration\n");
                break;
              default:
                fprintf(listing, "Unknown Declaration Node Kind\n");
                break;
            }
          break; /* end of nodekind == DeclK */

        case ParamK:
          fprintf(listing, "Parameter\n");
          switch (tree->kind.param)
            {
            case ParamVarK:
//              fprintf(listing, "ParamVarK\n");
              break;
            case ParamArrK:
              fprintf(listing, "ParamArrK\n");
              break;
            default:
              fprintf(listing, "Unknown Parameter Node Kind\n");
              break;
            }
          break;

        case TypeK:
          switch (tree->kind.type)
            {
              case TypeNameK:
                fprintf(listing, "Type: ");
                switch (tree->attr.type)
                  {
                  case INT:
                    fprintf(listing, "int\n");
                    break;
                  case VOID:
                    fprintf(listing, "void\n");
                    break;
                  }
                break;
              default:
                fprintf(listing, "Unknown Type Node Kind\n");
                break;
            }
          break;
        default:
          fprintf(listing,"Unknown Node kind\n");
          break;
        }
      for (i = 0; i < MAXCHILDREN; i++)
        printTree(tree->child[i]);
      tree = tree->sibling;
    }
  UNINDENT;
}
