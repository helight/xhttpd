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
 * File Name:    osversion.c
 *
 * Description:  show the information of the system's version
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
	FILE *fp = NULL;
	char buf[1024];

	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>OS VERSION</title>\
                <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center>\
                <h1>VERSION</h1></div><hr> <div>");

	if ((fp = fopen("/proc/version","r")) == NULL) {
		printf("can not open!\n");
		return -1;
	}
     
	do { 
		fgets(buf,sizeof(buf),fp);
		printf("%s <br>", buf);
	} while ( feof(fp) == 0);

	printf("</div><hr></div></body></html>\n");
	fclose(fp);
	
	return 0;
}
