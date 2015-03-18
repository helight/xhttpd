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
 *
 * Description:
 */

char *cgi_get(char *name);
int cookie_check(char *cookie, char *str);
int check_str(char *val1, char *val2);
int login_error(char *msg);
int display_msg(char *msg);

