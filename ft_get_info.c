#include "ft_ls.h"

/*
 * Determine the file type character according to st_mode
 * Returns: '-' (file), 'd' (directory), 'l' (link), 'c' (character),
 *          'b' (block), 'p' (FIFO), 's' (socket), '?' (unknown)
 */
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

/*
 * Convert permissions (st_mode) to rwxrwxrwx string
 * Fills the perms array with user/group/other permissions
 */
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

/*
 * Get username or UID
 * Uses getpwuid to get the name, otherwise converts UID to string
 * Buffer is static to avoid scope issues
 */
void	get_user_info(uid_t uid, const char **user_name)
{
	struct passwd *pwd;
	static char uid_buf[32];
	int len;
	int i;
	uid_t tmp;

	pwd = getpwuid(uid);
	if (pwd)
		*user_name = pwd->pw_name;
	else
	{
		len = 0;
		tmp = uid;
		if (tmp == 0)
		{
			uid_buf[0] = '0';
			len = 1;
		}
		else
		{
			i = 0;
			while (tmp > 0)
			{
				uid_buf[i++] = '0' + (tmp % 10);
				tmp /= 10;
			}
			len = i;
			i = 0;
			while (i < len / 2)
			{
				char c = uid_buf[i];
				uid_buf[i] = uid_buf[len - 1 - i];
				uid_buf[len - 1 - i] = c;
				i++;
			}
		}
		uid_buf[len] = '\0';
		*user_name = uid_buf;
	}
}

/*
 * Get group name or GID
 * Uses getgrgid to get the name, otherwise converts GID to string
 * Buffer is static to avoid scope issues
 */
void	get_group_info(gid_t gid, const char **group_name)
{
	struct group *grp;
	static char gid_buf[32];
	int len;
	int i;
	gid_t tmp;

	grp = getgrgid(gid);
	if (grp)
		*group_name = grp->gr_name;
	else
	{
		len = 0;
		tmp = gid;
		if (tmp == 0)
		{
			gid_buf[0] = '0';
			len = 1;
		}
		else
		{
			i = 0;
			while (tmp > 0)
			{
				gid_buf[i++] = '0' + (tmp % 10);
				tmp /= 10;
			}
			len = i;
			i = 0;
			while (i < len / 2)
			{
				char c = gid_buf[i];
				gid_buf[i] = gid_buf[len - 1 - i];
				gid_buf[len - 1 - i] = c;
				i++;
			}
		}
		gid_buf[len] = '\0';
		*group_name = gid_buf;
	}
}
