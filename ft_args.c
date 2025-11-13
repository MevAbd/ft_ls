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

/* Count the number of operands (arguments that are not options) */
int	count_operands(int argc, char **argv)
{
	int	i;
	int	count;
	int	after_end_marker;

	i = 1;
	count = 0;
	after_end_marker = 0;
	while (i < argc)
	{
		if (is_end_of_options(argv[i]))
		{
			after_end_marker = 1;
			i++;
			continue;
		}
		if (after_end_marker || !is_option(argv[i]))
			count++;
		i++;
	}
	if (after_end_marker && count == 0)
		return (0);
	return (count);
}