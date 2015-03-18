/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: config.h
 * modify date: 2008-12-06
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define	XHTTPD_NAME	"XIYOU-HTTPD"
#define XHTTPD_VERSION	"V0.01"
#define XHTTPD_AUTHOR	"Helight.Xu@gmail.com"

#define  CONFIG_FILE	"/etc/xhttp.conf"
#define  LOG_FILE	"/var/log/xhttp.log"
#define  DEFAULT_DOC	"login.htm"	//#define  WEB_ROOT   "/home/helight/workspace/xhttpd/web/"
#define  WEB_ROOT	"www/"
#define  PORT		8080

#define  SET_PORT		1
#define  SET_WEB_ROOT		2
#define  SET_DEFAULT_DOC	4
#define  SET_LOG_FILE		8


int do_config(struct server_env *server_env);

#endif /* _CONFIG_H_ */
