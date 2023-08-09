/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.h                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jowytrzy <jowytrzy@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2021/07/15 14:57:09 by jowytrzy      #+#    #+#                 */
/*   Updated: 2022/06/07 17:45:06 by vvissche      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "lexer/token.h"
# include "utils/status.h"

# define REQUIRED 1
# define OPTIONAL 0

typedef struct s_ast_node {
	t_token				*token;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}				t_ast_node;

t_ast_node	*ast_make_node(t_token *content);
t_ast_node	*ast_make_node_and_consume(t_token **tokens);

t_response	parse_token_type(t_token **token, bool (*validator)(t_token_type),
				bool required);

t_response	parse_redirection(t_token **tokens, t_ast_node *left);
t_response	parse_pipe(t_token **tokens, t_ast_node *left);
t_response	parse_argument(t_token **tokens, t_ast_node *left);

t_response	parse_pipe_sequence(t_token **token);
t_response	parse_command(t_token **token);
t_response	parse_command_prefix(t_token **token);
t_response	parse_command_name(t_token **token);
t_response	parse_command_suffix(t_token **token);
t_response	parse_io_redirect(t_token **token);

void		*free_tree(t_ast_node *node);
void		*free_tree_and_non_source_tokens(t_ast_node *node,
				enum e_token_source source);

#endif
