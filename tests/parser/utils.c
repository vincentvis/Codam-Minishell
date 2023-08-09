#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include "utils/status.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/validation.h"

Test(parser_utils, parse_word_ok){
	t_response				tokens = lex_input("hello world");
	const t_token * const	head = tokens.data;
	const t_response		tree_resp = parse_token_type((t_token **)&tokens.data, is_word, REQUIRED);
	const t_ast_node		*tree = tree_resp.data;

	cr_assert_str_eq(((t_token *)tokens.data)->content.raw_word, "world");
	cr_assert_eq(tree_resp.status, SUCCESS);

	cr_assert_not_null(tree);
	cr_assert_eq(head, tree->token);
	cr_assert_null(tree->left);
	cr_assert_null(tree->right);
}

Test(parser_utils, parse_word_ko){
	const t_response		tokens = lex_input("| world");
	const t_token * const	head = tokens.data;
	const t_response		tree_resp = parse_token_type((t_token **)&tokens.data, is_word, REQUIRED);

	cr_assert_eq(tree_resp.status, P_BAD_INPUT);
	cr_assert_null(tree_resp.data);
	cr_assert_str_eq(((t_token *)tokens.data)->content.raw_word, "| world");
}

Test(parser_utils, null_validator){
	const t_response		tokens = lex_input("| world");
	const t_token * const	head = tokens.data;
	const t_response		tree_resp = parse_token_type((t_token **)&tokens.data, NULL, REQUIRED);
	const t_ast_node		*tree = tree_resp.data;

	cr_assert_str_eq(((t_token *)tokens.data)->content.raw_word, "world");
	cr_assert_eq(tree_resp.status, SUCCESS);

	cr_assert_not_null(tree);
	cr_assert_eq(head, tree->token);
	cr_assert_null(tree->left);
	cr_assert_null(tree->right);
}
