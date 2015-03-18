/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: config.c
 * modify date: 2008-12-06
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#include "xhttpd.h"
#include "config.h"
#include "debug.h"

extern struct server_env s_env;

static int read_config(struct server_env *s_env);
static int do_default_config(struct server_env *s_env);
static int read_line(int fd, char *bufv);
static int do_setconfig(struct server_env *s_env, char *bufv);
static void check_config(struct server_env *s_env);

/*
 * do_config : do the config work after read config file or
 * do the default config
 *
 * do_config(struct server_env *server_env)
 * @ *server_env : the point of server_env that used in all files
 *
 * return : 0 is ok ,-1 is error
 *
 */
int do_config(struct server_env *s_env)
{
	int ret=0;
	char *tmp = NULL;
	struct stat statbuf;
	s_env->port = 0;
	s_env->set_mod = 0;
	s_env->start_time = 0;
	s_env->connection_timeout = 0;
	s_env->max_connect = 0;
	memset(s_env->version, '\0', sizeof(s_env->version));
	memset(s_env->name, '\0', sizeof(s_env->name));
	memset(s_env->index, '\0', sizeof(s_env->index));
	memset(s_env->author, '\0', sizeof(s_env->author));
	memset(s_env->log_file, '\0', sizeof(s_env->log_file));
	memset(s_env->root_dir, '\0', sizeof(s_env->root_dir));

	tmp = (char *)XHTTPD_NAME;
	strncpy(s_env->name, tmp, strlen(tmp));
	tmp = (char *)XHTTPD_VERSION;
	strncpy(s_env->version, tmp, strlen(tmp));
	tmp = (char *)XHTTPD_AUTHOR;
	strncpy(s_env->author, tmp, strlen(tmp));
	
        if (uname(&s_env->host_info) == -1) {
		debug_print("can't get host info\n");
                return -1;
	}
	if(stat(CONFIG_FILE,&statbuf) == 0) {
		debug_where();
		ret = read_config(s_env);
		if(ret < 0)
			do_default_config(s_env);
	} else {
		debug_where();
		do_default_config(s_env);
	}
	
	check_config(s_env);
	return 0;
}
/*
 * read_config : read config file
 *
 * read_config(struct server_env *server_env)
 * @ *server_env : the point of server_env that used in all files
 *
 * return : 0 is ok ,-1 is error
 */
static int read_config(struct server_env *s_env)
{
	int fd = 0;
	int ret = 0;
	char bufv[256];
	fd = open(CONFIG_FILE, O_RDONLY);
	if(fd < 0)
		return -1;
	ret = read_line(fd, bufv);
	while(ret != EOF){
		if(ret == 1){
			do_setconfig(s_env, bufv);
		}
		ret =  read_line(fd, bufv);
	}

	return 0;
}
static int do_setconfig(struct server_env *s_env, char *bufv)
{
	char *ch = NULL;
	unsigned int set_mod = s_env->set_mod;
	ch = strchr(bufv, ' ');
	*ch = '\0';
	ch++;
	debug_print("%s:%s\n", bufv, ch);
	if(strcmp(bufv,"DEFAULT_DOC") == 0){
		if((strstr(ch, ".htm") != NULL) || (strstr(ch, ".html") != NULL) \
			|| (strstr(ch, ".cgi") != NULL)){
			strncpy(s_env->index, ch, strlen(ch));
			set_mod |= SET_DEFAULT_DOC;
		} else {
			set_mod &= ~SET_DEFAULT_DOC;
			return -1;
		}
	}else if(strcmp(bufv,"WEB_ROOT") == 0){
		struct stat stats;
		if(lstat(ch, &stats) < 0){
			perror("lstat:\n");
			set_mod &= ~SET_WEB_ROOT;
			return -1;
		} 
		if(S_ISDIR(stats.st_mode)){
			strncpy(s_env->root_dir, ch, strlen(ch));
			set_mod |= SET_WEB_ROOT;
		} else {
			set_mod &= ~SET_WEB_ROOT;
                        return -1;
		}
	}else if(strcmp(bufv,"PORT") == 0){
		unsigned short port = atoi(ch);
		if((port > 0) && (port < 65535)){
			s_env->port=atoi(ch);
			set_mod |= SET_PORT;
		} else {
			set_mod &= ~SET_PORT;
			return -1;
		}
	}else if(strcmp(bufv,"LOGFILE") == 0){
		struct stat stats;
                if(lstat(ch, &stats) < 0){
                        perror("lstat:\n");
                        set_mod &= ~SET_LOG_FILE;
                        return -1;
                } 
                if(S_ISREG(stats.st_mode)){
			int fd = 0;
			if((fd = open(ch,O_WRONLY)) < 0){
				set_mod &= ~SET_LOG_FILE;
        	                return -1;
			}
			strncpy(s_env->log_file, ch, strlen(ch));
			set_mod |= SET_LOG_FILE;
			close(fd);
		}
	}
	s_env->set_mod = set_mod;
	return 0;
}
/*
 * do_default_config : do the default config
 *
 * do_default_config(struct server_env *server_env)
 * @ *server_env : the point of server_env that used in all files
 *
 * return : 0 is ok ,-1 is error
 */
static int do_default_config(struct server_env *s_env)
{
	unsigned int set_mod = s_env->set_mod;

	char *tmp = (char *)WEB_ROOT;
	strncpy(s_env->root_dir, tmp,strlen(tmp));
	set_mod |= SET_WEB_ROOT; 
	tmp = (char *)DEFAULT_DOC;
	strncpy(s_env->index, tmp, strlen(tmp));
	set_mod |= SET_DEFAULT_DOC; 
	s_env->port = PORT;
	set_mod |= SET_PORT; 
	tmp = (char *)LOG_FILE;
	strncpy(s_env->log_file, tmp, strlen(tmp));
	set_mod |= SET_LOG_FILE; 

	debug_print("http_dir: %s  index_file: %s  log_file: %s\n",
			s_env->root_dir, s_env->index, s_env->log_file);
	return 0;
}

/*
 * read one line from config file
 *
 * read_line(int fd, char *buf)
 * @ fd : the file description of open config file
 * @ buf : the buffer to read the command and value
 *
 */
static int read_line(int fd, char *bufv)
{
	int ret = 0;
	int i = 0;
	char ch = '\0';
	memset(bufv, '\0', strlen(bufv));
	if((ret = read(fd, &ch, 1)) == 0)
		return EOF;
	while(ch != '\n' && ret != 0){
		if (ch == '#' ) {	/*if it is a comment line,skip it over*/
			while (ch != '\n' ) {
				if ((ret = read(fd,&ch,1))  == 0) {
					break;
				}
			}
			ret = read(fd,&ch,1);
		} else if (ch == ' ') {	/*if it is a space,skip to the next character*/
			if (i != 0) {
				bufv[i++] = ch;
			}
			ret = read(fd,&ch,1);
		} else if (ret != 0){	/*a valuable character,store it*/
			bufv[i++] = ch;
			ret = read(fd,&ch,1);
		}
	}
	if (ret == 0 ) {	/*end of the line,also the end of the file,return EOF*/
		bufv[i] = '\0';
		return EOF;
	} else if (i != 0) {		/*end of an valuable line*/
		bufv[i] = '\0';
		return 1;
	}
	return 0;
}
static void check_config(struct server_env *s_env)
{
	unsigned int set_mod = s_env->set_mod;
	char *tmp = NULL;

	if((set_mod & SET_PORT) == 0){
		s_env->port = PORT;
	        set_mod |= SET_PORT;		
	}
	if((set_mod & SET_WEB_ROOT) == 0){
		char *tmp = (char *)WEB_ROOT;
	        strncpy(s_env->root_dir, tmp, strlen(tmp));
        	set_mod |= SET_WEB_ROOT; 
	}
	if((set_mod & SET_DEFAULT_DOC) == 0){
		tmp = (char *)DEFAULT_DOC;
        	strncpy(s_env->index, tmp, strlen(tmp));
	        set_mod |= SET_DEFAULT_DOC;
	}
	if((set_mod & SET_LOG_FILE) == 0){
		tmp = (char *)LOG_FILE;
        	strncpy(s_env->log_file, tmp, strlen(tmp));
	        set_mod |= SET_LOG_FILE;
	}
	s_env->set_mod = set_mod;
}
