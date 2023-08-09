#include <criterion/criterion.h>
#include <stdio.h>

#include "../include/utils/status.h"
#include "../include/lexer/lexer.h"
#include "../include/parser/parser.h"
#include "../include/parser/validation.h"

#include "../generic_test_helpers.h"
#include "parser_helpers.h"

Test(parser, simple_input_redirect)
{
	// echo "test" > file
	#define part1 "echo"
	#define part2 "\"test\""
	#define part3 ">"
	#define part4 "file"

	t_response	lex_response = lex_input(PART_TO_STR_4);
	cr_assert(lex_response.status == SUCCESS);

	t_ast_checking_node	expect[4] = {
		// 			type,		string, left node,	right node
		EXPECT_NODE(WORD_TYPE,	part1,	NULL,		&expect[1])
		EXPECT_NODE(WORD_TYPE,	part2,	NULL,		&expect[2])
		EXPECT_NODE(GREAT_TYPE,	part3,	&expect[3],	NULL)
		EXPECT_NODE(WORD_TYPE,	part4,	NULL,		NULL)
	};

	SETUP_FOR_TREE_TEST(0)

	compare_trees(actual, expected, __FUNCTION__, __LINE__);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
}

Test(parser, simple_pipe_sequence)
{
	// echo "test" | wc
	#define part1 "echo"
	#define part2 "\"test\""
	#define part3 "|"
	#define part4 "wc"

	t_response	lex_response = lex_input(PART_TO_STR_4);
	cr_assert(lex_response.status == SUCCESS);

	t_ast_checking_node	expect[4] = {
		// 			type,		string, left node,	right node
		EXPECT_NODE(PIPE_TYPE,	part3,	&expect[1],	&expect[3])
		EXPECT_NODE(WORD_TYPE,	part1,	NULL,		&expect[2])
		EXPECT_NODE(WORD_TYPE,	part2,	NULL,		NULL)
		EXPECT_NODE(WORD_TYPE,	part4,	NULL,		NULL)
	};

	SETUP_FOR_TREE_TEST(0)

	compare_trees(actual, expected, __FUNCTION__, __LINE__);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
}

Test(parser, multi_pipe_sequence)
{
	// base64 < /dev/random | head -c 1000 | cat
	#define part1 "base64"
	#define part2 "<"
	#define part3 "/dev/random"
	#define part4 "|"
	#define part5 "head"
	#define part6 "-c"
	#define part7 "1000"
	#define part8 part4
	#define part9 "cat"

	t_response	lex_response = lex_input(PART_TO_STR_9);
	cr_assert(lex_response.status == SUCCESS);

	// hardcoding the index is optional,
	// but makes it easier to spot which node to add in more complex sequences
	t_ast_checking_node	expect[] = {
		// 			type,		string, left node,			right node
		[0] = EXPECT_NODE(PIPE_TYPE,	part4,	&expect[1],	&expect[4])
		[1] = EXPECT_NODE(WORD_TYPE,	part1,	NULL,		&expect[2])
		[2] = EXPECT_NODE(LESS_TYPE,	part2,	&expect[3],	NULL)
		[3] = EXPECT_NODE(WORD_TYPE,	part3,	NULL, 		NULL)
		[4] = EXPECT_NODE(PIPE_TYPE,	part8,	&expect[5], &expect[8])
		[5] = EXPECT_NODE(WORD_TYPE,	part5,	NULL, 		&expect[6])
		[6] = EXPECT_NODE(WORD_TYPE,	part6,	NULL, 		&expect[7])
		[7] = EXPECT_NODE(WORD_TYPE,	part7,	NULL, 		NULL)
		[8] = EXPECT_NODE(WORD_TYPE,	part9,	NULL, 		NULL)
	};

	SETUP_FOR_TREE_TEST(0)

	compare_trees(actual, expected, __FUNCTION__, __LINE__);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
	#undef part6
	#undef part7
	#undef part8
	#undef part9
}

Test(parser, pipe_double_heredoc)
{
	// << EOF <<EOF2 cat -e | wc
	#define part1 "<<"
	#define part2 "EOF"
	#define part3 part1
	#define part4 "EOF2"
	#define part5 "cat"
	#define part6 "-e"
	#define part7 "|"
	#define part8 "wc"

	t_response	lex_response = lex_input(PART_TO_STR_8);
	cr_assert(lex_response.status == SUCCESS);

	t_ast_checking_node	expect[] = {
		EXPECT_NODE(PIPE_TYPE,	part7, &expect[2], &expect[1])
		EXPECT_NODE(WORD_TYPE,	part8, NULL, NULL)
		EXPECT_NODE(WORD_TYPE,	part5, &expect[4], &expect[3])
		EXPECT_NODE(WORD_TYPE,	part6, NULL, NULL)
		EXPECT_NODE(DLESS_TYPE,	part1, &expect[5], &expect[6])
		EXPECT_NODE(HERE_EOF_EXP_TYPE,	part2, NULL, NULL)
		EXPECT_NODE(DLESS_TYPE,	part3, &expect[7], NULL)
		EXPECT_NODE(HERE_EOF_EXP_TYPE,	part4, NULL, NULL)
	};

	SETUP_FOR_TREE_TEST(0)

	compare_trees(actual, expected, __FUNCTION__, __LINE__);

	#undef part1
	#undef part2
	#undef part3
	#undef part4
	#undef part5
	#undef part6
	#undef part7
	#undef part8
}

Test(parser, multiple_redirections_with_io_num)
{
	// < inp1 < inp2 1>outp1 2>outp2 echo "test"
	#define part1 "<"
	#define part2 "inp1"
	#define part3 "<"
	#define part4 "inp2"
	#define part5 "1"
	#define part6 ">"
	#define part7 "outp1"
	#define part8 "2"
	#define part9 ">"
	#define part10 "outp2"
	#define part11 "echo"
	#define part12 "\"test\""

	t_response	lex_response = lex_input(PART_TO_STR_5 part6 part7 __space part8 part9 part10 __space part11 __space part12);
	cr_assert(lex_response.status == SUCCESS);

	t_ast_checking_node	expect[] = {
		 [0] = EXPECT_NODE(WORD_TYPE, part11, &expect[2], &expect[1])
		 [1] = EXPECT_NODE(WORD_TYPE, part12, NULL, NULL)
		 [2] = EXPECT_NODE(LESS_TYPE, part1, &expect[3], &expect[4])
		 [3] = EXPECT_NODE(WORD_TYPE, part2, NULL, NULL)
		 [4] = EXPECT_NODE(LESS_TYPE, part3, &expect[5], &expect[6])
		 [5] = EXPECT_NODE(WORD_TYPE, part4, NULL, NULL)
		 [6] = EXPECT_NODE(GREAT_TYPE, part6, &expect[7], &expect[9])
		 [7] = EXPECT_NODE(WORD_TYPE, part7, &expect[8], NULL)
		 [8] = EXPECT_NODE(IO_NUMBER_TYPE, part5, NULL, NULL)
		 [9] = EXPECT_NODE(GREAT_TYPE, part9, &expect[10], NULL)
		[10] = EXPECT_NODE(WORD_TYPE, part10, &expect[11], NULL)
		[11] = EXPECT_NODE(IO_NUMBER_TYPE, part8, NULL, NULL)
	};

	SETUP_FOR_TREE_TEST(0)

	compare_trees(actual, expected, __FUNCTION__, __LINE__);

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
	#undef part11
	#undef part12
}
