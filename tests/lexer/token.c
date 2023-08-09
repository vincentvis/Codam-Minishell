#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "../include/lexer/token.h"
#include "../include/lexer/state.h"

Test(token, determine_state)
{
	cr_expect(token_determine_type(WORD_STATE) == WORD_TYPE);
	cr_expect(token_determine_type(DIGIT_STATE) == IO_NUMBER_TYPE);
	cr_expect(token_determine_type(PIPE_STATE) == PIPE_TYPE);
	cr_expect(token_determine_type(LESS_STATE) == LESS_TYPE);
	cr_expect(token_determine_type(GREAT_STATE) == GREAT_TYPE);
	cr_expect(token_determine_type(DGREAT_STATE) == DGREAT_TYPE);
	cr_expect(token_determine_type(DLESS_STATE) == DLESS_TYPE);
}

Test(token, create_raw_not_allocated)
{
	const char	*content = "token content";
	t_token	*new_token = token_create_raw(WORD_TYPE, content, strlen(content), 0);

	cr_assert_not_null(new_token);
	cr_expect(new_token->type == WORD_TYPE);
	cr_expect(new_token->length == strlen(content));
	cr_expect(new_token->allocated == 0);
	cr_expect(new_token->content.raw_word == content);
	cr_expect(new_token->next == NULL);
	free(new_token);
}

Test(token, create_raw_misc)
{
	t_token	*new_token = token_create_raw(WORD_TYPE, NULL, 0, 1);

	cr_assert_not_null(new_token);
	cr_expect(new_token->type == WORD_TYPE);
	cr_expect(new_token->length == 0);
	cr_expect(new_token->allocated == 1);
	cr_expect(new_token->content.raw_word == NULL);
	cr_expect(new_token->next == NULL);
	free(new_token);
}
