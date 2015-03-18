/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: cpipe.c
 * modify date: 2009-11-28
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 *
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	char buf[1024];
	printf("<html><head><title>cgi post</title>\
                <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center>\
                <h1>cgi post</h1></div><hr> <div>");
	
	read(0, buf, sizeof(buf));
	printf("buf:%s\n", buf);
	printf("<br> </div><hr></div></body></html>\n");
	
	return 0;
}
