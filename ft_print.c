#include "ft_ls.h"
#include <errno.h>
#include <string.h>

/* Print an error message for an inaccessible file/directory */
void	print_error(const char *path)
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

/* Print an error message for an inaccessible directory */
void	print_dir_error(const char *path)
{
	const char	*p1;
	const char	*p2;
	const char	*msg;

	p1 = "ls: ";
	p2 = ": ";
	msg = strerror(errno);
	write(2, p1, ft_strlen(p1));
	write(2, path, ft_strlen(path));
	write(2, p2, ft_strlen(p2));
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}

/* Print an error message for a memory allocation failure */
void	print_memory_error(void)
{
	const char *msg = "ls: memory allocation failed\n";
	write(2, msg, ft_strlen(msg));
}

/* Print an error message for an invalid option */
void	print_invalid_option(char c)
{
	const char	*p1;
	const char	*p2;

	p1 = "ls: invalid option -- ";
	p2 = "\nusage: ls [-lRart] [file ...]\n";
	write(2, p1, ft_strlen(p1));
	write(2, &c, 1);
	write(2, p2, ft_strlen(p2));
}

/* Print all error messages for invalid operands */
void	print_all_errors(char **error_paths, int error_count)
{
	int	i;

	i = 0;
	while (i < error_count)
	{
		print_error(error_paths[i]);
		i++;
	}
}

/* Display the "total X" line at the start of long format */
void	print_total(t_entry *entries, int count)
{
	long long	total;
	char		total_str[32];
	int			len;
	int			i;

	total = 0;
	i = 0;
	while (i < count)
	{
		total += entries[i].st.st_blocks;
		i++;
	}
	write(1, "total ", 6);
	len = ft_putnbr(total, total_str);
	write(1, total_str, len);
	write(1, "\n", 1);
}

/* Display the files section (operands that are files) */
void	print_files_section(t_entry *files, int file_count, t_flags *flags)
{
	if (file_count <= 0)
		return;
	sort_entries(files, file_count, flags->t);
	display_entries(files, file_count, flags, ".", 0);
}

/* Display directory sections (operands that are directories) */
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