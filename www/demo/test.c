/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: test.c
 * modify date: 2009-11-28
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 *
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{	
	char *path = getenv("XY_path");

	if (!path) {
		char *p = "hello";
		path = p;
	}
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>The content you want </title>");
	printf("<meta http-equiv='Content-Style-Type' content='text/css';charset=utf-8>");
	printf("</head><body>");

	printf("<h1 align=center > The imformation you need is</h1>");
	printf("<a href='test.xcgi?XY_path=%s%s'>%s</a>", path, path, path);
	printf("<hr></body></html>");

	return 0;
}
