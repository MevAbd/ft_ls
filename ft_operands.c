#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Add a path to the appropriate array (file or directory) */
static int	add_operand_path(const char *path, t_entry *file_entries,
	int *file_count, char **dir_paths, int *dir_count, struct stat *st)
{
	char	*dup_path;

	dup_path = ft_strdup(path);
	if (dup_path == NULL)
	{
		print_memory_error();
		return (0);
	}
	if (st != NULL)
	{
		file_entries[*file_count].name = dup_path;
		file_entries[*file_count].mtime = st->st_mtime;
		file_entries[*file_count].st = *st;
		(*file_count)++;
	}
	else
	{
		dir_paths[*dir_count] = dup_path;
		(*dir_count)++;
	}
	return (1);
}

/* Check if a single path is a file or directory and add it to the appropriate array */
static int	classify_single_operand(const char *path, t_entry *file_entries,
	int *file_count, char **dir_paths, int *dir_count,
	char **error_paths, int *error_count, int *had_error)
{
	struct stat st;

	if (lstat(path, &st) != 0)
	{
		error_paths[*error_count] = (char *)path;
		(*error_count)++;
		*had_error = 1;
		return (0);
	}
	if (!add_operand_path(path, file_entries, file_count, dir_paths, dir_count,
		S_ISDIR(st.st_mode) ? NULL : &st))
		*had_error = 1;
	return (1);
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

/* Handle all operands: classify, display, and free resources */
int	handle_all_operands(int argc, char **argv, t_flags *flags)
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

/* Parse all command-line arguments, ignore options, and classify each operand as file or directory */
void	classify_operands(
	int argc, char **argv,
	t_entry *file_entries, int *file_count,
	char **dir_paths, int *dir_count,
	int *had_error)
{
	int		i;
	int		after_end_marker;
	char	**error_paths;
	int		error_count;

	error_paths = (char **)malloc(sizeof(char *) * argc);
	if (error_paths == NULL)
		return;
	i = 1;
	after_end_marker = 0;
	error_count = 0;
	while (i < argc)
	{
		if (is_end_of_options(argv[i]))
		{
			after_end_marker = 1;
			i++;
			continue;
		}
		if (after_end_marker || !is_option(argv[i]))
		{
			classify_single_operand(argv[i], file_entries, file_count,
				dir_paths, dir_count, error_paths, &error_count, had_error);
		}
		i++;
	}
	print_all_errors(error_paths, error_count);
	free(error_paths);
}
