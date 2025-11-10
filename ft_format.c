#include "ft_ls.h"

/*
 * Convert an integer to a string
 * Handles negative numbers
 * Returns the length of the created string
 */
int	ft_itoa(int n, char *buf)
{
	int len;
	int i;
	int neg;
	int tmp;

	len = 0;
	neg = (n < 0);
	if (neg)
		n = -n;
	if (n == 0)
	{
		buf[0] = '0';
		return (1);
	}
	tmp = n;
	while (tmp > 0)
	{
		buf[len++] = '0' + (tmp % 10);
		tmp /= 10;
	}
	i = 0;
	while (i < len / 2)
	{
		char c = buf[i];
		buf[i] = buf[len - 1 - i];
		buf[len - 1 - i] = c;
		i++;
	}
	if (neg)
	{
		i = len;
		while (i > 0)
		{
			buf[i] = buf[i - 1];
			i--;
		}
		buf[0] = '-';
		len++;
	}
	buf[len] = '\0';
	return (len);
}

/*
 * Format a date with year (for files > 6 months old)
 * Format: "10 déc 2023"
 */
static void	format_date_with_year(struct tm *tm_info, char *date_buf,
	char *months[])
{
	int day;
	int year;
	int month_idx;

	day = tm_info->tm_mday;
	year = tm_info->tm_year + 1900;
	month_idx = tm_info->tm_mon;
	date_buf[0] = '0' + (day / 10);
	date_buf[1] = '0' + (day % 10);
	date_buf[2] = ' ';
	date_buf[3] = months[month_idx][0];
	date_buf[4] = months[month_idx][1];
	date_buf[5] = months[month_idx][2];
	date_buf[6] = ' ';
	date_buf[7] = '0' + (year / 1000);
	date_buf[8] = '0' + ((year / 100) % 10);
	date_buf[9] = '0' + ((year / 10) % 10);
	date_buf[10] = '0' + (year % 10);
	date_buf[11] = '\0';
}

/*
 * Format a date with time (for files < 6 months old)
 * Format: "10 nov 15:00"
 */
static void	format_date_with_time(struct tm *tm_info, char *date_buf,
	char *months[])
{
	int day;
	int hour;
	int min;
	int month_idx;

	day = tm_info->tm_mday;
	hour = tm_info->tm_hour;
	min = tm_info->tm_min;
	month_idx = tm_info->tm_mon;
	date_buf[0] = '0' + (day / 10);
	date_buf[1] = '0' + (day % 10);
	date_buf[2] = ' ';
	date_buf[3] = months[month_idx][0];
	date_buf[4] = months[month_idx][1];
	date_buf[5] = months[month_idx][2];
	date_buf[6] = ' ';
	date_buf[7] = '0' + (hour / 10);
	date_buf[8] = '0' + (hour % 10);
	date_buf[9] = ':';
	date_buf[10] = '0' + (min / 10);
	date_buf[11] = '0' + (min % 10);
	date_buf[12] = '\0';
}

/*
 * Format a modification date
 * Chooses between format with year (> 6 months) or with time (< 6 months)
 * Uses French month names (jan, fév, mar, etc.)
 */
void	format_date(time_t mtime, char *date_buf)
{
	time_t now;
	struct tm *tm_info;
	char *months[] = {"jan", "fév", "mar", "avr", "mai", "jun",
		"jul", "aoû", "sep", "oct", "nov", "déc"};

	now = time(NULL);
	tm_info = localtime(&mtime);
	if (now - mtime > 15768000 || mtime > now)
		format_date_with_year(tm_info, date_buf, months);
	else
		format_date_with_time(tm_info, date_buf, months);
}

/*
 * Write a total number (for the "total" line)
 * Converts the number to string and displays it
 */
static void	write_total_number(long long total)
{
	char total_str[32];
	int len;
	long long tmp;
	int j;

	len = 0;
	tmp = total;
	if (tmp == 0)
	{
		total_str[0] = '0';
		len = 1;
	}
	else
	{
		while (tmp > 0)
		{
			total_str[len++] = '0' + (tmp % 10);
			tmp /= 10;
		}
		j = 0;
		while (j < len / 2)
		{
			char c = total_str[j];
			total_str[j] = total_str[len - 1 - j];
			total_str[len - 1 - j] = c;
			j++;
		}
	}
	total_str[len] = '\0';
	write(1, total_str, len);
}

/*
 * Display the "total X" line at the start of long format
 * Calculates the sum of blocks (st_blocks) of all files
 * Used only for directories (show_total = 1)
 */
void	print_total(t_entry *entries, int count)
{
	long long total;
	int i;

	total = 0;
	i = 0;
	while (i < count)
	{
		total += entries[i].st.st_blocks;
		i++;
	}
	write(1, "total ", 6);
	write_total_number(total);
	write(1, "\n", 1);
}
