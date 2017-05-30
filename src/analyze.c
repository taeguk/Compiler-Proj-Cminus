/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "symtab.h"

/* counter for variable memory locations */
static int location = 0;

/* Procedure traverse is a generic recursive 
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc 
 * in postorder to tree pointed to by t
 */
/*
static void traverse( TreeNode * t,
               void (* preProc) (TreeNode *),
               void (* postProc) (TreeNode *) )
{ if (t != NULL)
  { preProc(t);
    { int i;
      for (i=0; i < MAXCHILDREN; i++)
        traverse(t->child[i],preProc,postProc);
    }
    postProc(t);
    traverse(t->sibling,preProc,postProc);
  }
}
*/
/* nullProc is a do-nothing procedure to 
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{ if (t==NULL) return;
  else return;
}

/* Procedure insertNode inserts 
 * identifiers stored in t into 
 * the symbol table 
 */

#define NewID 1 // <-> ExistId
#define AlreadyPushedScope 2

static void registerSymbol(TreeNode *reg_node, TreeNode *idNode, int flags)
{
  if (flags & NewID)
    {
      if (st_lookup(idNode->attr.ID) == -1)
        /* not yet in table, so treat as new definition */
        st_insert(idNode->attr.ID, idNode->lineno, reg_node, 0 /* TODO: in project 4 */);
      else
        /* TODO: ERROR!!!!! */;
    }
  else
    {
      if (st_lookup(idNode->attr.ID) == -1)
        /* TODO: ERROR!!!!! */;
      else
        /* already in table, so ignore location, 
         * add line number of use only */ 
        st_insert(idNode->attr.ID, idNode->lineno, NULL, 0);
    }

}

static void insertNode( TreeNode * t, int flags)
{ 
  /*
  if (t == NULL)
    {
      printf("[Debug in insertNode] t == NULL!!!\n");
      return;
    }
  */
  for (; t; t = t->sibling)
    {
      switch (t->nodeKind)
        {
          /* Declaration Kinds */
        case VariableDeclarationK:
          registerSymbol(t, t->attr.varDecl._id, NewID);
          break;
        case ArrayDeclarationK:
          registerSymbol(t, t->attr.arrDecl._id, NewID);
          break;
        case FunctionDeclarationK:
          registerSymbol(t, t->attr.funcDecl._id, NewID);
          st_push_scope();
          insertNode(t->attr.funcDecl.params, 0);
          insertNode(t->attr.funcDecl.cmpd_stmt, AlreadyPushedScope);
          break;

          /* Parameter Kinds */
        case VariableParameterK:
          registerSymbol(t, t->attr.varParam._id, NewID);
          break;
        case ArrayParameterK:
          registerSymbol(t, t->attr.arrParam._id, NewID);
          break;

          /* Statement Kinds */
        case CompoundStatementK:
          if (!(flags & AlreadyPushedScope))
            st_push_scope();
          insertNode(t->attr.cmpdStmt.local_decl, 0);
          insertNode(t->attr.cmpdStmt.stmt_list, 0);
          printSymTab(listing);
          st_pop_scope();
          break;
        case ExpressionStatementK:
          insertNode(t->attr.exprStmt.expr, 0);
          break;
        case SelectionStatementK:
          insertNode(t->attr.selectStmt.expr, 0);
          insertNode(t->attr.selectStmt.if_stmt, 0);
          insertNode(t->attr.selectStmt.else_stmt, 0);
          break;
        case IterationStatementK:
          insertNode(t->attr.iterStmt.expr, 0);
          insertNode(t->attr.iterStmt.loop_stmt, 0);
          break;
        case ReturnStatementK:
          insertNode(t->attr.retStmt.expr, 0);
          break;

          /* Expression Kinds */
        case AssignExpressionK:
          insertNode(t->attr.assignStmt.expr, 0);
          insertNode(t->attr.assignStmt._var, 0);
          break;
        case ComparisonExpressionK:
          insertNode(t->attr.cmpExpr.lexpr, 0);
          insertNode(t->attr.cmpExpr.op, 0);
          insertNode(t->attr.cmpExpr.rexpr, 0);
          break;
        case AdditiveExpressionK:
          insertNode(t->attr.addExpr.lexpr, 0);
          insertNode(t->attr.addExpr.op, 0);
          insertNode(t->attr.addExpr.rexpr, 0);
          break;
        case MultiplicativeExpressionK:
          insertNode(t->attr.multExpr.lexpr, 0);
          insertNode(t->attr.multExpr.op, 0);
          insertNode(t->attr.multExpr.rexpr, 0);
          break;

          /* Array Subscription and Function Invoke */
        case ArrayK:
          registerSymbol(t, t->attr.arr._id, 0);
          insertNode(t->attr.arr.arr_expr, 0);
          break;
        case CallK:
          registerSymbol(t, t->attr.call._id, 0);
          insertNode(t->attr.call.expr_list, 0);
          break;

          /* Leaf Nodes */
          /* Cannot be reached here!! */
        case VariableK:
          /* nothing to do */
          break;
        case ConstantK:
          /* nothing to do */
          break;
        case TokenTypeK:
          /* nothing to do */
          break;
        case ErrorK:
          /* TODO: ERROR!! */
        default:
          /* TODO: ERROR!! */
          ;
        }
    }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{ //traverse(syntaxTree,insertNode,nullProc);
  insertNode(syntaxTree, 0);
  printSymTab(listing);
  /*
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
  */
}

static void typeError(TreeNode * t, char * message)
{ fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
  Error = TRUE;
}

/* Procedure checkNode performs
 * type checking at a single tree node
 */
/*
static void checkNode(TreeNode * t)
{ switch (t->nodekind)
  { case ExpK:
      switch (t->kind.exp)
      { case OpK:
          if ((t->child[0]->type != Integer) ||
              (t->child[1]->type != Integer))
            typeError(t,"Op applied to non-integer");
          if ((t->attr.op == EQ) || (t->attr.op == LT))
            t->type = Boolean;
          else
            t->type = Integer;
          break;
        case ConstK:
        case IdK:
          t->type = Integer;
          break;
        default:
          break;
      }
      break;
    case StmtK:
      switch (t->kind.stmt)
      { case IfK:
          if (t->child[0]->type == Integer)
            typeError(t->child[0],"if test is not Boolean");
          break;
        case AssignK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"assignment of non-integer value");
          break;
        case WriteK:
          if (t->child[0]->type != Integer)
            typeError(t->child[0],"write of non-integer value");
          break;
        case RepeatK:
          if (t->child[1]->type == Integer)
            typeError(t->child[1],"repeat test is not Boolean");
          break;
        default:
          break;
      }
      break;
    default:
      break;

  }
}
*/
/* Procedure typeCheck performs type checking 
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * syntaxTree)
{ //traverse(syntaxTree,nullProc,checkNode);
}
