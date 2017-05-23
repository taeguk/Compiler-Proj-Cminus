/****************************************************/
/* File: util.h                                     */
/* Utility functions for the TINY compiler          */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

/* Procedure printToken prints a token 
 * and its lexeme to the listing file
 */
void printToken( TokenType, const char* );

void print_op_token(TokenType token);

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * new_stmt_node(StmtKind);

/* Function new_exp_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_exp_node(ExpKind);


/* Function new_decl_node creates a new declation
* node for syntax tree construction
*/
TreeNode * new_decl_node(DeclKind kind);

/* Function new_param_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_param_node(ParamKind kind);

/* Function new_type_node creates a new expression 
 * node for syntax tree construction
 */
TreeNode * new_type_node(TypeKind kind);


/* Function copy_string allocates and makes a new
 * copy of an existing string
 */
char * copy_string( char * );

/* procedure printTree prints a syntax tree to the 
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * );

#endif
