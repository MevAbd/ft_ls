#include "ft_ls.h"

/*
 * Check if a string is an option (starts with '-' followed by at least one char)
 * Returns 1 if it's an option, 0 otherwise
 * Note: "--" is not an option, it's treated as a file operand
 */
int	is_option(const char *s)
{
	return (s && s[0] == '-' && s[1] != '\0' && !(s[1] == '-' && s[2] == '\0'));
}

/*
 * Check if a string is "--" (end of options marker)
 * Returns 1 if it's "--", 0 otherwise
 */
static int	is_end_of_options(const char *s)
{
	return (s && s[0] == '-' && s[1] == '-' && s[2] == '\0');
}

/*
 * Count the number of operands (arguments that are not options)
 * Ignores options (starting with '-')
 * Note: "--" alone is ignored (means list current directory)
 */
int	count_operands(int argc, char **argv)
{
	int i;
	int count;
	int found_end_marker;

	i = 1;
	count = 0;
	found_end_marker = 0;
	while (i < argc)
	{
		if (is_end_of_options(argv[i]))
		{
			found_end_marker = 1;
			i++;
			continue;
		}
		if (!is_option(argv[i]))
			count++;
		i++;
	}
	if (found_end_marker && count == 0)
		return (0);
	return (count);
}

/*
 * Print an error message for an invalid option
 * Format: "ls: invalid option -- X\nusage: ls [-lRart] [file ...]\n"
 */
static void	print_invalid_option(char c)
{
	const char *p1 = "ls: invalid option -- ";
	const char *p2 = "\nusage: ls [-lRart] [file ...]\n";

	write(2, p1, ft_strlen(p1));
	write(2, &c, 1);
	write(2, p2, ft_strlen(p2));
}

/*
 * Check if a character is an allowed option
 * Allowed options: l, R, a, r, t
 */
static int	is_allowed_option(char c)
{
	return (c == 'l' || c == 'R' || c == 'a' || c == 'r' || c == 't');
}

/*
 * Set the flag corresponding to the option
 * Sets the appropriate flag to 1 in the flags structure
 */
static void	set_flag(char c, t_flags *flags)
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

/*
 * Parse an option string (e.g. "-lart")
 * Checks each character and sets corresponding flags
 * Exits with code 2 if invalid option found
 */
static void	parse_option_string(const char *opt_str, t_flags *flags)
{
	int j;

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

/*
 * Parse all command line arguments
 * Initializes all flags to 0, then parses each found option
 */
void	parse_args(int argc, char **argv, t_flags *flags)
{
	int i;

	flags->a = 0;
	flags->r = 0;
	flags->t = 0;
	flags->l = 0;
	flags->R = 0;
	i = 1;
	while (i < argc)
	{
		if (is_option(argv[i]))
			parse_option_string(argv[i], flags);
		i++;
	}
}
