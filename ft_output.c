#include "ft_ls.h"

/* Sort an array of directory paths alphabetically  */
static void	sort_dir_paths(char **dirs, int dir_count)
{
	int i;
	int j;

	i = 0;
	while (i < dir_count - 1)
	{
		j = 0;
		while (j < dir_count - 1 - i)
		{
			if (ft_strcmp(dirs[j], dirs[j + 1]) > 0)
			{
				char *tmp = dirs[j];
				dirs[j] = dirs[j + 1];
				dirs[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*
 * Display the files section (operands that are files)
 * Sorts files according to flags, then displays them
 * show_total = 0 because we don't display "total" for individual files
 */
void	print_files_section(t_entry *files, int file_count, t_flags *flags)
{
	if (file_count <= 0)
		return;
	sort_entries(files, file_count, flags->t);
	display_entries(files, file_count, flags, ".", 0);
}

/*
 * Display directory sections (operands that are directories)
 * Sorts directories alphabetically, then lists each one
 * Displays a "path:" header if multiple operands or if there are files
 * Adds line breaks between directories
 */
void	print_dirs_sections(char **dirs, int dir_count, int had_files,
	t_flags *flags)
{
	int i;
	int printed_any_dir;
	int multi;

	i = 0;
	printed_any_dir = 0;
	multi = (had_files || dir_count > 1);
	sort_dir_paths(dirs, dir_count);
	while (i < dir_count)
	{
		if (printed_any_dir || had_files)
			write(1, "\n", 1);
		if (multi)
		{
			write(1, dirs[i], ft_strlen(dirs[i]));
			write(1, ":\n", 2);
		}
		list_files(dirs[i], flags);
		printed_any_dir = 1;
		i++;
	}
}
