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
           DONT_OCCUR_PRINT; 
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
                           TreeNode *_var)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableDeclarationK;
      t->attr.varDecl.type_spec = type_specifier;
      t->attr.varDecl._var = _var;
    }

  return t;
}

TreeNode*
newArrayDeclarationNode(TreeNode *type_specifier,
                        TreeNode *_var,
                        TreeNode *_num)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ArrayDeclarationK;
      t->attr.arrDecl.type_spec = type_specifier;
      t->attr.arrDecl._var = _var;
      t->attr.arrDecl._num = _num;

    }

  return t;
}

TreeNode*
newFunctionDeclarationNode(TreeNode *type_specifier,
                           TreeNode *_var,
                           TreeNode *params,
                           TreeNode *compound_stmt)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = FunctionDeclarationK;
      t->attr.funcDecl.type_spec = type_specifier;
      t->attr.funcDecl._var = _var;
      t->attr.funcDecl.params = params;
      t->attr.funcDecl.cmpd_stmt = compound_stmt;
    }

  return t;
}

TreeNode*
newVariableParameterNode(TreeNode *type_specifier,
                         TreeNode *_var)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = VariableParameterK;
      t->attr.varParam.type_spec = type_specifier;
      t->attr.varParam._var = _var;
    }

  return t;
}

TreeNode*
newArrayParameterNode(TreeNode *type_specifier,
                      TreeNode *_var)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ArrayParameterK;
      t->attr.arrParam.type_spec = type_specifier;
      t->attr.arrParam._var = _var;
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
      t->attr.cmpdStmt.local_decl = local_declarations;
      t->attr.cmpdStmt.stmt_list = statement_list;
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
      t->attr.exprStmt.expr = expression;
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
      t->attr.selectStmt.expr = expression;
      t->attr.selectStmt.if_stmt = if_statement;
      t->attr.selectStmt.else_stmt = else_statement;
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
      t->attr.iterStmt.expr = expression;
      t->attr.iterStmt.loop_stmt = statement;
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
      t->attr.retStmt.expr = expression;
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
      t->attr.assignStmt._var = var;
      t->attr.assignStmt.expr = expression;
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
      t->attr.cmpExpr.lexpr = left_expression;
      t->attr.cmpExpr.op = relop;
      t->attr.cmpExpr.rexpr = right_expression;
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
      t->attr.addExpr.lexpr = left_expression;
      t->attr.addExpr.op = addop;
      t->attr.addExpr.rexpr = right_expression;
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
      t->attr.multExpr.lexpr = left_expression;
      t->attr.multExpr.op = mulop;
      t->attr.multExpr.rexpr = right_expression;
    }

  return t;
}

TreeNode *
newArrayNode(TreeNode *_var,
             TreeNode *expression)
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = ArrayK;
      t->attr.arr._var = _var;
      t->attr.arr.arr_expr = expression;
    }

  return t;
}

TreeNode*
newCallNode(TreeNode *_var,
            TreeNode *args) // nullable
{
  TreeNode * t = allocateTreeNode();
  if (t != NULL)
    {
      t->nodeKind = CallK;
      t->attr.call._var = _var;
      t->attr.call.expr_list = args;
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
      t->attr.TOK = token;
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
  MALLOC(t, n);
  strcpy(t,s);
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
          printTree(tree->attr.varDecl.type_spec);
          printTree(tree->attr.varDecl._var);
          break;

        case ArrayDeclarationK:
          PRINTDESC("Array Declaration\n");
          printTree(tree->attr.arrDecl.type_spec);
          printTree(tree->attr.arrDecl._var);
          printTree(tree->attr.arrDecl._num);
          break;

        case FunctionDeclarationK:
          PRINTDESC("Function Declaration\n");
          printTree(tree->attr.funcDecl.type_spec);
          printTree(tree->attr.funcDecl._var);
          PRINTDESC("> Parameters :\n");
          printTree(tree->attr.funcDecl.params);
          PRINTDESC("> Function Block :\n");
          printTree(tree->attr.funcDecl.cmpd_stmt);
          break;

        case VariableParameterK:
          PRINTDESC("Parameter (Variable)\n");
          printTree(tree->attr.varParam.type_spec);
          printTree(tree->attr.varParam._var);
          break;

        case ArrayParameterK:
          PRINTDESC("Parameter (Array)\n");
          printTree(tree->attr.arrParam.type_spec);
          printTree(tree->attr.arrParam._var);
          break;

        case CompoundStatementK:
          PRINTDESC("Compound Statement\n");
          PRINTDESC("> Local Declarations :\n");
          printTree(tree->attr.cmpdStmt.local_decl);
          PRINTDESC("> Local Statements :\n");
          printTree(tree->attr.cmpdStmt.stmt_list);
          break;

        case ExpressionStatementK:
          PRINTDESC("Expression Statement\n");
          PRINTDESC("> Expression :\n");
          printTree(tree->attr.exprStmt.expr);
          break;

        case SelectionStatementK:
          PRINTDESC("Selection Statement\n");
          PRINTDESC("> Expression inside if(*) :\n");
          printTree(tree->attr.selectStmt.expr);
          PRINTDESC("> Statements inside if clause :\n");
          printTree(tree->attr.selectStmt.if_stmt);
          PRINTDESC("> Statements inside else clause :\n");
          printTree(tree->attr.selectStmt.else_stmt);
          break;

        case IterationStatementK:
          PRINTDESC("Iteration Statement\n");
          PRINTDESC("> Expression inside while(*) :\n");
          printTree(tree->attr.iterStmt.expr);
          PRINTDESC("> Statements inside while clause :\n");
          printTree(tree->attr.iterStmt.loop_stmt);
          break;

        case ReturnStatementK:
          PRINTDESC("Return Statement\n");
          PRINTDESC("> Returning expression :\n");
          printTree(tree->attr.retStmt.expr);
          break;

        case AssignExpressionK:
          PRINTDESC("Assignment Expression\n");
          PRINTDESC("> Variable associated to assignment :\n");
          printTree(tree->attr.assignStmt._var);
          PRINTDESC("> Value assigned :\n");
          printTree(tree->attr.assignStmt.expr);
          break;

        case ComparisonExpressionK:
          PRINTDESC("Comparison Expression\n");
          printTree(tree->attr.cmpExpr.op);
          PRINTDESC("> Left expression compared :\n");
          printTree(tree->attr.cmpExpr.lexpr);
          PRINTDESC("> Right expression compared :\n");
          printTree(tree->attr.cmpExpr.rexpr);
          break;

        case AdditiveExpressionK:
          PRINTDESC("Additive Expression\n");
          printTree(tree->attr.addExpr.op);
          PRINTDESC("> Left expression added / subtracted :\n");
          printTree(tree->attr.addExpr.lexpr);
          PRINTDESC("> Right expression added / subtracted :\n");
          printTree(tree->attr.addExpr.rexpr);
          break;

        case MultiplicativeExpressionK:
          PRINTDESC("Multiplicative Expression\n");
          printTree(tree->attr.multExpr.op);
          PRINTDESC("> Left expression multiplied / divided :\n");
          printTree(tree->attr.multExpr.lexpr);
          PRINTDESC("> Right expression multiplied / divided :\n");
          printTree(tree->attr.multExpr.rexpr);
          break;

        case VariableK:
          PRINTDESC("Variable Id : %s\n", tree->attr.ID);
          break;

        case ArrayK:
          PRINTDESC("Array\n");
          printTree(tree->attr.arr._var);
          PRINTDESC("> Expression inside subscript [*]\n");
          printTree(tree->attr.arr.arr_expr);
          break;

        case CallK:
          PRINTDESC("Function Call\n");
          printTree(tree->attr.call._var);
          PRINTDESC("> Function arguments :\n");
          printTree(tree->attr.call.expr_list);
          break;

        case ConstantK:
          PRINTDESC("Constant : %d\n", tree->attr.NUM);
          break;

        case TokenTypeK:
          PRINTDESC("Token : %s\n", operatorString(tree->attr.TOK));
          break;

        default:
          PRINTDESC("[DEBUG] No such nodeKind\n");
        }
    }
  UNINDENT;
}

