/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Helight.Xu<Helight.Xu@gmail.com>
 * Created Time: Wed 27 May 2009 11:19:11 AM CST
 * File Name:    promem.c
 *
 * Description:  show the information of the system memery
 *
 */

#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char p_cmd[16];
static char p_state;
static int p_pid;
static int p_ppid, p_pgrp, p_session, p_tty_num, p_tpgid;
static unsigned long p_flags, p_min_flt, p_cmin_flt, p_maj_flt, p_cmaj_flt, p_utime, p_stime;
static long p_cutime, p_cstime, p_priority, p_nice, p_timeout, p_alarm;
static unsigned long p_start_time, p_vsize;
static long p_rss;
static unsigned long p_rss_rlim, p_start_code, p_end_code, p_start_stack;
static char buf[1024];

static int process_nums = 0;

int read_process_info(int pid);

int show_process()
{
	struct dirent *ent;
	DIR *dir;

	process_nums = 0;
	dir = opendir("/proc");
	printf("<tr style='background-color:#f60;'><td>进程名</td><td>进程号</td><td>虚拟地址</td><td>RSS</td><td>代码段</td></tr>");
	while ((ent = readdir(dir))) {
		if(*ent->d_name < '0' || *ent->d_name > '9') 
			continue;
		if (read_process_info(atoi(ent->d_name)))
			continue;
		if (process_nums%2)
			printf("<tr style='background-color:#992796;' ><td>%s</td><td>%d</td><td>%p</td><td>%p</td><td>%p</td></tr>",
				p_cmd, p_pid, (void *)p_vsize, (void *)p_rss, (void *)(p_end_code - p_start_code));
		else
			printf("<tr style='background-color:#999996;'><td>%s</td><td>%d</td><td>%p</td><td>%p</td><td>%p</td></tr>",
				p_cmd, p_pid, (void *)p_vsize, (void *)p_rss, (void *)(p_end_code - p_start_code));		
		process_nums++;
	}
	closedir(dir);
	return 1;
}

int read_process_info(int pid)
{
	char path[32];
	int num;
	int fd;
	char *tmp;
	struct stat sb;
	char *p;
	
	snprintf(path, 32, "/proc/%d/stat", pid);
	if ( (fd = open(path, O_RDONLY, 0) ) == -1 ) 
		return -1;
	num = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	p = strrchr(path, '/');
	*p = '\0';
	stat(path, &sb);
	if(num < 80) 
		return -1;
	buf[num] = '\0';
	tmp = strrchr(buf, ')');	 
	*tmp = '\0';				 
	memset(p_cmd, 0, sizeof(p_cmd));
	sscanf(buf, "%d (%15c", &p_pid, p_cmd); 
	num = sscanf(tmp + 2,				 
		   "%c "
		   "%d %d %d %d %d "
		   "%lu %lu %lu %lu %lu %lu %lu "
		   "%ld %ld %ld %ld %ld %ld "
		   "%lu %lu "
		   "%ld "
		   "%lu %lu %lu %lu",
		   &p_state,
		   &p_ppid, &p_pgrp, &p_session, &p_tty_num, &p_tpgid,
		   &p_flags, &p_min_flt, &p_cmin_flt, &p_maj_flt, &p_cmaj_flt, &p_utime, &p_stime,
		   &p_cutime, &p_cstime, &p_priority, &p_nice, &p_timeout, &p_alarm,
		   &p_start_time, &p_vsize,
		   &p_rss,
		   &p_rss_rlim, &p_start_code, &p_end_code, &p_start_stack
		  );
	if(p_pid != pid) 
		return -1;
	return 0;
}


int main()
{
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>CPUINFO</title>\
                <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><table style='border:2px; border-color:red'; >");
	show_process();
	printf("</table></body></html>\n");
	return 0;
}
