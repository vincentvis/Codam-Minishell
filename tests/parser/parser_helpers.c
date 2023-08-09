#include <criterion/criterion.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "../generic_test_helpers.h"
#include "parser_helpers.h"

t_token fake_token(int type, char *string)
{
	t_token	token;

	token.type = type;
	token.content.raw_word = string;
	token.length = strlen(string);
	return (token);
}

//
// For debugging creating manual AST's
//
void	test_print_ast(t_ast_node *tree, int indent, char *extra)
{
	int	i = 0;
	if (!tree)
		return ;
	if (tree->right)
		test_print_ast(tree->right, indent + 1, "┌");
	while (i < indent - 1)
	{
		printf("\t");
		i++;
	}
	if (indent != 0)
		printf("%s--- ", extra);
	if (tree->token)
		printf("%.*s\n", (int)tree->token->length,
			tree->token->content.raw_word);
	if (tree->left)
		test_print_ast(tree->left, indent + 1, "└");
}

void	test_print_check_ast(t_ast_checking_node *tree, int indent, char *extra)
{
	int	i = 0;
	if (!tree)
		return ;
	if (tree->right)
		test_print_check_ast(tree->right, indent + 1, "┌");
	while (i < indent - 1)
	{
		printf("\t");
		i++;
	}
	if (indent != 0)
		printf("%s--- ", extra);
	if (tree->token.content.raw_word)
		printf("%.*s\n", (int)tree->token.length,
			tree->token.content.raw_word);
	if (tree->left)
		test_print_check_ast(tree->left, indent + 1, "└");
}

void	print_asts(t_ast_node *actual, t_ast_checking_node *expected)
{
	test_print_ast(actual, 0, "-");
	test_print_check_ast(expected, 0, "-");
}

void	compare_trees(t_ast_node *actual, t_ast_checking_node *expected, const char *caller, int line)
{
	cr_expect_not_null(actual,
		"Expected to have a generated token at this point, got NULL (%s:%i)",
		caller,
		line
	);
	cr_expect_not_null(expected,
		"Expected to have a verify token to check against, got NULL (%s:%i)",
		caller,
		line
	);

	cr_expect_neq((unsigned long)actual, (unsigned long)expected,
		"It seems like you're comparing a tree with nodes that are the same! [%p == %p] (%s:%i)",
		actual,
		expected,
		caller,
		line
	);
	cr_expect(actual->token->type == expected->token.type,
		"Expected type [%s], found type [%s] (%s:%i)",
		get_token_type_str(actual->token->type),
		get_token_type_str(expected->token.type),
		caller,
		line
	);
	cr_expect(memcmp(actual->token->content.raw_word, expected->token.content.raw_word, actual->token->length) == 0,
		"Expected token content [%.*s], found [%.*s] (%s:%i)",
		(int)expected->token.length, expected->token.content.raw_word,
		(int)actual->token->length, actual->token->content.raw_word,
		caller,
		line
	);
	if (expected->left)
	{
		cr_expect_not_null(actual->left,
			"Expected token to have a left node, but token has NULL (%s:%i)",
			caller,
			line
		);
		if (actual->left)
			compare_trees(actual->left, expected->left, caller, line);
	}
	else
	{
		cr_expect_null(actual->left,
			"Expected token to not have a left node, token has [%p] (%s:%i)",
			actual->left,
			caller,
			line
		);
	}
	if (expected->right)
	{
		cr_expect_not_null(actual->right,
			"Expected token to have a right node, but token has NULL (%s:%i)",
			caller,
			line
		);
		if (actual->right)
			compare_trees(actual->right, expected->right, caller, line);
	}
	else
	{
		cr_expect_null(actual->right,
			"Expected token to not have a right node, token has [%p] (%s:%i)",
			actual->right,
			caller,
			line
		);
	}
}
