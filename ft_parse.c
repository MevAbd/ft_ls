#include "ft_ls.h"

/* Print an error message for an invalid option */
static void		print_invalid_option(char c)
{
	const char	*p1;
	const char	*p2;

	p1 = "ls: invalid option -- ";
	p2 = "\nusage: ls [-lRart] [file ...]\n";
	write(2, p1, ft_strlen(p1));
	write(2, &c, 1);
	write(2, p2, ft_strlen(p2));
}

/* Check if a character is an allowed option */
static int		is_allowed_option(char c)
{
	return (c == 'l' || c == 'R' || c == 'a' || c == 'r' || c == 't');
}

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