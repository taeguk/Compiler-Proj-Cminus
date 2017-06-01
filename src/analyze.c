/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the TINY compiler                            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdarg.h>

#include "globals.h"
#include "symtab.h"
#include "analyze.h"
#include "symtab.h"

/* counter for variable memory locations */
static int location = 0;

static NodeType tokenToNodeType (TokenType token)
{
  switch (token)
    {
    case INT:
      return IntT;
    case VOID:
      return VoidT;
    default:
      DONT_OCCUR_PRINT;
      return ErrorT;
    }
}

static SymbolInfo * setSymbolInfo (TreeNode *t)
{
  SymbolInfo * symbolInfo;

  if ((symbolInfo = malloc(sizeof(*symbolInfo)) ) == NULL)
    {
      // Error
      DONT_OCCUR_PRINT;
      return NULL;
    }

  switch (t->nodeKind)
    {
      case VariableDeclarationK:
        if (tokenToNodeType(t->attr.varDecl.type_spec->attr.TOK) != IntT)
          return NULL; // TODO: Error Message
        symbolInfo->nodeType = IntT;
        break;


      case ArrayDeclarationK:
        if (tokenToNodeType(t->attr.funcDecl.type_spec->attr.TOK) != IntT)
          return NULL; // TODO: Error message
        symbolInfo->nodeType = IntArrayT; // TODO: Type check
        symbolInfo->attr.arrInfo.len = t->attr.arrDecl._num->attr.NUM; // TODO: bracket type check
        break;


      case FunctionDeclarationK:
        if (tokenToNodeType(t->attr.funcDecl.type_spec->attr.TOK) == ErrorT)
          return NULL; // TODO: Error message
        symbolInfo->nodeType = FuncT;
        symbolInfo->attr.funcInfo.retType =
          tokenToNodeType(t->attr.funcDecl.type_spec->attr.TOK);

        TreeNode * trace;
        NodeType * newParamList;
        int n_param;
        int i;

        n_param = 0;
        trace = t->attr.funcDecl.params;

        // TODO: Parameter == 0 check, Void check
        while (trace)
          {
            ++ n_param;
            trace = trace->sibling;
          }
        if ((newParamList = malloc(n_param * sizeof(NodeType))) == NULL)
          {
            DONT_OCCUR_PRINT;
            return NULL;
          }

        trace = t->attr.funcDecl.params;
        i = 0;
        while (trace)
          {
            if (trace->nodeKind == VariableParameterK)
              newParamList[i] =
                tokenToNodeType(trace->attr.varParam.type_spec->attr.TOK); // Is it need type check?
            else if (trace->nodeKind == ArrayParameterK)
              newParamList[i] =
                tokenToNodeType(trace->attr.arrParam.type_spec->attr.TOK);
            else
              {
                DONT_OCCUR_PRINT; // TODO: check is it really "DON'T occur"
                return NULL;
              }

            ++i;
            trace = trace->sibling;
          }


        symbolInfo->attr.funcInfo.paramTypeList = newParamList;
        break;


      case VariableParameterK:
        if (tokenToNodeType(t->attr.varParam.type_spec->attr.TOK) != IntT)
          return NULL; // TODO: Error Message
        symbolInfo->nodeType = IntT;
        break;


      case ArrayParameterK:
        if (tokenToNodeType(t->attr.arrParam.type_spec->attr.TOK) != IntT)
          return NULL; // TODO: Error Message
        symbolInfo->nodeType = IntArrayT;
        break;
    }


  return symbolInfo;
}

static void printError(TreeNode * t, const char *error_type, const char *fmt, ...)
{
  va_list args;
  fprintf(listing, "%s error at line %d: ", error_type, t->lineno);

  va_start(args, fmt);
  vfprintf(listing, fmt, args);
  va_end(args);

  fprintf(listing, "\n");
  Error = TRUE;
}

#define AlreadyPushedScope 2

static int registerSymbol(TreeNode *regNode, TreeNode *idNode, SymbolInfo * symbolInfo)
{
  int is_cur_scope;
  if (st_lookup(idNode->attr.ID, &is_cur_scope) == NULL || !is_cur_scope)
    {
      /* not yet in table, so treat as new definition */
      st_insert(idNode->attr.ID, idNode->lineno, regNode, symbolInfo /* TODO: in project 4 */);
      idNode->nodeType = symbolInfo->nodeType;
      idNode->symbolInfo = symbolInfo;
      return TRUE;
    }
  else /* redeclaration */
    {
      printError(idNode, "Declaration", "Redeclaration of symbol \"%s\"", idNode->attr.ID);
      idNode->nodeType = ErrorT;
      return FALSE;
    }
}

static void referSymbol(TreeNode *reg_node, TreeNode *idNode)
{
  int is_cur_scope;
  SymbolInfo * symbolInfo;
  if ((symbolInfo = st_lookup(idNode->attr.ID, &is_cur_scope)) == NULL) /* undeclared V/P/F */
    {
      printError(idNode, "Declaration", "Undeclared symbol \"%s\"", idNode->attr.ID);
    }
  else
    {
      /* already in table, so ignore location,
       * add line number of use only */
      st_insert(idNode->attr.ID, idNode->lineno, NULL, NULL);
      idNode->nodeType = symbolInfo->nodeType;
      idNode->symbolInfo = symbolInfo;
    }
}

static void insertNode( TreeNode * t, int flags)
{
  int registerSuccess;
  SymbolInfo * symbolInfo = setSymbolInfo(t);
  for (; t; t = t->sibling)
    {
      switch (t->nodeKind)
        {
          /* Declaration Kinds */
        case VariableDeclarationK:
          if(symbolInfo == NULL) break;
          registerSuccess = registerSymbol(t, t->attr.varDecl._id, symbolInfo);
          break;
        case ArrayDeclarationK:
          if(symbolInfo == NULL) break;
          registerSuccess = registerSymbol(t, t->attr.arrDecl._id, symbolInfo);
          break;
        case FunctionDeclarationK:
          if(symbolInfo == NULL) break;
          registerSuccess = registerSymbol(t, t->attr.funcDecl._id, symbolInfo);
          st_push_scope();
          insertNode(t->attr.funcDecl.params, 0);
          insertNode(t->attr.funcDecl.cmpd_stmt, AlreadyPushedScope);
          break;

          /* Parameter Kinds */
        case VariableParameterK:
          if(symbolInfo == NULL) break;
          registerSymbol(t, t->attr.varParam._id, symbolInfo);
          break;
        case ArrayParameterK:
          if(symbolInfo == NULL) break;
          registerSymbol(t, t->attr.arrParam._id, symbolInfo);
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

        case VariableK:
          referSymbol(t, t);
          break;
        case ArrayK:
          referSymbol(t, t->attr.arr._id);
          insertNode(t->attr.arr.arr_expr, 0);
          break;
        case CallK:
          referSymbol(t, t->attr.call._id);
          insertNode(t->attr.call.expr_list, 0);
          break;

          /* Leaf Nodes */
          /* Cannot be reached here!! */
        case ConstantK:
          /* nothing to do */
          break;
        case TokenTypeK:
          /* nothing to do */
          break;
        case ErrorK:
          DONT_OCCUR_PRINT;
          break;
        default:
          DONT_OCCUR_PRINT;
        }
    }
}

/* Function buildSymtab constructs the symbol 
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{
  insertNode(syntaxTree, 0);
  printSymTab(listing);
  /*
  if (TraceAnalyze)
  { fprintf(listing,"\nSymbol table:\n\n");
    printSymTab(listing);
  }
  */
}

/* Procedure typeCheck performs type evaluation
 * by syntax tree traversal
 */
NodeType typeCheck(TreeNode *n)
{
  if (n == NULL) return NoneT;
  if (n->nodeType != NotResolvedT) return n->nodeType;

  TreeNode *t = n;
  for(t=n; t; t=t->sibling)
    {
      switch (t->nodeKind)
        {
        case VariableDeclarationK:
          if(t->attr.varDecl.type_spec->attr.TOK != INT)
            {
              printError(t, "Type", "Variable type other than 'int' is not allowed.");
              t->nodeType = ErrorT;
            }
          else
            {
              t->nodeType = NoneT;
            }
          break;

        case ArrayDeclarationK:
          if(t->attr.arrDecl.type_spec->attr.TOK != INT)
            {
              printError(t, "Type", "Array type other than 'int' is not allowed.");
              t->nodeType = ErrorT;
            }
          else
            {
              t->nodeType = NoneT;
            }
          break;

        case FunctionDeclarationK:
        {
          TreeNode *cmpdStmt = t->attr.funcDecl.cmpd_stmt;
          if(cmpdStmt != NULL)
            {
              if(t->attr.arrDecl.type_spec->attr.TOK != INT)
                cmpdStmt->attr.cmpdStmt.retType = IntT;
              else if(t->attr.arrDecl.type_spec->attr.TOK != VOID)
                cmpdStmt->attr.cmpdStmt.retType = VoidT;
              else
                DONT_OCCUR_PRINT;
              typeCheck(t->attr.funcDecl.cmpd_stmt);
            }
          t->nodeType = NoneT;
        }
          break;

        case VariableParameterK:
          if(t->attr.varParam.type_spec->attr.TOK != INT)
            {
              printError(t, "Type", "Parameter type other than 'int' or 'int[ ]' is not allowed.");
              t->nodeType = ErrorT;
            }
          else
            {
              t->nodeType = NoneT;
            }
          break;

        case ArrayParameterK:
          if(t->attr.arrParam.type_spec->attr.TOK != INT)
            {
              printError(t, "Type", "Parameter type other than 'int' or 'int[ ]' is not allowed.");
              t->nodeType = ErrorT;
            }
          else
            {
              t->nodeType = NoneT;
            }
          break;

        case CompoundStatementK:
        {
          // Propagate return type for return statement
          TreeNode *stmt;
          for(stmt = t->attr.cmpdStmt.stmt_list;
              stmt != NULL;
              stmt = stmt->sibling)
            {
              if(stmt->nodeKind == ReturnStatementK)
                {
                  stmt->attr.retStmt.retType = t->attr.cmpdStmt.retType;
                }
            }

          typeCheck(t->attr.cmpdStmt.local_decl);
          typeCheck(t->attr.cmpdStmt.stmt_list);
          t->nodeType = NoneT;
        }
          break;

        case ExpressionStatementK:
          typeCheck(t->attr.exprStmt.expr);
          t->nodeType = NoneT;
          break;

        case SelectionStatementK:
          if (typeCheck(t->attr.selectStmt.expr) != IntT)
            {
              printError(t, "Type", "Expression inside selection statement should be type 'int'.");
              t->nodeType = ErrorT;
            }
          else
            {
              typeCheck(t->attr.selectStmt.if_stmt);
              typeCheck(t->attr.selectStmt.else_stmt);
              t->nodeType = NoneT;
            }
          break;

        case IterationStatementK:
          if (typeCheck(t->attr.iterStmt.expr) != IntT)
            {
              printError(t, "Type", "Expression inside iteration statement should be type 'int'.");
              t->nodeType = ErrorT;
            }
          else
            {
              typeCheck(t->attr.iterStmt.loop_stmt);
              t->nodeType = NoneT;
            }
          break;

        case ReturnStatementK:
        {
          if(t->attr.retStmt.retType != IntT
             || t->attr.retStmt.retType != VoidT)
            {
              DONT_OCCUR_PRINT;
              t->nodeType = ErrorT;
            }
          else
            {
              NodeType type = typeCheck(t->attr.retStmt.expr);
              if(type != t->attr.retStmt.retType)
                {
                  printError(t, "Type", "Returning expression must match pre-declared function return type.");
                  t->nodeType = ErrorT;
                }
              else
                {
                  t->nodeType = NoneT;
                }
            }
        }
          break;

        case AssignExpressionK:
        {
          NodeType exprType = typeCheck(t->attr.assignStmt.expr);
          NodeType varType = typeCheck(t->attr.assignStmt._var);
          if (varType != IntT || varType != exprType)
            {
              printError(t, "Type", "Assignment and assignee type mismatch");
              t->nodeType = ErrorT;
            }
          else
            {
              t->nodeType = varType;
            }
        }
          break;

        case ComparisonExpressionK:
        {
          NodeType lType = typeCheck(t->attr.cmpExpr.lexpr);
          NodeType rType = typeCheck(t->attr.cmpExpr.rexpr);
          if (lType == IntT && lType == rType)
            {
              t->nodeType = IntT;
            }
          else
            {
              printError(t, "Type", "Comparison between different types.");
              t->nodeType = ErrorT;
            }
        }
          break;

        case AdditiveExpressionK:
        {
          NodeType lType = typeCheck(t->attr.addExpr.lexpr);
          NodeType rType = typeCheck(t->attr.addExpr.rexpr);
          if (lType == IntT && lType == rType)
            {
              t->nodeType = IntT;
            }
          else
            {
              printError(t, "Type", "Addition between different types.");
              t->nodeType = ErrorT;
            }
        }
          break;

        case MultiplicativeExpressionK:
        {
          NodeType lType = typeCheck(t->attr.multExpr.lexpr);
          NodeType rType = typeCheck(t->attr.multExpr.rexpr);
          if (lType == IntT && lType == rType)
            {
              t->nodeType = IntT;
            }
          else
            {
              printError(t, "Type", "Multiplication between different types.");
              t->nodeType = ErrorT;
            }
        }
          break;

        case ArrayK:
        {
          int isError = FALSE;
          if (typeCheck(t->attr.arr._id) != IntArrayT)
            {
              // TODO: error print
              t->nodeType = ErrorT;
              isError = TRUE;
            }
          if(typeCheck(t->attr.arr.arr_expr) != IntT)
            {
              // TODO: error print
              t->nodeType = ErrorT;
              isError = TRUE;
            }

          if(!isError)
            {
              t->nodeType = IntT;
            }
        }

        case CallK:
        {
          int isError = FALSE;
          SymbolInfo *info = t->symbolInfo;
          NodeType fType = typeCheck(t->attr.call._id);

          if (info == NULL)
            {
              DONT_OCCUR_PRINT;
              t->nodeType = ErrorT;
              isError = TRUE;
            }

          if (fType != FuncT)
            {
              if (fType == IntT || fType == IntArrayT)
                {
                  printError(t, "Type", "Variable '%s' is not callable.", t->attr.call._id->attr.ID);
                }
              else
                {
                  printError(t, "Type", "'%s' is never declared.", t->attr.call._id->attr.ID);
                }
              t->nodeType = ErrorT;
              isError = TRUE;
            }

          if(!isError)
            {
              // Parameter type checking
              int exprIdx = 0;
              TreeNode *expr;
              for(expr = t->attr.call.expr_list;
                  expr != NULL;
                  expr = expr->sibling, exprIdx++)
                {
                  if(exprIdx >= info->attr.funcInfo.len)
                    {
                      printError(t,
                                 "Type",
                                 "Too many parameters while calling function '%s'.",
                                 t->attr.call._id->attr.ID);
                      isError = TRUE;
                      break;
                    }
                  if(typeCheck(expr) != info->attr.funcInfo.paramTypeList[exprIdx])
                    {
                      printError(t,
                                 "Type",
                                 "Type mismatch of parameter at %d while calling '%s'.",
                                 exprIdx + 1,
                                 t->attr.call._id->attr.ID);
                      isError = TRUE;
                    }
                }
              if(exprIdx < info->attr.funcInfo.len)
                {
                  printError(t,
                             "Type",
                             "Too little parameters while calling function '%s'.",
                             t->attr.call._id->attr.ID);
                  isError = TRUE;
                  break;
                }
            }

          if(!isError)
            {
              t->nodeType = info->attr.funcInfo.retType;
            }

        }
          break;

        case VariableK:
          /* TODO: get variable's type from st_lookup.
           * we have to modify st_lookup for getting variable's type.
           * */
          t->nodeType = ErrorT;
          break;

        case ConstantK:
          t->nodeType = IntT;
          break;
        case TokenTypeK:
          t->nodeType = NoneT;
          break;

        case ErrorK:
          DONT_OCCUR_PRINT;
        default:
          DONT_OCCUR_PRINT;
          break;
        }
    }

  return n->nodeType;
}
