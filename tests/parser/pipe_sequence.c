#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "utils/status.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "test_utils.h"

// Test(parser, command_only_cmd_prefix)
// {
// 	const t_response	response = lex_input("< infile > outfile");
// 	t_response			command;

// 	cr_assert(response.status == SUCCESS);

// 	command = parse_command(response.data);
// 	cr_assert(command.status == SUCCESS);
// }

Test(parser, pipe_sequence_autoish)
{
	const t_response	response = lex_input("echo | wc");
	t_response			pipe_seq;

	cr_assert(response.status == SUCCESS);

	pipe_seq = parse_pipe_sequence(response.data);
	// cr_assert(pipe_seq.status == SUCCESS);

	t_token left_token = {.type = word_type, .content.raw_word = "echo", .length = 4 };
	t_ast_node left = {
		.token = &left_token,
		.left = NULL,
		.right = NULL
	};

	t_token right_token = { .type = word_type, .content.raw_word = "wc", .length = 2 };
	t_ast_node right = {
		.token = &right_token,
		.left = NULL,
		.right = NULL
	};

	t_token head_token = { .type = pipe_type, .content.raw_word = "|", .length = 1 };
	t_ast_node expected_tree = {
		.token = &head_token,
		.left = &left,
		.right = &right
	};

	// compare_trees((t_ast_node *)pipe_seq.data, &expected_tree);
}

// Test(parser, pipe_sequence)
// {
// 	const t_response	response = lex_input("echo | wc");
// 	t_response			pipe_seq;
// 	t_ast_node			*tree_head;

// 	cr_assert(response.status == SUCCESS);

// 	pipe_seq = parse_pipe_sequence(response.data);
// 	cr_assert(pipe_seq.status == SUCCESS);

// 	// assert that what we get back is a tree.. but how to check?
// 	// manually?...
// 	tree_head = ((t_ast_node *)pipe_seq.data);
// 	cr_assert(is_pipe(tree_head->token->type));

// 	cr_assert(is_word(tree_head->left->token->type));
// 	cr_assert_null(tree_head->left->left);
// 	cr_assert_null(tree_head->left->right);

// 	cr_assert(is_word(tree_head->right->token->type));
// 	cr_assert_null(tree_head->right->left);
// 	cr_assert_null(tree_head->right->right);
// }
