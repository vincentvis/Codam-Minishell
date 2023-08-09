#ifndef GENERIC_TEST_HELPERS_H
# define GENERIC_TEST_HELPERS_H

#include "../include/parser/parser.h"

#define __space " "
#define PART_TO_STR_1 part1
#define PART_TO_STR_2 PART_TO_STR_1 __space part2
#define PART_TO_STR_3 PART_TO_STR_2 __space part3
#define PART_TO_STR_4 PART_TO_STR_3 __space part4
#define PART_TO_STR_5 PART_TO_STR_4 __space part5
#define PART_TO_STR_6 PART_TO_STR_5 __space part6
#define PART_TO_STR_7 PART_TO_STR_6 __space part7
#define PART_TO_STR_8 PART_TO_STR_7 __space part8
#define PART_TO_STR_9 PART_TO_STR_8 __space part9
#define PART_TO_STR_10 PART_TO_STR_9 __space part10
#define PART_TO_STR_11 PART_TO_STR_10 __space part11
#define PART_TO_STR_12 PART_TO_STR_11 __space part12

/*
	FOR LEXER
 */
#define __other_whitespace "\t\v\f\r\n"
// creates a part of the lex_input_output struct

const char *get_token_type_str(int type);
const char	*debug_token_type_from_enum(int type);
const char	*debug_state_type_from_enum(int state);
const char	*debug_status_code_from_enum(int status);
void		make_sure_env_is_empty(void);
void		setup_empty_env(void);
t_response	sheenv_unset_from_input(const char *input);

#endif
