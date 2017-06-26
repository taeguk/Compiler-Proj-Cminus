#include "globals.h"
#include "cgen.h"
#include <string.h>

static int regSize = 4;

static int globalMemAlloc(int);
static int labelAlloc(void);
static int localCodeGen(TreeNode *, FILE *, int);

static int L_cleanup;

// Global decls
void codeGen(TreeNode *syntaxTree, FILE *codeStream)
{
  int i;
  TreeNode *t;

  fprintf(codeStream, ".data\n");
  fprintf(codeStream, "newline: .asciiz \"\\n\"\n");
  fprintf(codeStream, "output_str: .asciiz \"Output : \"\n");
  fprintf(codeStream, "input_str: .asciiz \"Input : \"\n");


  fprintf(codeStream, "\n.text\n");
  for(t = syntaxTree;
      t != NULL;
      t = t->sibling)
    {
      if(t->nodeKind == VariableDeclarationK)
        {
          t->attr.varDecl._var->symbolInfo->attr.intInfo.memloc = globalMemAlloc(sizeof(int));
          t->attr.varDecl._var->symbolInfo->attr.intInfo.globalFlag = TRUE;
        }
      else if(t->nodeKind == ArrayDeclarationK)
        {
          t->attr.arrDecl._var->symbolInfo->attr.arrInfo.memloc =
            globalMemAlloc(sizeof(int) * t->attr.arrDecl._var->symbolInfo->attr.arrInfo.arrLen);
          t->attr.arrDecl._var->symbolInfo->attr.arrInfo.globalFlag = TRUE;
        }
      else if(t->nodeKind == FunctionDeclarationK)
        {
          // Function labeling
          fprintf(codeStream, "# Function declaration\n");
          fprintf(codeStream, "%s:\n", t->attr.funcDecl._var->attr.ID);

          // Function parameter's total memory
          TreeNode *param;
          int accLoc = 0;
          for(param = t->attr.funcDecl.params;
              param != NULL;
              param = param->sibling)
            {
              TreeNode *var = param->attr.funcDecl._var;
              if(var->symbolInfo->nodeType == IntT)
                accLoc += sizeof(int);
              else if(var->nodeType == IntArrayT)
                accLoc += regSize;
            }

          // real location
          // ex. f(a, b, c)
          // a: 8(fp), b: 4(fp), c:0(fp)
          // pushed early <--> pushed late
          for(param = t->attr.funcDecl.params;
              param != NULL;
              param = param->sibling)
            {
              switch(param->nodeType)
                {
                case VariableParameterK:
                  accLoc -= sizeof(int);
                  param->attr.varParam._var->symbolInfo->attr.intInfo.memloc = accLoc;
                  break;
                case ArrayParameterK:
                  accLoc -= regSize;
                  param->attr.arrParam._var->symbolInfo->attr.intInfo.memloc = accLoc;
                  break;
                default:
                  DONT_OCCUR_PRINT;
                }
            }

          // allocate stack
          fprintf(codeStream, "\n# Allocate stack\n");
          fprintf(codeStream, "addiu $sp, $sp, %d\n", -10 * regSize);

          // save register $fp
          fprintf(codeStream, "# Save registers\n");
          fprintf(codeStream, "sw $fp, %d($sp)\n", 0 * regSize);
          // save registers $s0~$s7
          for(i=0; i<8; ++i)
            fprintf(codeStream, "sw $s%d, %d($sp)\n", i, (i+1) * regSize);
          // save register $ra
          fprintf(codeStream, "sw $ra, %d($sp)\n", 9 * regSize);

          // set frame
          fprintf(codeStream, "addiu $fp, $sp, %d\n", 10 * regSize);

          L_cleanup = labelAlloc();

          // cmpd statement generation
          fprintf(codeStream, "\n# Compound statement for function\n");
          int updateStack = localCodeGen(t->attr.funcDecl.cmpd_stmt, codeStream, 10 * regSize);
          if(updateStack != 10 * regSize)
            DONT_OCCUR_PRINT;

          // cleanup for function with no return
          fprintf(codeStream, "\n# Stack cleanup\n");
          fprintf(codeStream, "L%d:\n", L_cleanup);

          // cleanup remained local stack.
          fprintf(codeStream, "addiu $sp, $fp, %d\n", -10 * regSize);

          // load registers
          fprintf(codeStream, "lw $fp, %d($sp)\n", 0 * regSize);
          // load registers $s0~$s7
          for(i=0; i<8; ++i)
            fprintf(codeStream, "lw $s%d, %d($sp)\n", i, (i+1) * regSize);
          // load register $ra
          fprintf(codeStream, "lw $ra, %d($sp)\n", 9 * regSize);
          fprintf(codeStream, "addiu $sp, $sp, %d\n", 10 * regSize);

          // return
          fprintf(codeStream, "\n# Return to caller\n");
          fprintf(codeStream, "jr $ra\n\n");
        }
      else
        DONT_OCCUR_PRINT;
    }
}

// Global memory always starts with 0x10000000
static int globalMemAlloc(int size)
{
  if (size <= 0) DONT_OCCUR_PRINT;
  static int addr = 0x10000000;
  addr += size;
  return addr-size;
}

// Label generator
static int labelAlloc(void)
{
  static int label = 0x00000000;
  return label++;
}

// Local decls
static int localCodeGen(TreeNode *syntaxTree, FILE *codeStream, int currStack)
{
  TreeNode *t;
  for(t = syntaxTree;
      t != NULL;
      t = t->sibling)
    {
      switch (t->nodeKind)
        {

        case VariableDeclarationK:
        {
          int size = sizeof(int);

          fprintf(codeStream, "\n# Local variable declaration\n");
          fprintf(codeStream, "addiu $sp, $sp, %d\n", -size);
          currStack += size;
          t->attr.varDecl._var->symbolInfo->attr.intInfo.memloc = -currStack;
          break;
        }
        case ArrayDeclarationK:
        {
          int size = regSize;
          fprintf(codeStream, "\n# Local array declaration\n");
          fprintf(codeStream, "addiu $sp, $sp, %d\n", -size);
          currStack += size * t->attr.varDecl._var->symbolInfo->attr.arrInfo.arrLen;
          t->attr.varDecl._var->symbolInfo->attr.arrInfo.memloc = -currStack;
          break;
        }

        case CompoundStatementK:
        {
          fprintf(codeStream, "\n# Compound Statement\n");
          int updateStack = currStack;

          updateStack = localCodeGen(t->attr.cmpdStmt.local_decl, codeStream, updateStack);
          if(localCodeGen(t->attr.cmpdStmt.stmt_list, codeStream, updateStack) != updateStack)
            DONT_OCCUR_PRINT;

          // stack cleanup
          if(updateStack < currStack)
            DONT_OCCUR_PRINT;

          fprintf(codeStream, "\n# Local stack cleanup\n");
          fprintf(codeStream, "addiu $sp, $sp, %d\n", updateStack - currStack);

          /*
          if(updateStack > 0) // it does not have return stmt
            {
              fprintf(codeStream, "\n# Local stack cleanup\n");
              fprintf(codeStream, "addiu $sp, $sp, %d\n", updateStack - currStack);
            }
          */
          break;
        }
        case ExpressionStatementK:
        {
          if(localCodeGen(t->attr.exprStmt.expr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          break;
        }
        case SelectionStatementK:
        {
          fprintf(codeStream, "\n# Selection Statement\n");
          fprintf(codeStream, "# Selection Statement Expression\n");
          if(localCodeGen(t->attr.selectStmt.expr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          int L_exit = labelAlloc(), L_false = labelAlloc();
          fprintf(codeStream, "beqz $v0, L%d\n", L_false);
          fprintf(codeStream, "# Selection Statement If Statement\n");
          if(localCodeGen(t->attr.selectStmt.if_stmt, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "j L%d\n", L_exit);
          fprintf(codeStream, "L%d:\n", L_false);
          fprintf(codeStream, "# Selection Statement Else Statement\n");
          if(localCodeGen(t->attr.selectStmt.else_stmt, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "L%d:\n", L_exit);
          break;
        }
        case IterationStatementK:
        {
          fprintf(codeStream, "\n# Iteration Statement\n");
          int L_cmp = labelAlloc(), L_loop = labelAlloc();
          fprintf(codeStream, "j L%d\n", L_cmp);
          fprintf(codeStream, "L%d:\n", L_loop);
          fprintf(codeStream, "# Iteration Statement Loop Statement\n");
          if(localCodeGen(t->attr.iterStmt.loop_stmt, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "L%d:\n", L_cmp);
          fprintf(codeStream, "# Iteration Statement Expression\n");
          if(localCodeGen(t->attr.iterStmt.expr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "bnez $v0, L%d\n", L_loop);
          break;
        }
        case ReturnStatementK:
        {
          // TODO: review this
          // ex) have to jr immediatly (but have to clean stack with prev value
          // of registers. no need to load. just roll up the stack.)
          // int f(int i) {
          //   if(i) {
          //     int j;
          //     return 1;
          //   }
          //   return 0;
          // }
          if(t->attr.retStmt.expr != NULL)
            if(localCodeGen(t->attr.retStmt.expr, codeStream, currStack) != currStack)
              DONT_OCCUR_PRINT;

          fprintf(codeStream, "j L%d\n", L_cleanup);

          break;

          /*
          // cleanup
          fprintf(codeStream, "\n# Stack cleanup\n");

          // return to original sp
          fprintf(codeStream, "addiu $sp, $sp, %d\n", currStack - 10 * regSize);

          // load register $fp
          fprintf(codeStream, "lw $fp, %d($sp)\n", 0 * regSize);
          // load registers $s0~$s7
          int i;
          for(i=0; i<8; ++i)
            fprintf(codeStream, "lw $s%d, %d($sp)\n", i, (i+1) * regSize);
          // load register $ra
          fprintf(codeStream, "lw $ra, %d($sp)\n", 9 * regSize);

          // free stack
          fprintf(codeStream, "addiu $sp, $sp, %d\n", 10 * regSize);

          // return
          fprintf(codeStream, "\n# Return to caller\n");
          fprintf(codeStream, "jr $ra\n\n");
          */

          // Optimization: do not need to check other stmts
          //return 0;
        }

        case AssignExpressionK:
        {
          if(localCodeGen(t->attr.assignStmt.expr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          if (t->attr.assignStmt._var->nodeKind == VariableK)
            {
              fprintf(codeStream,
                      t->attr.assignStmt._var->symbolInfo->attr.intInfo.globalFlag ?
                      "sw $v0, %d\n" : "sw $v0, %d($fp)\n",
                      t->attr.assignStmt._var->symbolInfo->attr.intInfo.memloc);
            }
          else if (t->attr.assignStmt._var->nodeKind == ArrayK)
            {
              fprintf(codeStream, "move $s0, $v0\n");
              if(localCodeGen(t->attr.assignStmt.expr, codeStream, currStack) != currStack)
                DONT_OCCUR_PRINT;
              fprintf(codeStream, "sw $s0, 0($v0)\n");
            }
          else
            DONT_OCCUR_PRINT;
          break;
        }
        case ComparisonExpressionK:
        {
          if(localCodeGen(t->attr.cmpExpr.lexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "addiu $sp, $sp, -%lu\n", sizeof(int));
          fprintf(codeStream, "sw $v0, 0($sp)\n");
          currStack += sizeof(int);

          if(localCodeGen(t->attr.cmpExpr.rexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "lw $s0, 0($sp)\n");
          fprintf(codeStream, "addiu $sp, $sp, %lu\n", sizeof(int));
          currStack -= sizeof(int);

          switch(t->attr.cmpExpr.op->attr.TOK)
            {
            case LT: fprintf(codeStream, "slt $v0, $s0, $v0\n"); break;
            case LE: fprintf(codeStream, "sle $v0, $s0, $v0\n"); break;
            case GT: fprintf(codeStream, "sgt $v0, $s0, $v0\n"); break;
            case GE: fprintf(codeStream, "sge $v0, $s0, $v0\n"); break;
            case EQ: fprintf(codeStream, "seq $v0, $s0, $v0\n"); break;
            case NE: fprintf(codeStream, "sne $v0, $s0, $v0\n"); break;
            default: DONT_OCCUR_PRINT;
            }
          break;
        }
        case AdditiveExpressionK:
        {
          if(localCodeGen(t->attr.addExpr.lexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "addiu $sp, $sp, -%lu\n", sizeof(int));
          fprintf(codeStream, "sw $v0, 0($sp)\n");
          currStack += sizeof(int);

          if(localCodeGen(t->attr.addExpr.rexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "lw $s0, 0($sp)\n");
          fprintf(codeStream, "addiu $sp, $sp, %lu\n", sizeof(int));
          currStack -= sizeof(int);

          switch(t->attr.addExpr.op->attr.TOK)
            {
            case PLUS: fprintf(codeStream, "add $v0, $s0, $v0\n"); break;
            case MINUS: fprintf(codeStream, "sub $v0, $s0, $v0\n"); break;
            default: DONT_OCCUR_PRINT;
            }
          break;
        }
        case MultiplicativeExpressionK:
        {
          if(localCodeGen(t->attr.multExpr.lexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "addiu $sp, $sp, -%lu\n", sizeof(int));
          fprintf(codeStream, "sw $v0, 0($sp)\n");
          currStack += sizeof(int);

          if(localCodeGen(t->attr.multExpr.rexpr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "lw $s0, 0($sp)\n");
          fprintf(codeStream, "addiu $sp, $sp, %lu\n", sizeof(int));
          currStack -= sizeof(int);

          switch(t->attr.multExpr.op->attr.TOK)
            {
            case TIMES: fprintf(codeStream, "mul $v0, $s0, $v0\n"); break;
            case OVER: fprintf(codeStream, "div $v0, $s0, $v0\n"); break;
            default: DONT_OCCUR_PRINT;
            }
          break;
        }

        case ArrayK:
        {
          if(localCodeGen(t->attr.arr.arr_expr, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "li $s0, %lu\n", sizeof(int));
          fprintf(codeStream, "mul $s0, $v0, $s0\n");
          if(localCodeGen(t->attr.arr._var, codeStream, currStack) != currStack)
            DONT_OCCUR_PRINT;
          fprintf(codeStream, "add $v0, $v0, $s0\n");
          fprintf(codeStream, "lw $v0, 0($v0)\n");
          break;
        }
        case CallK:
        {
          int accLoc = 0, i;
          TreeNode *expr;
          if (!strcmp(t->attr.call._var->attr.ID, "input"))
            {
              // print "input : "
              fprintf(codeStream, "\n# input\n");
              fprintf(codeStream, "li $v0, 4\n");
              fprintf(codeStream, "la $a0, input_str\n");
              fprintf(codeStream, "syscall\n");
              // read_int
              fprintf(codeStream, "li $v0, 5\n");
              fprintf(codeStream, "syscall\n");
            }
          else if (!strcmp(t->attr.call._var->attr.ID, "output"))
            {
              // print "output : "
              fprintf(codeStream, "\n# output\n");
              fprintf(codeStream, "move $t0, $v0\n");
              fprintf(codeStream, "li $v0, 4\n");
              fprintf(codeStream, "la $a0, output_str\n");
              fprintf(codeStream, "syscall\n");
              fprintf(codeStream, "move $v0, $t0\n");
              // print_int
              if (localCodeGen(t->attr.call.expr_list, codeStream, currStack + accLoc) != (currStack + accLoc))
                DONT_OCCUR_PRINT;
              fprintf(codeStream, "move $a0, $v0\n"); // the argument
              fprintf(codeStream, "li $v0, 1\n");
              fprintf(codeStream, "syscall\n");
              // print newline
              fprintf(codeStream, "li $v0, 4\n");
              fprintf(codeStream, "la $a0, newline\n");
              fprintf(codeStream, "syscall\n");
            }
          else
            {
              for(expr = t->attr.call.expr_list, i = 0;
                  expr != NULL;
                  expr = expr->sibling, i++)
                {
                  int size;
                  switch(t->attr.call._var->symbolInfo->attr.funcInfo.paramTypeList[i])
                    {
                    case IntT: size = sizeof(int); break;
                    case IntArrayT: size = regSize; break;
                    default: DONT_OCCUR_PRINT;
                    }

                  if(localCodeGen(expr, codeStream, currStack + accLoc) != (currStack + accLoc))
                    DONT_OCCUR_PRINT;

                  fprintf(codeStream, "addiu $sp, $sp, %d\n", -size);
                  fprintf(codeStream, "sw $v0, 0($sp)\n");
                  accLoc += size;
                }
              fprintf(codeStream, "jal %s\n", t->attr.call._var->attr.ID);
              fprintf(codeStream, "addiu $sp, $sp, %d\n", accLoc);
            }
          break;
        }

        case VariableK:
        {
          switch(t->symbolInfo->nodeType)
            {
            case IntT:
              fprintf(codeStream,
                      t->symbolInfo->attr.intInfo.globalFlag ? "lw $v0, %d\n" : "lw $v0, %d($fp)\n",
                      t->symbolInfo->attr.intInfo.memloc);
              break;
            case IntArrayT:
              fprintf(codeStream,
                      t->symbolInfo->attr.arrInfo.globalFlag ? "li $v0, %d\n" : "addiu $v0, $fp, %d\n",
                      t->symbolInfo->attr.intInfo.memloc);
              break;
            default:
              DONT_OCCUR_PRINT;
            }
          break;
        }
        case ConstantK:
        {
          fprintf(codeStream, "li $v0, %d\n", t->attr.TOK);
          break;
        }

        default:
          DONT_OCCUR_PRINT;
        }
    }
  return currStack;
}
