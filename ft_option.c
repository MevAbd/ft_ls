#include "ft_ls.h"

/* Returns 1 if it's an option */
int	is_option(const char *s)
{
	return (s && s[0] == '-' && s[1] != '\0'
		&& !(s[1] == '-' && s[2] == '\0'));
}

/* Returns 1 if it's "--" (end of options marker) */
int	is_end_of_options(const char *s)
{
	return (s && s[0] == '-' && s[1] == '-' && s[2] == '\0');
}

/* Check if a character is an allowed option */
int	is_allowed_option(char c)
{
	return (c == 'l' || c == 'R' || c == 'a' || c == 'r' || c == 't');
}