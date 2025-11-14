#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Counts files, collects entries, sorts, displays, and handles recursion if -R */
void		list_files(const char *path, t_flags *flags)
{
	t_entry	*entries;
	int		count;
	int		show_hidden;

	show_hidden = flags->a;
	count = count_files(path, show_hidden);
	if (count == 0)
		return;
	entries = (t_entry *)malloc(sizeof(t_entry) * count);
	if (entries == NULL)
		return;
	if (!collect_entries(path, entries, count, show_hidden))
	{
		free(entries);
		return;
	}
	sort_entries(entries, count, flags->t);
	if (flags->R && ft_strcmp(path, ".") == 0)
		write(1, ".:\n", 3);
	display_entries(entries, count, flags, path, 1);
	if (flags->R)
		process_recursive_dirs(path, entries, count, flags);
	free_entries(entries, count);
}

int			main(int argc, char **argv)
{
	t_flags	flags;
	int		num_operands;

	parse_args(argc, argv, &flags);
	num_operands = count_operands(argc, argv);
	if (num_operands == 0)
	{
		list_files(".", &flags);
		return (0);
	}
	return (handle_all_operands(argc, argv, &flags));
}
