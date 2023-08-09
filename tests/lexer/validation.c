#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdbool.h>

#include "../include/lexer/validation.h"

Test(validation, ispipe)
{
	cr_assert(ispipe('|'));
}

Test(validation, is_not_pipe)
{
	int	c = 0;
	while (c < 256)
	{
		if (c == '|')
			c++;
		cr_assert_not(ispipe(c));
		c++;
	}
}

Test(validation, isleftchevron)
{
	cr_assert(isleftchevron('<'));
}

Test(validation, is_not_leftchevron)
{
	int	c = 0;
	while (c < 256)
	{
		if (c == '<')
			c++;
		cr_assert_not(isleftchevron(c));
		c++;
	}
}

Test(validation, isrightchevron)
{
	cr_assert(isrightchevron('>'));
}

Test(validation, is_not_rightchevron)
{
	int	c = 0;
	while (c < 256)
	{
		if (c == '>')
			c++;
		cr_assert_not(isrightchevron(c));
		c++;
	}
}

Test(validation, isquote)
{
	cr_assert(isquote('\''));
}

Test(validation, is_not_quote)
{
	int	c = 0;
	while (c < 256)
	{
		if (c == '\'')
			c++;
		cr_assert_not(isquote(c));
		c++;
	}
}

Test(validation, isdquote)
{
	cr_assert(isdquote('"'));
}

Test(validation, is_not_dquote)
{
	int	c = 0;
	while (c < 256)
	{
		if (c == '"')
			c++;
		cr_assert_not(isdquote(c));
		c++;
	}
}

Test(validation, isinsidequote)
{
	cr_assert_not(isinsidequote('\''));
	cr_assert_not(isinsidequote(0));
}

Test(validation, is_not_insidequote)
{
	int	c = 1;
	while (c < 256)
	{
		if (c == '\'')
			c++;
		cr_assert(isinsidequote(c));
		c++;
	}
}

Test(validation, isinsidedquote)
{
	cr_assert_not(isinsidedquote('"'));
	cr_assert_not(isinsidedquote(0));
}

Test(validation, is_not_insidedquote)
{
	int	c = 1;
	while (c < 256)
	{
		if (c == '"')
			c++;
		cr_assert(isinsidedquote(c));
		c++;
	}
}

Test(validation, is_not_word)
{
	const char		not_word_chars[] = " \t\n\v\f\r<>|'\"";
	const size_t	len = strlen(not_word_chars) + 1;
	size_t			i = 0;

	while (i < len)
	{
		cr_assert_not(isword(not_word_chars[i]));
		i++;
	}
}

Test(validation, isword)
{
	int	c = 1;
	const char		not_word_chars[] = " \t\n\v\f\r<>|'\"";
	const size_t	len = strlen(not_word_chars);

	while (c < 256)
	{
		while (memchr(not_word_chars, c, len) != NULL)
			c++;
		cr_assert(isword(c));
		c++;
	}
}
