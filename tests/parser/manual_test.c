// #include "criterion/criterion.h"
#include <stdio.h>
#include "parser/parser.h"
#include "lexer/lexer.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/status.h"

void	print_ast(t_ast_node *tree, int indent, char *extra)
{
	int	i = 0;

	if (!tree)
		return ;
	if (tree->right)
		print_ast(tree->right, indent + 1, "┌");
	while (i < indent - 1)
	{
		printf("\t");
		i++;
	}
	if (indent != 0)
		printf("%s--- ", extra);
	if (tree->token)
		printf("%.*s\n", (int)tree->token->length, tree->token->content.raw_word);
	if (tree->left)
		print_ast(tree->left, indent + 1, "└");
}

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

int	main(void)
{
	// const t_response	tokens = lex_input(">>redirection 2> > >'''redir helllo world world world orororor > edirection <more| <<redirection pipe on a pipe or the pipe not sure >>bye | more pipe ");
	printf("\n\n______________________\n");
	
	// const t_response	tokens = lex_input("2<Makefile cat >file | |<<here cat");
	const t_response	tokens = lex_input("2<");

	// printf("Status: %i, \n", tokens.status);
	// if (tokens.status == 0)
		print_tokens(tokens.data);
	const t_response	ast = parse_pipe_sequence(&tokens.data);
	if (ast.status != SUCCESS)
		printf("bad\n");
	printf("%p\n", ast.data);
	print_ast(ast.data, 0, "");
}
