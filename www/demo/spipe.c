/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: spipe.c
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
	char *cookie = getenv("HTTP_COOKIE");
	int len = atoi(getenv("CONTENT_LENGTH"));
	printf("Set-Cookie:user=helight.xu\r\n");
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>cgi post</title>\
                <meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center>\
                <h1>cgi post</h1></div><hr> <div>");
	
	read(0, buf, len);
	buf[len] = '\0';
	printf("buf:%s\n", buf);
	printf("cookie:%s len:%d\n", cookie, len);

        printf("<form action=spost.cgi method=post>");
	printf("<input type=text name=XY_wd  size=42 maxlength=100><br>");
	printf("<input type=text name=XY_ww  size=42 maxlength=100><br>");

        printf("<br><input type=submit value=配置 >\
                <input type=reset value=重置 ></form>\
                </div><hr></div></body></html>\n");
	
	return 0;
}
