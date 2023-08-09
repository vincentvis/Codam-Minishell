#include <criterion/criterion.h>
#include <stdio.h>
#include <string.h>
#include "utils/status.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/validation.h"
#include "test_utils.h"

Test(parser_io_redirect, simple_redir){
const t_response		tokens = lex_input("> redirection");
t_token *				top_token = tokens.data;
const t_response		redirection = parse_io_redirect(&(tokens.data));
// t_ast_node				*tree_head = ((t_ast_node *)redirection.data);

// cr_assert_eq(redirection.status, SUCCESS);
// cr_assert_null(tokens.data);

// t_ast_node	expected_leaf = ast_node_init(top_token->next, NULL, NULL);
// t_ast_node	expected_top = ast_node_init(top_token, &expected_leaf, NULL);
// compare_trees(&expected_top, tree_head);
}
