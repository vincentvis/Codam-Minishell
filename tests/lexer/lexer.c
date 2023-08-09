#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "../include/utils/status.h"
#include "../include/lexer/lexer.h"
#include "../include/lexer/state.h"
#include "../include/lexer/token.h"
#include "../include/lexer/validation.h"

#include "../generic_test_helpers.h"

#define _PT_(p,t) {.part = p, .type = t}
struct lex_input_output
{
	char			*part;
	t_token_type	type;
};


Test(lexer, fail_on_null_input, .signal = SIGSEGV)
{
	t_response	fail = lex_input(NULL);
	cr_expect_null(fail.data);
}

Test(lexer, only_whitespace)
{
	t_response	response = lex_input(__space __other_whitespace __space);
	cr_expect(response.status == SUCCESS);
	cr_expect_null(response.data);
}

Test(lexer, simple_one_word)
{
	#define part1 "echo"
	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_1);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
}

Test(lexer, simple_leading_whitespace)
{
	#define part1 "echo"
	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(__space  __other_whitespace __space PART_TO_STR_1);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
}

Test(lexer, simple_trailing_whitespace)
{
	#define part1 "echo"
	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_1 __space __other_whitespace __space);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
}

Test(lexer, simple_two_words)
{
	#define part1 "echo"
	#define part2 "text"
	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, simple_multi_whitespace_between_words)
{
	#define part1 "echo"
	#define part2 "text"
	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(part1 __space __other_whitespace __space part2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, simple_input_redirect)
{
	#define part1 "<"
	#define part2 "outfile"
	struct lex_input_output test_io[] = {
		_PT_(part1, LESS_TYPE),
		_PT_(part2, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, simple_output_redirect)
{
	#define part1 ">"
	#define part2 "outfile"
	struct lex_input_output test_io[] = {
		_PT_(part1, GREAT_TYPE),
		_PT_(part2, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, simple_heredoc)
{
	#define part1 "<<"
	#define part2 "DELIM"
	struct lex_input_output test_io[] = {
		_PT_(part1, DLESS_TYPE),
		_PT_(part2, HERE_EOF_EXP_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, heredoc_no_expand)
{
	#define part1 "<<"
	#define part2 "'DE'LIM"
	struct lex_input_output test_io[] = {
		_PT_(part1, DLESS_TYPE),
		_PT_(part2, HERE_EOF_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_2);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0,
			"%.*s != %s",
			(int)token->length, token->content.raw_word, test_io[test_io_iterator].part);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
}

Test(lexer, double_heredoc)
{
	#define part1 "<<"
	#define part2 "DELIM"
	#define part3 "<<"
	#define part4 "QUOTE'D'_DELIM"
	struct lex_input_output test_io[] = {
		_PT_(part1, DLESS_TYPE),
		_PT_(part2, HERE_EOF_EXP_TYPE),
		_PT_(part3, DLESS_TYPE),
		_PT_(part4, HERE_EOF_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_4);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);
	#undef part1
	#undef part2
	#undef part3
	#undef part4
}

Test(lexer, complex_statement)
{
	#define part1 ">"
	#define part2 "out1"
	#define part3 "<<"
	#define part4 "EOF"
	#define part5 "cat"
	#define part6 "|"
	#define part7 "grep"
	#define part8 "$QUERY"
	#define part9 ">>"
	#define part10 "found.txt"

	// TODO fomr here
	struct lex_input_output test_io[] = {
		_PT_(part1, GREAT_TYPE),
		_PT_(part2, WORD_TYPE),
		_PT_(part3, DLESS_TYPE),
		_PT_(part4, HERE_EOF_EXP_TYPE),
		_PT_(part5, WORD_TYPE),
		_PT_(part6, PIPE_TYPE),
		_PT_(part7, WORD_TYPE),
		_PT_(part8, WORD_TYPE),
		_PT_(part9, DGREAT_TYPE),
		_PT_(part10, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_10);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
	#undef part6
	#undef part7
	#undef part8
	#undef part9
	#undef part10
}

Test(lexer, io_number_no_spaces)
{
	#define part1 "2"
	#define part2 ">"
	#define part3 "stderr_redir"
	#define part4 "cat"
	#define part5 "1"
	#define part6 ">"
	#define part7 "stdout_redir"

	struct lex_input_output test_io[] = {
		_PT_(part1, IO_NUMBER_TYPE),
		_PT_(part2, GREAT_TYPE),
		_PT_(part3, WORD_TYPE),
		_PT_(part4, WORD_TYPE),
		_PT_(part5, IO_NUMBER_TYPE),
		_PT_(part6, GREAT_TYPE),
		_PT_(part7, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(part1 part2 part3 __space part4 __space part5 part6 __space part7);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
	#undef part6
	#undef part7
}

Test(lexer, io_number_space_after_io_number)
{
	#define part1 "2" // should be word type because of space
	#define part2 ">"
	#define part3 "stderr_redir"
	#define part4 "cat"
	#define part5 "1" // should be word type because of space
	#define part6 ">"
	#define part7 "stdout_redir"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, GREAT_TYPE),
		_PT_(part3, WORD_TYPE),
		_PT_(part4, WORD_TYPE),
		_PT_(part5, WORD_TYPE),
		_PT_(part6, GREAT_TYPE),
		_PT_(part7, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_7);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
	#undef part6
	#undef part7
}

Test(lexer, invalid_two_pipes)
{
	#define part1 "echo"
	#define part2 "|"
	#define part3 "|"
	#define part4 "cat"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, PIPE_TYPE),
		_PT_(part3, PIPE_TYPE),
		_PT_(part4, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_4);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
}

Test(lexer, invalid_double_less)
{
	#define part1 "echo"
	#define part2 ">"
	#define part3 ">"
	#define part4 "cat"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, GREAT_TYPE),
		_PT_(part3, GREAT_TYPE),
		_PT_(part4, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_4);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
}

Test(lexer, double_and_single_quotes)
{
	#define part1 "echo"
	#define part2 "\"dquotes\""
	#define part3 "'squotes'"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, WORD_TYPE),
		_PT_(part3, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_3);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
}

Test(lexer, double_and_single_quotes_together)
{
	#define part1 "echo"
	#define part2 "\"-\"\"n\""
	#define part3 "'te''st'"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, WORD_TYPE),
		_PT_(part3, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(PART_TO_STR_3);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
}

Test(lexer, invalid_unclosed_double_quote)
{
	#define part1 "echo"
	#define part2 "\"unclosed double quote"

	t_response	response = lex_input(PART_TO_STR_2);

	cr_expect(response.status == L_BAD_INPUT);
	cr_expect_null(response.data);

	#undef part1
	#undef part2
}

Test(lexer, invalid_unclosed_single_quote)
{
	#define part1 "echo"
	#define part2 "'unclosed double quote"

	t_response	response = lex_input(PART_TO_STR_2);

	cr_expect(response.status == L_BAD_INPUT);
	cr_expect_null(response.data);

	#undef part1
	#undef part2
}

Test(lexer, redirect_filename_chain_no_spaces)
{
	// cat <file1>file2
	// should put output of file1 into file2
	#define part1 "cat"
	#define part2 "<"
	#define part3 "file1"
	#define part4 ">"
	#define part5 "file2"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, LESS_TYPE),
		_PT_(part3, WORD_TYPE),
		_PT_(part4, GREAT_TYPE),
		_PT_(part5, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(part1 part2 part3 part4 part5);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
}

Test(lexer, redirect_filename_is_number_with_space)
{
	// cat <2 >file2
	// should put output of file1 into file2
	#define part1 "cat"
	#define part2 "<"
	#define part3 "2"
	#define part4 ">"
	#define part5 "file2"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, LESS_TYPE),
		_PT_(part3, WORD_TYPE),
		_PT_(part4, GREAT_TYPE),
		_PT_(part5, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(part1 part2 part3 __space part4 part5);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
}

Test(lexer, redirect_filename_is_number_without_space)
{
	// cat <2>file2
	// should PARSE to a syntax error, but lexer should be fine with it
	#define part1 "cat"
	#define part2 "<"
	#define part3 "2"
	#define part4 ">"
	#define part5 "file2"

	struct lex_input_output test_io[] = {
		_PT_(part1, WORD_TYPE),
		_PT_(part2, LESS_TYPE),
		_PT_(part3, IO_NUMBER_TYPE),
		_PT_(part4, GREAT_TYPE),
		_PT_(part5, WORD_TYPE),
		{.part = NULL}
	};
	int expected_size = (sizeof(test_io) / sizeof(struct lex_input_output)) - 1;
	int	test_io_iterator = 0;

	t_response	response = lex_input(part1 __space part2 part3 part4 part5);
	cr_expect(response.status == SUCCESS);
	cr_expect_not_null(response.data);

	t_token		*token = response.data;
	while (token)
	{
		cr_expect_not_null(token);
		cr_expect(memcmp(token->content.raw_word, test_io[test_io_iterator].part, token->length) == 0);
		cr_expect(token->type == test_io[test_io_iterator].type);
		test_io_iterator++;
		token = token->next;
	}
	cr_expect_null(token);
	cr_expect(test_io_iterator == expected_size);

	token_free_list(response.data);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
}
