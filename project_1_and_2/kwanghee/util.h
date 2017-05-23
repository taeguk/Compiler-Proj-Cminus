/****************************************************/
/* File: util.h                                     */
/* Utility functions for the C- compiler            */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/* Modified by Kwanghee Choi                        */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

TreeNode* addSibling(TreeNode *, TreeNode *);
TreeNode* allocateTreeNode(void);
int TokenTypeChecker(TokenType);
int NodeKindChecker(TreeNode *, NodeKind);

TreeNode *newVariableDeclarationNode(TreeNode *, TreeNode *);
TreeNode *newArrayDeclarationNode(TreeNode *, TreeNode *, TreeNode *);
TreeNode *newFunctionDeclarationNode(TreeNode *, TreeNode *, TreeNode *, TreeNode *);

TreeNode *newVariableParameterNode(TreeNode *, TreeNode *);
TreeNode *newArrayParameterNode(TreeNode *, TreeNode *);

TreeNode *newCompoundStatementNode(TreeNode *, TreeNode *);
TreeNode *newExpressionStatementNode(TreeNode *);
TreeNode *newSelectionStatementNode(TreeNode *, TreeNode *, TreeNode *);
TreeNode *newIterationStatementNode(TreeNode *, TreeNode *);
TreeNode *newReturnStatementNode(TreeNode *);

TreeNode *newAssignExpressionNode(TreeNode *, TreeNode *);
TreeNode *newComparisonExpressionNode(TreeNode *, TreeNode *, TreeNode *);
TreeNode *newAdditiveExpressionNode(TreeNode *, TreeNode *, TreeNode *);
TreeNode *newMultiplicativeExpressionNode(TreeNode *, TreeNode *, TreeNode *);

TreeNode *newVariableNode(char *);
TreeNode *newArrayNode(TreeNode *, TreeNode *);
TreeNode *newCallNode(TreeNode *, TreeNode *);
TreeNode *newConstantNode(char *);
TreeNode *newTokenTypeNode(TokenType);

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken(TokenType, const char*);

/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char* copyString(char*);

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree(TreeNode*);

#endif
