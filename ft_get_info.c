#include "ft_ls.h"

/* Determine the file type character according to st_mode */
void	get_file_type_char(mode_t mode, char *type)
{
	if (S_ISREG(mode))
		*type = '-';
	else if (S_ISDIR(mode))
		*type = 'd';
	else if (S_ISLNK(mode))
		*type = 'l';
	else if (S_ISCHR(mode))
		*type = 'c';
	else if (S_ISBLK(mode))
		*type = 'b';
	else if (S_ISFIFO(mode))
		*type = 'p';
	else if (S_ISSOCK(mode))
		*type = 's';
	else
		*type = '?';
}

/* Convert permissions to rwxrwxrwx string */
void	get_permissions(mode_t mode, char *perms)
{
	perms[0] = (mode & S_IRUSR) ? 'r' : '-';
	perms[1] = (mode & S_IWUSR) ? 'w' : '-';
	perms[2] = (mode & S_IXUSR) ? 'x' : '-';
	perms[3] = (mode & S_IRGRP) ? 'r' : '-';
	perms[4] = (mode & S_IWGRP) ? 'w' : '-';
	perms[5] = (mode & S_IXGRP) ? 'x' : '-';
	perms[6] = (mode & S_IROTH) ? 'r' : '-';
	perms[7] = (mode & S_IWOTH) ? 'w' : '-';
	perms[8] = (mode & S_IXOTH) ? 'x' : '-';
	perms[9] = '\0';
}

/* Get username or UID */
void	get_user_info(uid_t uid, const char **user_name)
{
	struct passwd	*pwd;
	static char		uid_buf[32];

	pwd = getpwuid(uid);
	if (pwd)
		*user_name = pwd->pw_name;
	else
	{
		ft_itoa((int)uid, uid_buf);
		*user_name = uid_buf;
	}
}

/* Get group name or GID */
void	get_group_info(gid_t gid, const char **group_name)
{
	struct group	*grp;
	static char		gid_buf[32];

	grp = getgrgid(gid);
	if (grp)
		*group_name = grp->gr_name;
	else
	{
		ft_itoa((int)gid, gid_buf);
		*group_name = gid_buf;
	}
}