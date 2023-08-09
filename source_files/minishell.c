/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/08/01 11:27:00 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/08 11:18:41 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "utils/signals.h"
#include "utils/readline.h"
#include <readline/readline.h>
#include <readline/history.h>

#include "libft.h"
#include "utils/minishell_colors.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/status.h"
#include "parser/parser.h"
#include "executor/execute.h"
#include "env/she_env.h"
#include "expansion/expansion.h"
#include "utils/exitcode.h"

#include "executor/exec_internal.h"
#include "builtins/builtins.h"

static int	we_are_done_and_want_to_continue_with_our_lives(
		t_token *tokens,
		t_ast_node *tree
	)
{
	const t_response	expanded_tree = expand_tree((t_ast_node **)&(tree));
	t_response			execution;

	if (expanded_tree.status != SUCCESS)
	{
		manage_error(expanded_tree.status, expanded_tree.data);
		if (tree)
			tree = free_tree_and_non_source_tokens(tree, LEXER);
		token_free_list(tokens);
		return (expanded_tree.status);
	}
	tree = expanded_tree.data;
	execution = execute(tree);
	if (execution.status != SUCCESS)
		manage_error(execution.status, execution.data);
	clean_pids();
	tree = free_tree_and_non_source_tokens(tree, LEXER);
	token_free_list(tokens);
	return (SUCCESS);
}

int	minishell(char *line)
{
	const t_response	lex_response = lex_input(line);
	t_token				*tokens;
	t_response			tree;

	if (lex_response.status != SUCCESS || lex_response.data == NULL)
	{
		store_exit_code(258);
		return (manage_error(lex_response.status, lex_response.data));
	}
	tokens = lex_response.data;
	tree = parse_pipe_sequence(&tokens);
	if (tree.status != SUCCESS)
	{
		store_exit_code(258);
		manage_error(tree.status, tokens);
		token_free_list(lex_response.data);
		return (tree.status);
	}
	return (we_are_done_and_want_to_continue_with_our_lives(lex_response.data,
			tree.data));
}

void	cleanup(void)
{
	clean_pids();
	sheenv_destroy();
	clear_history();
	rl_clear_history();
}

int	main(int argc, char const *argv[], char const *env[])
{
	char	*line;

	if (sheenv_init(env).status != SUCCESS)
		error_and_exit("env", "failed to initialise the environment", 1);
	line = NULL;
	while (get_line(&line))
	{
		if (line && *line != '\0')
			minishell(line);
	}
	cleanup();
	if (line == NULL)
		return (handle_ctrld());
	(void)argc;
	(void)argv;
	return (0);
}
