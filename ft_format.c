#include "ft_ls.h"

/* Format day and month part of date */
static int	format_day_month(struct tm *tm_info, char *date_buf, char *months[])
{
	int	day;
	int	month_idx;

	day = tm_info->tm_mday;
	month_idx = tm_info->tm_mon;
	date_buf[0] = '0' + (day / 10);
	date_buf[1] = '0' + (day % 10);
	date_buf[2] = ' ';
	date_buf[3] = months[month_idx][0];
	date_buf[4] = months[month_idx][1];
	date_buf[5] = months[month_idx][2];
	date_buf[6] = ' ';
	return (7);
}

/* Format: "10 déc 2023" */
static void	format_date_with_year(struct tm *tm_info, char *date_buf,
	char *months[])
{
	int	year;
	int	offset;

	offset = format_day_month(tm_info, date_buf, months);
	year = tm_info->tm_year + 1900;
	date_buf[offset] = '0' + (year / 1000);
	date_buf[offset + 1] = '0' + ((year / 100) % 10);
	date_buf[offset + 2] = '0' + ((year / 10) % 10);
	date_buf[offset + 3] = '0' + (year % 10);
	date_buf[offset + 4] = '\0';
}

/* Format: "10 nov 15:00" */
static void	format_date_with_time(struct tm *tm_info, char *date_buf,
	char *months[])
{
	int	hour;
	int	min;
	int	offset;

	offset = format_day_month(tm_info, date_buf, months);
	hour = tm_info->tm_hour;
	min = tm_info->tm_min;
	date_buf[offset] = '0' + (hour / 10);
	date_buf[offset + 1] = '0' + (hour % 10);
	date_buf[offset + 2] = ':';
	date_buf[offset + 3] = '0' + (min / 10);
	date_buf[offset + 4] = '0' + (min % 10);
	date_buf[offset + 5] = '\0';
}

/* Format a modification date */
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