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
 * File Name:    pspower.c
 *
 * Description:  count the running time of each process
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
#include <time.h>


int GetProcInformation(int pid);
int ShowInformation();

static char PName[16];
static char Pstate;
static int Ppid;
static unsigned long Putime, Pstime, Pcutime, Pcstime, PCtime;
static unsigned long ultmp;
static int itmp;
static char buf[800];
static int process_nums = 0;

int ShowProcInformation()
{
	struct dirent *ent;
	DIR *dir;
	
	process_nums = 0;
	printf("<tr style='background-color:#f60;'><td>进程名</td><td>进程号</td><td>Ctime</td><td>Rtime</td>"
		"<td>Utime</td><td>Stime</td><td>CUtime</td><td>CStime</td></tr>");
	dir = opendir("/proc");
	while ((ent = readdir(dir))) {
		if(*ent->d_name < '0' || *ent->d_name > '9') 
			continue;
		if (GetProcInformation(atoi(ent->d_name)))
			continue;
		ShowInformation();
		process_nums++;
	}

	closedir(dir);

	return 1;
}

unsigned long startTime = 0;
void read_start_time()
{
	FILE *fp;
	long tmp;
	time_t t;
	struct tm *tp;

	if ((fp = fopen("/proc/uptime", "r")) == 0) {
		printf("read /ptoc/uptime error\n");
		exit(-1);
	}
	fscanf(fp, "%ld %ld\n", &tmp, &t);
	t = time(0);
	tp = localtime(&t);

	startTime = tp->tm_hour * 3600 + tp->tm_min * 60 + tp->tm_sec - tmp;
	fclose(fp);
}
int GetProcInformation(int pid)
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
	memset(PName, 0, sizeof(PName));
	sscanf(buf, "%d (%15c", &Ppid, PName); 
	num = sscanf(tmp + 2,				 
		   "%c "
		   "%d %d %d %d %d "
		   "%lu %lu %lu %lu %lu %lu %lu "
		   "%ld %ld %ld %ld %ld %ld "
		   "%lu",
		   &Pstate,
		   &itmp, &itmp, &itmp, &itmp, &itmp,
		   &ultmp, &ultmp, &ultmp, &ultmp, &ultmp, &Putime, &Pstime,
		   &Pcutime, &Pcstime, &ultmp, &ultmp, &ultmp, &ultmp, 
		   &PCtime
		  );
	if(Ppid != pid) 
		return -1;
	return 0;
}
static char *ConvertToTime(long time)
{
	static char buffer[15];
	long hour, min, sec;
	memset(buffer, '\0', 10);
	hour = time / 3600;
	time %= 3600;
	min = time / 60;
	time %= 60;
	sec = time;
	snprintf(buffer, 15, "%2ld:%2ld:%2ld", hour , min , sec);
	return buffer;
}
static char *GetRunTime(long utime, long stime)
{
	long t;
	t = utime / sysconf(_SC_CLK_TCK) + stime / sysconf(_SC_CLK_TCK);
	return ConvertToTime(t);
}
static char *GetCreateTime(long t)
{
	t = t / sysconf(_SC_CLK_TCK) + startTime;
	return ConvertToTime(t);
}
#define BUF_LEN 15
static char utime[BUF_LEN], sstime[BUF_LEN], cutime[BUF_LEN], cstime[BUF_LEN],sctime[BUF_LEN], rtime[BUF_LEN];
int ShowInformation()
{
	char pid[10];
	
	snprintf(pid, 10, "%d", Ppid);
	snprintf(utime, BUF_LEN, "%ld", Putime);
	snprintf(sstime, BUF_LEN, "%ld", Pstime);
	snprintf(cutime, BUF_LEN, "%ld", Pcutime);
	snprintf(cstime, BUF_LEN, "%ld", Pcstime);
	snprintf(sctime, BUF_LEN, "%s", GetCreateTime(PCtime));
	snprintf(rtime, BUF_LEN, "%s", GetRunTime(Putime, Pstime));
	if (process_nums%2)
		printf("<tr style='background-color:#992796;' ><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
			PName, pid, sctime, rtime, utime, sstime, cutime, cstime);
	else
		printf("<tr style='background-color:#999996;'><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td></tr>",
			PName, pid, sctime, rtime, utime, sstime, cutime, cstime);	return 0;
}
int main()
{
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>CPUINFO</title>\
                <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><table style='border:2px; border-color:red'; >");
	ShowProcInformation();
	printf("</table></body></html>\n");
	return 0;
}
