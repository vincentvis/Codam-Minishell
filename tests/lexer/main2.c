#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/status.h"
#include <stdio.h>

void	print_tokens(const t_token *token)
{
	int					i;
	static const char	*types[] = {
		[word_type] = "word",
		[io_number_type] = "number",
		[pipe_type] = "pipe",
		[less_type] = "less",
		[dless_type] = "dless",
		[great_type] = "great",
		[dgreat_type] = "dgreat"
	};

	i = 0;
	while (token)
	{
		printf("Token %i, type: %s. Content: %.*s\n",
			i, types[token->type], (int)token->length,
			token->content.raw_word);
		i++;
		token = token->next;
	}
}

int	main(int argc, char **argv)
{
	t_response	tokens;

	(void)argc;
	if (argc < 2)
		return (-1);
	tokens = lex_input(argv[1]);
	printf("Status: %i\n", tokens.status);
	if (tokens.status == 0)
		print_tokens(tokens.data);
	return (0);
}
