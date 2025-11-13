#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Add a path to the appropriate array (file or directory) */
static int		add_operand_path(const char *path, t_operands_data *data,
				struct stat *st)
{
	char		*dup_path;

	dup_path = ft_strdup(path);
	if (dup_path == NULL)
	{
		print_memory_error();
		return (0);
	}
	if (st != NULL)
	{
		data->file_entries[data->file_count].name = dup_path;
		data->file_entries[data->file_count].mtime = st->st_mtime;
		data->file_entries[data->file_count].st = *st;
		data->file_count++;
	}
	else
	{
		data->dir_paths[data->dir_count] = dup_path;
		data->dir_count++;
	}
	return (1);
}

/* Check if a single path is a file or directory and add it to the appropriate array */
static int		classify_single_operand(const char *path, t_operands_data *data,
				char **error_paths, int *error_count)
{
	struct stat	st;

	if (lstat(path, &st) != 0)
	{
		error_paths[*error_count] = (char *)path;
		(*error_count)++;
		data->had_error = 1;
		return (0);
	}
	if (!add_operand_path(path, data, S_ISDIR(st.st_mode) ? NULL : &st))
		data->had_error = 1;
	return (1);
}



/* Count the number of operands (arguments that are not options) */
int				count_operands(int argc, char **argv)
{
	int			i;
	int			count;
	int			after_end_marker;

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
static int		allocate_operands_buffers(t_operands_data *data, int argc)
{
	data->file_entries = (t_entry *)malloc(sizeof(t_entry) * argc);
	data->dir_paths = (char **)malloc(sizeof(char *) * argc);
	if (data->file_entries == NULL || data->dir_paths == NULL)
	{
		free(data->file_entries);
		free(data->dir_paths);
		return (1);
	}
	data->file_count = 0;
	data->dir_count = 0;
	data->had_error = 0;
	return (0);
}

/* Handle all operands: classify, display, and free resources */
int				handle_all_operands(int argc, char **argv, t_flags *flags)
{
	t_operands_data	data;

	if (allocate_operands_buffers(&data, argc))
		return (1);
	classify_operands(argc, argv, &data);
	print_files_section(data.file_entries, data.file_count, flags);
	print_dirs_sections(data.dir_paths, data.dir_count, data.file_count > 0, flags);
	free_operands(data.file_entries, data.file_count, data.dir_paths, data.dir_count);
	return (data.had_error ? 1 : 0);
}

/* Parse all command-line arguments, ignore options, and classify each operand as file or directory */
void			classify_operands(int argc, char **argv, t_operands_data *data)
{
	int			i;
	int			after_end_marker;
	char		**error_paths;
	int			error_count;

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
			classify_single_operand(argv[i], data, error_paths, &error_count);
		}
		i++;
	}
	print_all_errors(error_paths, error_count);
	free(error_paths);
}
