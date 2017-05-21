/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the C- compiler                              */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Modified by Kwanghee Choi                        */
/****************************************************/

#include "globals.h"
#include "util.h"

static const char * const nodeName[] = {
    "ErrorK",
    "VariableDeclarationK",
    "ArrayDeclarationK",
    "FunctionDeclarationK",
    "VariableParameterK",
    "ArrayParameterK",

    "CompoundStatementK",

    "ExpressionStatementK",
    "SelectionStatementK",
    "IterationStatementK",
    "ReturnStatementK",
    "AssignExpressionK",

    "ComparisonExpressionK",
    "AdditiveExpressionK",
    "MultiplicativeExpressionK",

    "VariableK",
    "ArrayK",
    "CallK",

    "ConstantK",
    "TokenTypeK"
};

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void
printToken(TokenType token, const char* tokenString)
{
  switch (token)
    {
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case ERROR:   fprintf(listing,
                          "ERROR\t\t\t%s\n",
                          tokenString); break;

    case ID:      fprintf(listing,
                          "ID\t\t\t%s\n",
                          tokenString); break;
    case NUM:     fprintf(listing,
                          "NUM\t\t\t%s\n",
                          tokenString); break;

    case ELSE:    fprintf(listing,
                          "ELSE\t\t\t%s\n",
                          tokenString); break;
    case IF:      fprintf(listing,
                          "IF\t\t\t%s\n",
                          tokenString); break;
    case INT:     fprintf(listing,
                          "INT\t\t\t%s\n",
                          tokenString); break;
    case RETURN:  fprintf(listing,
                          "RETURN\t\t\t%s\n",
                          tokenString); break;
    case VOID:    fprintf(listing,
                          "VOID\t\t\t%s\n",
                          tokenString); break;
    case WHILE:   fprintf(listing,
                          "WHILE\t\t\t%s\n",
                          tokenString); break;

    case PLUS:    fprintf(listing,
                          "PLUS\t\t\t%s\n",
                          tokenString); break;
    case MINUS:   fprintf(listing,
                          "MINUS\t\t\t%s\n",
                          tokenString); break;
    case TIMES:   fprintf(listing,
                          "TIMES\t\t\t%s\n",
                          tokenString); break;
    case OVER:    fprintf(listing,
                          "OVER\t\t\t%s\n",
                          tokenString); break;

    case LT:      fprintf(listing,
                          "<\t\t\t%s\n",
                          tokenString); break;
    case LE:      fprintf(listing,
                          "<=\t\t\t%s\n",
                          tokenString); break;
    case GT:      fprintf(listing,
                          ">\t\t\t%s\n",
                          tokenString); break;
    case GE:      fprintf(listing,
                          ">=\t\t\t%s\n",
                          tokenString); break;
    case EQ:      fprintf(listing,
                          "==\t\t\t%s\n",
                          tokenString); break;
    case NE:      fprintf(listing,
                          "!=\t\t\t%s\n",
                          tokenString); break;

    case ASSIGN:  fprintf(listing,
                          "=\t\t\t%s\n",
                          tokenString); break;
    case SEMI:    fprintf(listing,
                          ";\t\t\t%s\n",
                          tokenString); break;
    case COMMA:   fprintf(listing,
                          ",\t\t\t%s\n",
                          tokenString); break;

    case LPAREN:   fprintf(listing,
                          "(\t\t\t%s\n",
                          tokenString); break;
    case RPAREN:   fprintf(listing,
                          ")\t\t\t%s\n",
                          tokenString); break;
    case LBRACK:   fprintf(listing,
                          "[\t\t\t%s\n",
                          tokenString); break;
    case RBRACK:   fprintf(listing,
                          "]\t\t\t%s\n",
                          tokenString); break;
    case LBRACE:   fprintf(listing,
                          "{\t\t\t%s\n",
                          tokenString); break;
    case RBRACE:   fprintf(listing,
                          "}\t\t\t%s\n",
                          tokenString); break;

    default:       /* should never happen */
                   fprintf(listing,
                           "[DEBUG] UNKNOWN TOKEN %d\n",
                           token);
  }
}

TreeNode*
addSibling(TreeNode *origin, TreeNode *follow)
{
  if (origin != NULL) {
    TreeNode *t = origin;
    while (t->sibling != NULL) t = t->sibling;
    t->sibling = follow;
  }
  else {
    origin = follow;
  }
  return origin;
}

TreeNode*
allocateTreeNode(void)
{
  TreeNode *t = calloc(1, sizeof(TreeNode));
  if (t == NULL)
    {
      fprintf(listing,
              "Out of memory error at line %d\n",
              lineno);
    }
  else
    {
      t->sibling = NULL;
      t->lineno = lineno;
    }

  return t;
}

TreeNode*
newVariableDeclarationNode(TreeNode *type_specifier,
                           TreeNode *_id)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableDeclarationK;
      t->attr.type_specifier = type_specifier;
      t->attr._id = _id;
    }

  return t;
}

TreeNode*
newArrayDeclarationNode(TreeNode *type_specifier,
                        TreeNode *_id,
                        TreeNode *_num)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ArrayDeclarationK;
      t->attr.type_specifier = type_specifier;
      t->attr._id = _id;
      t->attr._num = _num;

    }

  return t;
}

TreeNode*
newFunctionDeclarationNode(TreeNode *type_specifier,
                           TreeNode *_id,
                           TreeNode *params,
                           TreeNode *compound_stmt)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = FunctionDeclarationK;
      t->attr.type_specifier = type_specifier;
      t->attr._id = _id;
      t->attr.params = params;
      t->attr.compound_stmt = compound_stmt;
    }

  return t;
}

TreeNode*
newVariableParameterNode(TreeNode *type_specifier,
                         TreeNode *_id)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableParameterK;
      t->attr.type_specifier = type_specifier;
      t->attr._id = _id;
    }

  return t;
}

TreeNode*
newArrayParameterNode(TreeNode *type_specifier,
                      TreeNode *_id)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ArrayParameterK;
      t->attr.type_specifier = type_specifier;
      t->attr._id = _id;
    }

  return t;
}

TreeNode*
newCompoundStatementNode(TreeNode *local_declarations, // nullable
                         TreeNode *statement_list) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = CompoundStatementK;
      t->attr.local_declarations = local_declarations;
      t->attr.statement_list = statement_list;
    }

  return t;
}

TreeNode*
newExpressionStatementNode(TreeNode *expression) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ExpressionStatementK;
      t->attr.expression = expression;
    }

  return t;
}

TreeNode*
newSelectionStatementNode(TreeNode *expression, // nullable
                          TreeNode *if_statement,
                          TreeNode *else_statement) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = SelectionStatementK;
      t->attr.expression = expression;
      t->attr.if_statement = if_statement;
      t->attr.else_statement = else_statement;
    }

  return t;
}

TreeNode*
newIterationStatementNode(TreeNode *expression,
                          TreeNode *statement) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = IterationStatementK;
      t->attr.expression = expression;
      t->attr.loop_statement = statement;
    }

  return t;
}

TreeNode*
newReturnStatementNode(TreeNode *expression) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ReturnStatementK;
      t->attr.expression = expression;
    }

  return t;
}

TreeNode*
newAssignExpressionNode(TreeNode *var,
                        TreeNode *expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = AssignExpressionK;
      t->attr.var = var;
      t->attr.expression = expression;
    }

  return t;
}

TreeNode*
newComparisonExpressionNode(TreeNode *left_expression,
                            TreeNode *relop,
                            TreeNode *right_expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ComparisonExpressionK;
      t->attr.left_expression = left_expression;
      t->attr.operator_specifier = relop;
      t->attr.right_expression = right_expression;
    }

  return t;
}

TreeNode*
newAdditiveExpressionNode(TreeNode *left_expression,
                          TreeNode *addop,
                          TreeNode *right_expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = AdditiveExpressionK;
      t->attr.left_expression = left_expression;
      t->attr.operator_specifier = addop;
      t->attr.right_expression = right_expression;
    }

  return t;
}

TreeNode*
newMultiplicativeExpressionNode(TreeNode *left_expression,
                                TreeNode *mulop,
                                TreeNode *right_expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = MultiplicativeExpressionK;
      t->attr.left_expression = left_expression;
      t->attr.operator_specifier = mulop;
      t->attr.right_expression = right_expression;
    }

  return t;
}

TreeNode *
newVariableNode(char *ID)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableK;
      t->attr.ID = copyString(ID);
    }
  return t;
}

TreeNode *
newArrayNode(TreeNode *_id,
             TreeNode *expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableK;
      t->attr._id = _id;
      t->attr.array_expression = expression;
    }

  return t;
}

TreeNode*
newCallNode(TreeNode *_id,
            TreeNode *args) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = CallK;
      t->attr._id = _id;
      t->attr.expression_list = args;
    }

  return t;
}

TreeNode *
newConstantNode(char *NUM)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ConstantK;
      t->attr.NUM = atoi(NUM);
    }

  return t;
}

TreeNode *
newTokenTypeNode(TokenType token)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = TokenTypeK;
      t->attr.token_type = token;
    }

  return t;

}

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char*
copyString(char * s)
{
  int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t==NULL)
    {
      fprintf(listing,
              "Out of memory error at line %d\n",
              lineno);
    }
  else 
    {
      strcpy(t,s);
    }
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
static void
printSpaces(void)
{
  int i;
  for (i=0; i<indentno; i++) fprintf(listing," ");
}

/* operatorString returns string of operator */
static const char *
operatorString(TokenType op)
{
  if(op == INT) return "int";
  if(op == VOID) return "void";

  if(op == PLUS) return "+";
  if(op == MINUS) return "-";
  if(op == TIMES) return "*";
  if(op == OVER) return "/";

  if(op == LT) return "<";
  if(op == LE) return "<=";
  if(op == GT) return ">";
  if(op == GE) return ">=";
  if(op == EQ) return "==";
  if(op == NE) return "!=";

  return "";
}

#define PRINTDESC(...)\
  do {\
      printSpaces();\
      fprintf(listing, __VA_ARGS__);\
  } while(0);

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void
printTree(TreeNode* tree)
{
  INDENT;
  if (tree == NULL) PRINTDESC("(null)\n");
  for (; tree != NULL; tree = tree->sibling)
    {
      switch (tree->nodeKind)
        {
        case ErrorK:
          PRINTDESC("[DEBUG] ErrorK at printTree\n");
          break;

        case VariableDeclarationK:
          PRINTDESC("Variable Declaration\n");
          printTree(tree->attr.type_specifier);
          printTree(tree->attr._id);
          break;

        case ArrayDeclarationK:
          PRINTDESC("Array Declaration\n");
          printTree(tree->attr.type_specifier);
          printTree(tree->attr._id);
          printTree(tree->attr._num);
          break;

        case FunctionDeclarationK:
          PRINTDESC("Function Declaration\n");
          printTree(tree->attr.type_specifier);
          printTree(tree->attr._id);
          PRINTDESC("> Parameters :\n");
          printTree(tree->attr.params);
          PRINTDESC("> Function Block :\n");
          printTree(tree->attr.compound_stmt);
          break;

        case VariableParameterK:
          PRINTDESC("Parameter (Variable)\n");
          printTree(tree->attr.type_specifier);
          printTree(tree->attr._id);
          break;

        case ArrayParameterK:
          PRINTDESC("Parameter (Array)\n");
          printTree(tree->attr.type_specifier);
          printTree(tree->attr._id);
          break;

        case CompoundStatementK:
          PRINTDESC("Compound Statement\n");
          PRINTDESC("> Local Declarations :\n");
          printTree(tree->attr.local_declarations);
          PRINTDESC("> Local Statements :\n");
          printTree(tree->attr.statement_list);
          break;

        case ExpressionStatementK:
          PRINTDESC("Expression Statement\n");
          PRINTDESC("> Expression :\n");
          printTree(tree->attr.expression);
          break;

        case SelectionStatementK:
          PRINTDESC("Selection Statement\n");
          PRINTDESC("> Expression inside if(*) :\n");
          printTree(tree->attr.expression);
          PRINTDESC("> Statements inside if clause :\n");
          printTree(tree->attr.if_statement);
          PRINTDESC("> Statements inside else clause :\n");
          printTree(tree->attr.else_statement);
          break;

        case IterationStatementK:
          PRINTDESC("Iteration Statement\n");
          PRINTDESC("> Expression inside while(*) :\n");
          printTree(tree->attr.expression);
          PRINTDESC("> Statements inside while clause :\n");
          printTree(tree->attr.loop_statement);
          break;

        case ReturnStatementK:
          PRINTDESC("Return Statement\n");
          PRINTDESC("> Returning expression :\n");
          printTree(tree->attr.expression);
          break;

        case AssignExpressionK:
          PRINTDESC("Assignment Expression\n");
          PRINTDESC("> Variable associated to assignment :\n");
          printTree(tree->attr.var);
          PRINTDESC("> Value assigned :\n");
          printTree(tree->attr.expression);
          break;

        case ComparisonExpressionK:
          PRINTDESC("Comparison Expression\n");
          printTree(tree->attr.operator_specifier);;
          PRINTDESC("> Left expression compared :\n");
          printTree(tree->attr.left_expression);
          PRINTDESC("> Right expression compared :\n");
          printTree(tree->attr.right_expression);
          break;

        case AdditiveExpressionK:
          PRINTDESC("Additive Expression\n");
          printTree(tree->attr.operator_specifier);;
          PRINTDESC("> Left expression added / subtracted :\n");
          printTree(tree->attr.left_expression);
          PRINTDESC("> Right expression added / subtracted :\n");
          printTree(tree->attr.right_expression);
          break;

        case MultiplicativeExpressionK:
          PRINTDESC("Multiplicative Expression\n");
          printTree(tree->attr.operator_specifier);;
          PRINTDESC("> Left expression multiplied / divided :\n");
          printTree(tree->attr.left_expression);
          PRINTDESC("> Right expression multiplied / divided :\n");
          printTree(tree->attr.right_expression);
          break;

        case VariableK:
          PRINTDESC("Variable Id : %s\n", tree->attr.ID);
          break;

        case ArrayK:
          PRINTDESC("Array\n");
          printTree(tree->attr._id);
          PRINTDESC("> Expression inside subscript [*]\n");
          printTree(tree->attr.expression);
          break;

        case CallK:
          PRINTDESC("Function Call\n");
          printTree(tree->attr._id);
          PRINTDESC("> Function arguments :\n");
          printTree(tree->attr.expression_list);
          break;

        case ConstantK:
          PRINTDESC("Constant : %d\n", tree->attr.NUM);
          break;

        case TokenTypeK:
          PRINTDESC("Token : %s\n", operatorString(tree->attr.token_type));
          break;

        default:
          PRINTDESC("[DEBUG] No such nodeKind\n");
        }
    }
  UNINDENT;
}

