#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Allocate buffers for operands (files and directories) */
static int	allocate_operands_buffers(t_entry **file_entries, char ***dir_paths,
	int argc)
{
	*file_entries = (t_entry *)malloc(sizeof(t_entry) * argc);
	*dir_paths = (char **)malloc(sizeof(char *) * argc);
	if (*file_entries == NULL || *dir_paths == NULL)
	{
		free(*file_entries);
		free(*dir_paths);
		return (1);
	}
	return (0);
}

/*
** Handle all operands: classify, display, and free resources
** Returns error status (1 if errors occurred, 0 otherwise)
*/
static int	handle_all_operands(int argc, char **argv, t_flags *flags)
{
	t_entry	*file_entries;
	char	**dir_paths;
	int		file_count;
	int		dir_count;
	int		had_error;

	if (allocate_operands_buffers(&file_entries, &dir_paths, argc))
		return (1);
	file_count = 0;
	dir_count = 0;
	had_error = 0;
	classify_operands(argc, argv, file_entries, &file_count,
		dir_paths, &dir_count, &had_error);
	print_files_section(file_entries, file_count, flags);
	print_dirs_sections(dir_paths, dir_count, file_count > 0, flags);
	free_operands(file_entries, file_count, dir_paths, dir_count);
	return (had_error ? 1 : 0);
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