
#ifndef PARSER_HELPERS_H
# define PARSER_HELPERS_H


#include "../include/utils/status.h"
#include "../include/lexer/lexer.h"
#include "../include/parser/parser.h"
#include "../include/parser/validation.h"
#include "../include/parser/parser.h"

/*
	FOR PARSER
 */

#define EXPECT_NODE(exp_type, exp_part, leaf_left, leaf_right) {.token = fake_token(exp_type, exp_part), .left = leaf_left, .right = leaf_right},

typedef struct ast_checking_node {
	t_token	token;
	struct ast_checking_node *left;
	struct ast_checking_node *right;
}	t_ast_checking_node;


typedef struct check_pair
{
	t_ast_node	*actual;
	t_ast_checking_node	*expected;
}	t_check_pair;


// some weird method of doing this non-recurively.. so it can be in a macro
// if we do it in a function we don't get the line-number
#define SETUP_FOR_TREE_TEST(HEAD) t_token	*lexed_tokens = lex_response.data; \
	t_response tree = parse_pipe_sequence(&lexed_tokens); \
	cr_assert(tree.status == SUCCESS); \
	cr_assert_not_null(tree.data); \
	t_ast_node	*actual = tree.data; \
	t_ast_checking_node	*expected = &expect[HEAD];

t_token fake_token(int type, char *string);
void	test_print_ast(t_ast_node *tree, int indent, char *extra);
void	test_print_check_ast(t_ast_checking_node *tree, int indent, char *extra);
void	print_asts(t_ast_node *actual, t_ast_checking_node *expected);
void	compare_trees(t_ast_node *actual, t_ast_checking_node *expected, const char *caller, int line);

#endif
