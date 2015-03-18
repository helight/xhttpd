/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: xhttpd.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#include "init.h"
#include "set_socket.h"
#include "request.h"
#include "debug.h"

static void xhttpd_signal_handle(int signum);

struct server_env s_env; 
int listen_fd = 0;

int main(int argc,char **argv)
{
	pid_t fd;
	struct timeval tv;
	fd_set readfds;
	
	init_server(&s_env);	
	debug_print("finish init\n");
	
	listen_fd = listen_connect(&s_env);	
		
	fprintf(stdout, "%s  %s @ %s  \n%s \n",	
			s_env.name, s_env.version,
			s_env.host_info.sysname, s_env.author);
	
	signal(SIGTERM, xhttpd_signal_handle);
	signal(SIGHUP, xhttpd_signal_handle);
	signal(SIGINT, xhttpd_signal_handle);
	signal(SIGCHLD, xhttpd_signal_handle);
	signal(SIGCHLD, NULL);
	
	while (1) {
		struct user_info *new_user = httpd_accept(listen_fd);

		if (new_user->fd == -1) {
			do_close(new_user->fd);

			debug_print("connect error\n");
			if (new_user)
				free(new_user);
			exit(1);
		}

		signal(SIGCLD,SIG_IGN);		/*to avoid zombie process*/
		fd = fork();
		if (fd == -1) {
			perror("main fork error:");
			debug_print("fork error \n");
		} else if (fd > 0) {			/*parent*/
		
			do_close(new_user->fd);
			//continue;
		} else if (fd == 0) {
		
			do_close(listen_fd);			
			debug_print("new child!\n");
			
			FD_ZERO(&readfds);
			FD_SET(new_user->fd, &readfds);
			tv.tv_sec = 5;
			tv.tv_usec = 0;
			select(new_user->fd + 1, &readfds, NULL, NULL, &tv);
			if (FD_ISSET(new_user->fd, &readfds)) {
				/*to handle the request*/
				handle_request(new_user);
			}
						
			do_close(new_user->fd);

			if(new_user)
				free(new_user);
			exit(0);
		}
		if (new_user)
			free(new_user);
	}

	close(listen_fd);
	while(waitpid(-1,NULL,WNOHANG) > 0); /* clean up child processes */
	return 0;
}

/*
 * close the useless fd
 *
 * do_close(int fd)
 * @ fd : fd
 *
 * return value: the closed fd's state
 */
int do_close(int fd)
{
	int state;
	while ((state = close(fd)) == -1 && errno == EINTR) {
		perror("close error:");
		continue;
	}
	return state;
}

static void xhttpd_signal_handle(int signum)
{
	if (signum == SIGCHLD) {
		/* clean up child processes */
		while(waitpid(-1,NULL,WNOHANG) > 0); 
		debug_print("give child to change: %d \n", getpid());
	} else {
	
		while(waitpid(-1,NULL,WNOHANG) > 0);
		debug_print("close all children: %d\n", getpid());
		
		close(0);
		close(1);	
		close(listen_fd);		
		exit(0);
	}
}
