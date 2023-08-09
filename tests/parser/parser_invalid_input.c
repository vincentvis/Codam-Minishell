#include <criterion/criterion.h>
#include <stdio.h>

#include "../include/utils/status.h"
#include "../include/lexer/lexer.h"
#include "../include/parser/parser.h"
#include "../include/parser/validation.h"

#include "../generic_test_helpers.h"
#include "parser_helpers.h"

Test(parser, start_with_pipe, .disabled = false)
{
	t_response lex_response = lex_input("| cat");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_expect(tree.status == SUCCESS);
	cr_expect_null(tree.data);
}

Test(parser, start_with_double_pipe, .disabled = false)
{
	t_response lex_response = lex_input("| | cat");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_expect(tree.status == SUCCESS);
	cr_expect_null(tree.data);
}

Test(parser, redirect_filename_is_number_without_space, .disabled = false)
{
	// cat <2>file2
	// should PARSE to a syntax error, but lexer should be fine with it

	t_response	lex_response = lex_input("cat <2>file2");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_expect(tree.status == P_BAD_INPUT); // is this the correct status?
	cr_expect_null(tree.data);
}

Test(parser, double_pipe, .disabled = false)
{
	t_response	lex_response = lex_input("echo something | | cat");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_assert(tree.status == P_BAD_INPUT);
	cr_assert_null(tree.data);
}

Test(parser, end_with_pipe, .disabled = false)
{
	t_response lex_response = lex_input("cat |");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_expect(tree.status == P_BAD_INPUT);
	cr_expect_null(tree.data);
}

Test(parser, triple_pipe, .disabled = false)
{
	t_response lex_response = lex_input("cat | | | cat");
	cr_assert(lex_response.status == SUCCESS);

	t_token		*lexed_tokens = lex_response.data;
	t_response	tree = parse_pipe_sequence(&lexed_tokens);

	cr_expect(tree.status == P_BAD_INPUT);
	cr_expect_null(tree.data);
}

Test(parser, end_with_redirect, .disabled = false)
{
	t_response lex_responses[] = {
		lex_input("cat <"),
		lex_input("cat <<"),
		lex_input("cat >"),
		lex_input("cat >>"),
	};
	const int num_tests = 4;
	int i = 0;
	while (i < num_tests)
	{
		cr_assert(lex_responses[i].status == SUCCESS);
		i++;
	}
	t_response	tree_response;
	t_token		*lexed_tokens;
	i = 0;
	while (i < num_tests)
	{
		lexed_tokens = lex_responses[i].data;
		tree_response = parse_pipe_sequence(&lexed_tokens);
		cr_expect(tree_response.status == P_BAD_INPUT);
		cr_expect_null(tree_response.data);
		i++;
	}
}

Test(parser, tripple_redirect_symbol, .disabled = false)
{
	t_response lex_responses[] = {
		lex_input("<<< something"),
		lex_input(">>> something"),
		lex_input("something >>> f"),
		lex_input("something <<< f"),
	};
	const int num_tests = 4;
	int i = 0;
	while (i < num_tests)
	{
		cr_assert(lex_responses[i].status == SUCCESS);
		i++;
	}
	t_response	tree_response;
	t_token		*lexed_tokens;
	i = 0;
	while (i < num_tests)
	{
		lexed_tokens = lex_responses[i].data;
		tree_response = parse_pipe_sequence(&lexed_tokens);
		cr_expect(tree_response.status == P_BAD_INPUT);
		cr_expect_null(tree_response.data);
		i++;
	}
}

Test(parser, double_redir_different_type, .disabled = false)
{
	t_response lex_responses[] = {
		lex_input("echo >< something"),
		lex_input("echo <> something"),
		lex_input("echo <<> something"),
		lex_input("echo <>> something"),
		lex_input("echo >>< something"),
		lex_input("echo ><< something"),
		lex_input("echo >><< something"),
		lex_input("echo <<>> something")
	};
	const int num_tests = 8;
	int i = 0;
	while (i < num_tests)
	{
		cr_assert(lex_responses[i].status == SUCCESS);
		i++;
	}
	t_response	tree_response;
	t_token		*lexed_tokens;
	i = 0;
	while (i < num_tests)
	{
		lexed_tokens = lex_responses[i].data;
		tree_response = parse_pipe_sequence(&lexed_tokens);
		cr_expect(tree_response.status == P_BAD_INPUT);
		cr_expect_null(tree_response.data);
		i++;
	}
}
