#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/*
 * Main entry point of the program
 *
 * Execution flow:
 * 1. Parse arguments to detect options (-l, -R, -a, -r, -t)
 * 2. Count operands (non-option paths)
 * 3. If no operand: list current directory
 * 4. Otherwise:
 *    - Classify operands into files and directories
 *    - Display files first (sorted)
 *    - Display directories next (with headers if multiple)
 *
 * Returns 0 on success, 1 on minor error
 */
int	main(int argc, char **argv)
{
	t_flags flags;
	int had_error;
	int num_operands;
	t_entry *file_entries;
	char **dir_paths;
	int file_count;
	int dir_count;

	flags.a = 0;
	flags.r = 0;
	flags.t = 0;
	flags.l = 0;
	flags.R = 0;
	had_error = 0;
	parse_args(argc, argv, &flags);
	num_operands = count_operands(argc, argv);
	if (num_operands == 0)
	{
		list_files(".", &flags);
		return (0);
	}
	file_entries = (t_entry *)malloc(sizeof(t_entry) * argc);
	dir_paths = (char **)malloc(sizeof(char *) * argc);
	if (file_entries == NULL || dir_paths == NULL)
	{
		free(file_entries);
		free(dir_paths);
		return (1);
	}
	file_count = 0;
	dir_count = 0;
	classify_operands(argc, argv, file_entries, &file_count,
		dir_paths, &dir_count, &had_error);
	print_files_section(file_entries, file_count, &flags);
	print_dirs_sections(dir_paths, dir_count, file_count > 0, &flags);
	free_operands(file_entries, file_count, dir_paths, dir_count);
	return (had_error ? 1 : 0);
}
