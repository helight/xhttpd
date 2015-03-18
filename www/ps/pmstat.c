/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author: huzhenbi<huzhenbi05@sina.com>
 * Created Time: Wed 27 May 2009 11:19:11 AM CST
 * File Name:    pmstat.c
 *
 * Description:  show the memery information of each process
 
 * Modified: Helight.Xu<Helight.Xu@gmail.com>>
 */
 
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>       
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/socket.h>
#include <ctype.h>

int main()
{
	DIR  *dir;
	DIR  *dir1 = NULL;
	FILE *fp;
	struct dirent  *ptr;
	struct dirent  *ptr1;
	char s[255];
	char path[50];
	char path1[50];
	char *mem ;
	int i;

	printf("Content-type:text/html\r\n\r\n");      
	printf("<html><head></head><body><div align='left'>");
	printf("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	if ((dir = opendir("/proc")) == NULL) {
	perror(" opendir") ; 
	return -1;
	}
	printf("<div><table style ='border-width:5px; border-collapse:collapse; \
	border-color:#222796'; border='2'><tr><td width='40px'>PID</td><td width='100px'>\
	该任务虚拟地址空间大小</td><td width='130px'>该任务当前驻留物理地址空间大小</td>\
	<td width='140px'>该任务能驻留物理地址空间的最大值 </td><td width='150px'>\
	该任务在虚拟地址空间代码段的起始地址 </td><td width='150px'>该任务在虚拟地址空间代码段的结束地址\
	</td><td width='140px'>该任务在虚拟地址空间栈的结束地址</td></tr>"); 

	while ((ptr = readdir(dir)) != NULL) {
		if (isdigit(ptr->d_name[0])) {
			strcpy(path, "/proc/");
			strcat(path, ptr->d_name);
			    
			if ((dir1 = opendir(path)) == NULL) {
				perror("opendir") ;
				return -1;
			}
			while ((ptr1 = readdir(dir1)) != NULL) {
				if (!strcmp(ptr1->d_name, "stat")) {
					strcpy(path1, path);
					strcat(path1, "/stat");       
					fp = fopen(path1, "r");
					fgets(s, 255, fp);  
					mem = strstr(s, " ");
					for ( i = 1; i < 22; i++) {    // di 23 ge shu wei suo xu de shu,
						mem += 1;
						mem = strstr(mem, " ");
					}
					printf("<tr><td width='40px'>%s</td>", ptr->d_name);
					printf("<td width='100px'>%s</td>", strtok(mem, " "));
					printf("<td width='130px'>%s</td>", strtok(NULL, " "));
					printf("<td width='140px'>%s</td>", strtok(NULL, " "));
					printf("<td width='150px'>%s</td>", strtok(NULL, " "));
					printf("<td width='150px'>%s</td>", strtok(NULL, " "));
					printf("<td width='140px'>%s</td></tr>", strtok(NULL, " "));
				}
			} 
		}
	}
	
	printf("</table></div>");
	closedir(dir1);
	closedir(dir);
	printf("</div></body></html>");

	return 0;
}


