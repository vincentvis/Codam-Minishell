#include <criterion/criterion.h>
#include "utils/status.h"
#include "parser/parser.h"
#include "lexer/lexer.h"

Test(parser, pipe_error_double_pipe){
const t_response	tokens = lex_input("hello || bye");
// const t_response	ast = ast_create(tokens.data);

cr_assert_eq(ast.status, L_BAD_INPUT);
}

Test(parser, ok1){
const t_response	tokens = lex_input("hello | bye");
// const t_response	ast = ast_create(tokens.data);

cr_assert_eq(ast.status, SUCCESS);
}



