/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: cgilib.c
 * modify date: 2008-11-10
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 * modify by: susu1399@sina.com 2009-12-05
 *
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgilib.h"

char *cgi_get(char *name)
{
	char *method = NULL;
	char *qstring = NULL;
	char *valu = NULL;
	char *ch = NULL;
	if (name == NULL)
		return NULL;

	method = (char *)getenv("REQUEST_METHOD");
	if (method == NULL)
		return NULL;

	if (strncmp(method, "GET", 3) == 0) {
		qstring = (char *)getenv("QUERY_STRING");
		if (qstring == NULL)
			return NULL;
		if((ch = strstr(qstring, name)) == NULL)
			return NULL;
		valu = ch + strlen(name) +1;
		if ((ch = strchr(valu, '&')) == NULL) {
			return valu;
		} else {
			*ch = '\0';
			return valu;
		}
	} else if (strncmp(method, "POST", 4) == 0) {

	}

	return NULL;
}

int cookie_check(char *cookie, char *str)
{
	if (strstr(cookie, str) == NULL) {
		login_error("login.htm");
                return -1;
	}
	return 0;
}

int check_str(char *val1, char *val2)
{
	return strcmp(val1, val2); 
}

int login_error(char *msg)
{
	printf("<html><head><title>login error</title><meta \
		http-equiv='Content-Type' content='text/html;\
	        charset=utf-8'/>");
	printf("<body><div width=900px><div align=center><p><p><p>\
		<h1>You Haven't Login!!<br><p>wrong name or passworld<br><p><p>");
	printf("<a href='%s'>Login</a> <br></div></body></html>\n", msg);
	return 0;
}

int display_msg(char *msg)
{
	printf("<html><head><title>login error</title><meta \
		http-equiv='Content-Type' content='text/html;\
		charset=utf-8'/>");
	printf("<body><div width=900px><div align=center><p><p><p>\
		<h1>%s<h1><br><p><p>", msg);
	printf("</div></body></html>\n");
	return 0;
}


