#include "ft_ls.h"



/* Set the flag corresponding to the option */
static void		set_flag(char c, t_flags *flags)
{
	if (c == 'a')
		flags->a = 1;
	if (c == 'r')
		flags->r = 1;
	if (c == 't')
		flags->t = 1;
	if (c == 'l')
		flags->l = 1;
	if (c == 'R')
		flags->R = 1;
}

static void		parse_option_string(const char *opt_str, t_flags *flags)
{
	int	j;

	j = 1;
	while (opt_str[j] != '\0')
	{
		if (!is_allowed_option(opt_str[j]))
		{
			print_invalid_option(opt_str[j]);
			exit(2);
		}
		set_flag(opt_str[j], flags);
		j++;
	}
}

/* Parse all command line arguments */
void			parse_args(int argc, char **argv, t_flags *flags)
{
	int	i;

	flags->a = 0;
	flags->r = 0;
	flags->t = 0;
	flags->l = 0;
	flags->R = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i] && argv[i][0] == '-' && argv[i][1] == '-'
			&& argv[i][2] == '\0')
			break;
		if (is_option(argv[i]))
			parse_option_string(argv[i], flags);
		i++;
	}
}