/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: Init.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */
#include "init.h"
#include "config.h"
#include "debug.h"

extern struct server_env s_env;

static void daemon_init(struct server_env *s_env);



static inline void init_env(struct server_env *s_env)
{
	memset(s_env, 0, sizeof(struct server_env));
}

/*
 * config the server
 *
 * init_server(void)
 *
 */
void init_server(struct server_env *s_env)
{
	init_env(s_env);
	
	do_config(s_env);
	
	debug_print("www root dir is :%s \n", s_env->root_dir);
	if (chdir(s_env->root_dir)<0) {
		perror("some error happened when do chdir\n");
		exit(errno);
	}
	
	daemon_init(s_env);

	return;
}
#ifndef Debug
/*
 * make the server run as a daemon process
 *
 * daemon_init(void)
 *
 */
static void daemon_init(struct server_env *s_env)
{
	int i;
	pid_t pid;
	int tmp;
	if ((pid = fork()) == -1) {
		perror("fork error");
		write_log("fork error", 0);
		exit(errno);
	}
	else if (pid) {
        	exit(0);
	}
	if (setsid() == -1) {
		perror("setsid error");
		write_log("setsid error", 0);
		exit(errno);
	}
	signal(SIGHUP,SIG_IGN); 	/*ignore signals*/

	if ((pid = fork()) == -1) {
		perror("fork error");
		write_log("fork error", 0);
		exit(errno);
	}
	else if (pid) {
		exit(0);
	}
	if (chroot(s_env->root_dir) == -1) {
		perror("chroot error");
		write_log("chroot error", 0);
		exit(errno);
	}
	if ((tmp = chdir(s_env->root_dir)) == -1) {
		perror("chdir error");
		write_log("chdir error", 0);
		exit(errno);
	}
	umask(0);
	for (i = 0;i < MAXFD;i++) {
		do_close(i);
	}
}
#else
static void daemon_init(struct server_env *s_env)
{

}
#endif




