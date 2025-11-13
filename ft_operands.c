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

/*
 * Add a file to the files array
 * Duplicates the name and stores metadata (mtime, stat)
 * Returns 1 on success, 0 on error
 */
static int	add_file_entry(t_entry *file_entries, int *file_count,
	const char *path, struct stat *st)
{
	file_entries[*file_count].name = ft_strdup(path);
	if (file_entries[*file_count].name == NULL)
	{
		print_memory_error();
		return (0);
	}
	file_entries[*file_count].mtime = st->st_mtime;
	file_entries[*file_count].st = *st;
	(*file_count)++;
	return (1);
}

/*
 * Add a directory to the directories array
 * Duplicates the directory path
 * Returns 1 on success, 0 on error
 */
static int	add_dir_path(char **dir_paths, int *dir_count, const char *path)
{
	dir_paths[*dir_count] = ft_strdup(path);
	if (dir_paths[*dir_count] == NULL)
	{
		print_memory_error();
		return (0);
	}
	(*dir_count)++;
	return (1);
}

/*
 * Process an operand (file or directory in arguments)
 * Does an lstat to determine type and adds it to the right array
 * If error, stores the path in error_paths array
 * Returns 1 on success, 0 on error (file doesn't exist)
 */
static int	process_operand(const char *path, t_entry *file_entries,
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
	if (S_ISDIR(st.st_mode))
	{
		if (!add_dir_path(dir_paths, dir_count, path))
			*had_error = 1;
	}
	else
	{
		if (!add_file_entry(file_entries, file_count, path, &st))
			*had_error = 1;
	}
	return (1);
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
 * Print all error messages for invalid operands
 * Prints errors in the order they were encountered
 */
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

/*
 * Classify operands into files and directories
 * Iterates through all arguments, ignores options
 * For each operand, determines if it's a file or directory
 * Collects errors first, then processes valid operands
 * Fills file_entries and dir_paths arrays accordingly
 * Note: "--" alone is ignored (means list current directory)
 */
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
			process_operand(argv[i], file_entries, file_count,
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
