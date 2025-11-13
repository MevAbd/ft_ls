#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/*
 * Print an error message for an inaccessible file/directory
 * Format: "ls: path: error message\n"
 */
static void	print_error(const char *path)
{
	const char *p1 = "ls: ";
	const char *p2 = ": ";
	const char *msg = strerror(errno);

	write(2, p1, ft_strlen(p1));
	write(2, path, ft_strlen(path));
	write(2, p2, ft_strlen(p2));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

/*
 * Print an error message for a memory allocation failure
 */
static void	print_memory_error(void)
{
	const char *msg = "ls: memory allocation failed\n";
	write(2, msg, ft_strlen(msg));
}

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


/* Print all error messages for invalid operands */
static void	print_all_errors(char **error_paths, int error_count)
{
	int	i;

	i = 0;
	while (i < error_count)
	{
		print_error(error_paths[i]);
		i++;
	}
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

/*
 * Free memory allocated for operands
 * Frees file names, then the arrays
 */
void	free_operands(t_entry *files, int file_count, char **dirs, int dir_count)
{
	int i;

	i = 0;
	while (i < file_count)
	{
		free(files[i].name);
		i++;
	}
	free(files);
	i = 0;
	while (i < dir_count)
	{
		free(dirs[i]);
		i++;
	}
	free(dirs);
}
