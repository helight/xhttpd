/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: xhttpd.h
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

/*
 *   MACROS
 */

#ifndef _XHTTPD_H            /*prevent reincluding this file*/
#define _XHTTPD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>             /* signal */

#define  NAME_LEN 128
#define  MAXFD 65535
#define  LISTENQ 8


int do_close(int fd);
void write_log(char *format, ...);

/*
 * The request method of brows
 * and now in this httpd we can do GET and POST
 */
enum method {
        GET,
        POST,
        UNKNOW,
};

/*
 * the state of the link
 */
enum state {
        ALIVE,	/*connect*/
        DEAD,	/*not connnect*/
};
/*
 * version of http request
 * 1.0
 * 1.1
 */
enum httpv {
        V0,	/*version 1.0*/
        V1,	/*version 1.1*/
        UNV,
};
/*
 * the config environment of the server,
 * It should be readed	from the configure file.
 *
 * @ port : listen port
 * @ start_time : when the server start
 * @ connection_timeout : how long should the connection keep when it's idle
 * @ max_connect : the max number client connect the server
 * @ log_file : the path of log file
 * @ version : the version of my server
 * @ name : the name of my server
 * @ index : the file name of the default
 * @ author : the name of the author
 * @ host_info : the information of the host system
 * @ root_dir : the default server dir
 */

struct server_env{
	unsigned short	port;				/*listen port*/
	unsigned int	set_mod;			/*the set stat of the server*/
	unsigned int	start_time;			/**/
	unsigned int	connection_timeout;		/**/
	unsigned int	max_connect;			/*max number of connected!*/
	char		version[8];
	char		name[16];
	char		index[16];			/*default index file*/
	char		author[32];			/*the name of author*/
	char		log_file[NAME_LEN];			/*path of log*/
	char		root_dir[NAME_LEN];		/*document root*/
	struct utsname 	host_info;			/*the information of the system*/
};

/*
 * the environment of the server when it is running ,
 * The date should be collect when the serve is running
 *
 * @ connect : how many client connect to the server now
 * @ state : the state of my server -- r or d
 */

struct run_env{
	unsigned int	connect;			/*number of connected*/
	char		state;
};

struct list_head {
        struct list_head *next, *prev;
};

/*
 * for user info
 */
struct user_info{
	int		fd;			/*connect file descr*/
	unsigned short 	port;			/*client port*/
	struct in_addr	ipaddr;			/*client ip*/
	unsigned long 	login_time;		/*login time*/
	enum httpv	httpv;			/*The request version of http */
	enum method	method;
	enum state	constate;
	char		brows[16];
	char		filename[32];
	char		path[512];		/*the request path*/
	char		*values;		/*the arg values*/
	char		*cookie;		/*the cookie*/
	struct user_info *next;
};


#endif /*the _XHTTPD_H end*/

