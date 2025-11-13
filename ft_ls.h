#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <time.h>
# include <errno.h>
# include <string.h>
# include <stddef.h>
# include <pwd.h>
# include <grp.h>

// ============================================================================
// STRUCTURES
// ============================================================================

// Structure for option flags
typedef struct s_flags
{
	int a;  // -a : show all files (including hidden)
	int r;  // -r : reverse order
	int t;  // -t : sort by modification date
	int l;  // -l : long format (details)
	int R;  // -R : recursive (subdirectories)
} t_flags;

// Structure for an entry (file/directory)
typedef struct s_entry
{
	char		*name;      // File/directory name
	time_t		mtime;      // Modification date (for -t sort)
	struct stat	st;         // Complete stat (for -l format)
}	t_entry;

// ============================================================================
// STRING MANIPULATION FUNCTIONS
// ============================================================================
size_t	ft_strlen(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
int		ft_putnbr(long long n, char *buf);
char	*ft_build_full_path(const char *base, const char *name);

// ============================================================================
// PARSING AND ARGUMENT FUNCTIONS
// ============================================================================
int		is_option(const char *s);
int		is_end_of_options(const char *s);
int		count_operands(int argc, char **argv);
void	parse_args(int argc, char **argv, t_flags *flags);

// ============================================================================
// FILE AND DIRECTORY MANAGEMENT FUNCTIONS
// ============================================================================
int		count_files(const char *path, int show_hidden);
int		collect_entries(const char *path, t_entry *entries, int count,
			int show_hidden);
void	sort_entries(t_entry *entries, int count, int use_time);
void	display_entries(t_entry *entries, int count, t_flags *flags,
			const char *path, int show_total);
void	free_entries(t_entry *entries, int count);
void	list_files(const char *path, t_flags *flags);

// ============================================================================
// OPERAND MANAGEMENT FUNCTIONS (files/directories in arguments)
// ============================================================================
void	classify_operands(
			int argc, char **argv,
			t_entry *file_entries, int *file_count,
			char **dir_paths, int *dir_count,
			int *had_error);
void	print_files_section(t_entry *files, int file_count, t_flags *flags);
void	print_dirs_sections(char **dirs, int dir_count, int had_files,
			t_flags *flags);
void	free_operands(t_entry *files, int file_count, char **dirs, int dir_count);

// ============================================================================
// FORMATTING FUNCTIONS
// ============================================================================
void	format_date(time_t mtime, char *date_buf);
void	print_total(t_entry *entries, int count);

// ============================================================================
// INFORMATION RETRIEVAL FUNCTIONS
// ============================================================================
void	get_file_type_char(mode_t mode, char *type);
void	get_permissions(mode_t mode, char *perms);
void	get_user_info(uid_t uid, const char **user_name);
void	get_group_info(gid_t gid, const char **group_name);

#endif
