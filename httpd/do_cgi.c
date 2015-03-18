/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: do_cgi.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */


#include "do_cgi.h"
#include "response.h"
#include "http.h"
#include "funs.h"
#include "debug.h"

static void response_xcgi_get(struct user_info *new_user);
static void response_cgi_get(struct user_info *new_user);
static void response_xcgi_post(struct user_info *new_user);
static void response_cgi_post(struct user_info *new_user);
static void decode(char *ch);
static void set_cgi_common_env(struct user_info *new_user);
static void unset_cgi_common_env(struct user_info *new_user);
static void send_cgi_head_response(enum httpv);

/*
 * do_cgi run the cgi program
 *
 * do_cgi(struct user_info *new_user)
 * @ new_user : the new link
 */
int do_cgi(struct user_info *new_user)
{

	switch (new_user->method) {
		case GET:
			debug_print("new_user->values: %s\n",new_user->values);
			if (strstr(new_user->path, ".xcgi"))
				response_xcgi_get(new_user);
			else
				response_cgi_get(new_user);
			break;
		case POST:
			if (strstr(new_user->path, ".xcgi"))
				response_xcgi_post(new_user);
			else
				response_cgi_post(new_user);
			break;
		default:
			debug_print("Unknown request\n");
			http_nak(new_user->fd, 405, NAK_405);
			return -1;
	}
	
	return 0;
}

/*
 * deal with the get xcgi 
 *
 * response_xcgi_get(struct user_info *new_user)
 * @ new_user : the new link
 */
static void response_xcgi_get(struct user_info *new_user)
{
	char	*file_name = new_user->path;
	int	fd = new_user->fd;
	int 	ffd;
	char 	*f_name, *token;
	char 	cmd_arg[5][32];
	char 	cmd_value[5][64];
	int 	i = 0, j = 0;
	int	fp[2];
	int	ret = 0;
	char *ch = NULL;
	
	if ((ret = pipe(fp)) < 0) {
		perror("pipe:\n");
		return;
	}
			
	f_name = new_user->values;
	debug_print("path: %s\n", f_name);
	if (new_user->values != NULL) {

		debug_print("argc:%s \n ", f_name);
		token = strtok(f_name, "=");
		debug_print("token : %s \n",token);
		while (token != NULL) {
			if (strlen(token) < 32)
				strcpy(cmd_arg[i], token);
			else
				strcpy(cmd_arg[i], "NULL");
			ch = token + strlen(token) + 1;
			debug_print("token : %s \n", ch);
			if ((*ch == '&' ) || (*ch == ' ')) {
				strcpy(cmd_value[i++], "NULL");
				goto do_fork;
			} else {
				token = strtok(0, "&");
				debug_print("token : %s \n",token);
				decode(token);
				if (strlen(token) < 64)
					strcpy(cmd_value[i++], token);
				else 
					strcpy(cmd_value[i++], "NULL");
			}
			debug_print("argc : %s value : %s i:%d\n",
					cmd_arg[i-1], cmd_value[i-1], i);
			if (i >= 5)
				break;
			token = strtok(0, "=");
			ch = token;
		}
	}
do_fork:	
	ffd = fork();
	if (ffd < 0) {
		perror("fork\n");
		exit(0);
	}
	if (ffd == 0) {

		for (j = 0; j < i; j++) {
			setenv(cmd_arg[j], cmd_value[j], 1);
			debug_print("cmd_arg: %s", cmd_arg[j]);
		}
		set_cgi_common_env(new_user);
	
		close(fp[1]);
		close(0);
		dup2(fp[0], 0);
		debug_print("file_name: %s\n", file_name);
		close(1);	//stdout
		dup2(fd, 1);  //stdout
		
		send_cgi_head_response(new_user->httpv);
		//debug_print("HTTP/1.1 200 OK\r\nServer: Helight\r\n");
		if (execl(file_name, file_name, NULL, NULL)<0) {
			perror("Err on execl");
			http_nak(1, 403, NAK_403);
		}
		for (j = 0; j < i; j++) {
			unsetenv(cmd_arg[j++]);
		}
		set_cgi_common_env(new_user);
		if(new_user)
			free(new_user);
		exit(0);
	}
	if (ffd > 0) {
		close(fp[0]);
		ret = write(fp[1], "NULL", 5);
		debug_print("ret: %d \n", ret);	
		wait(NULL);
		return;
	}
	return;
}

/*
 * deal with the get cgi
 *
 * esponse_cgi_get(struct user_info *new_user)
 * @ new_user : the new link
 */
static void response_cgi_get(struct user_info *new_user)
{
	int 	ffd;
	int	fp[2];
	int	ret = 0;
	
	if ((ret = pipe(fp)) < 0) {
		perror("pipe:\n");
		return;
	}
		
	ffd = fork();
	if (ffd < 0) {
		perror("fork\n");
		exit(0);
	}
	if (ffd == 0) {
		char	*file_name = new_user->path;
		int	fd =new_user->fd;
		
		set_cgi_common_env(new_user);
		close(fp[1]);
		close(0);
		dup2(fp[0], 0);
		debug_print("file_name: %s\n", file_name);
		close(1);	//stdout
		dup2(fd, 1);  //stdout

		send_cgi_head_response(new_user->httpv);
		//debug_print("HTTP/1.1 200 OK\r\nServer: Helight\r\n");
		if(execl(file_name, file_name, NULL, NULL)<0) {
			perror("Err on execl");
			http_nak(1, 403, NAK_403);
		}

		unset_cgi_common_env(new_user);
		
		if(new_user)
			free(new_user);
		exit(0);
	}
	if (ffd > 0) {
		close(fp[0]);
		ret = write(fp[1], "NULL", 5);
		debug_print("ret: %d \n", ret);
		wait(NULL);
			return;
	}

	return;
}

/*
 * response_xcgi_post deal with the post xcgi
 *
 * response_xcgi_post(struct user_info *new_user)
 * @ new_user : the new link
 */
static void response_xcgi_post(struct user_info *new_user)
{
	response_xcgi_get(new_user);
	return;
}

/*
 * response_cgi_post deal with the post cgi
 *
 * response_cgi_post(struct user_info *new_user)
 * @ new_user : the new link
 */
static void response_cgi_post(struct user_info *new_user)
{
	int 	ffd;
	int	fp[2];
	int	ret = 0;
	
	if ((ret = pipe(fp)) < 0) {
		perror("pipe:\n");
		return;
	}
	
	if ((ffd = fork()) < 0) {
		perror("fork\n");
		exit(0);
	}
	if (ffd == 0) {
		char	*file_name = new_user->path;
		int	fd = new_user->fd;
		//usleep(20);
		close(fp[1]);
		close(0);
		dup2(fp[0], 0);
		
		set_cgi_common_env(new_user);
			
		debug_print("file_name: %s\n", file_name);
		close(1);	//stdout
		dup2(fd, 1);  //stdout

		send_cgi_head_response(new_user->httpv);
		//debug_print("HTTP/1.1 200 OK\r\nServer: Helight\r\n");
		if(execl(file_name, file_name, NULL, NULL)<0) {
			perror("Err on execl");
			http_nak(1, 403, NAK_403);
		}
		
		unset_cgi_common_env(new_user);
		if(new_user)
			free(new_user);
		exit(0);
	}
	if (ffd > 0) {
		close(fp[0]);
		if (new_user->values) {
			ret = write(fp[1], new_user->values, strlen(new_user->values));
			debug_print("ret: %d values:%d \n", ret, (int)(strlen(new_user->values) - 1));
			ret = write(fp[1], "\r\n\r\n\r\n\r\n", 4);
		} else {
			ret = write(fp[1], "NULL", 5);
			debug_print("ret: %d \n", ret);
		}	
			wait(NULL);
			return;
	}
	
	return;
}
/*
 * stanslate %xx to Chinese
 *
 * decode(char *ch)
 * @ *ch : the string that have or not have the %xx
 *
 */
static void decode(char *ch)
{
	int	n = 0;
	int	i = 0;
	int	j = 0;
        n = strlen(ch);
        for (i = 0; i < n; i++) {
                if (ch[i] == '%') {
                        i++;
                        if (ch[i] >= 'a') ch[i] = ch[i] - 'a'+10;
                        else if (ch[i] >= 'A') ch[i] = ch[i] - 'A'+10;
                        else if (ch[i] >= '0') ch[i] = ch[i] - '0';

                        if (ch[i+1] >= 'a') ch[i+1] = ch[i+1] - 'a'+10;
                        else if (ch[i+1] >= 'A') ch[i+1] = ch[i+1] - 'A'+10;
                        else if (ch[i+1] >= '0') ch[i+1] = ch[i+1] - '0';

                        //debug_print("%02x-%02x ",ch[i], ch[i+1]);
                        ch[j] = ch[i]*16 + ch[i+1];
                        //debug_print("kk:%02x the I:%d the J:%d\n", ch[j], i, j);
                        j++; i++;
                } else {
			ch[j++] = ch[i];
		}

	}
	ch[j] = '\0';
}
/*
 * set the common cgi env
 *
 * set_cgi_common_env(struct user_info *new_user)
 * @ *new_user : the new link
 *
 */
static void set_cgi_common_env(struct user_info *new_user)
{
	if (new_user->cookie != NULL)
		setenv(HTTP_COOKIE, new_user->cookie, 1);

	switch (new_user->method){
		case POST:
			setenv(REQUEST_METHOD, "POST", 1);
			if (new_user->values) {
				char buf[32];
				snprintf(buf, sizeof(buf), "%d", (int)strlen(new_user->values));
				setenv(CONTENT_LENGTH, buf, 1);
			} else
				setenv(CONTENT_LENGTH, "0", 1);
			break;
		case GET:
			setenv(REQUEST_METHOD, "GET", 1);
			if (new_user->values) {
				char buf[32];
				snprintf(buf, sizeof(buf), "%d", (int)strlen(new_user->values));
				setenv(Q_STRING, new_user->values, 1);
				setenv(CONTENT_LENGTH, buf, 1);
			} else
				setenv(CONTENT_LENGTH, "0", 1);
			break;
		default:
			return;
	}
}

/*
 * set the common cgi env
 *
 * unset_cgi_common_env(struct user_info *new_user)
 * @ *new_user : the new link
 *
 */
static void unset_cgi_common_env(struct user_info *new_user)
{
	unsetenv(HTTP_COOKIE);
	switch (new_user->method){
		case POST:
			unsetenv(REQUEST_METHOD);
			if(new_user->values)
				unsetenv(CONTENT_LENGTH);
			else
				unsetenv(CONTENT_LENGTH);
			break;
		case GET:
			unsetenv(REQUEST_METHOD);
			if(new_user->values) {
				unsetenv(Q_STRING);
				unsetenv(CONTENT_LENGTH);
			} else
				unsetenv(CONTENT_LENGTH);
			break;
		default:
			return;
	}
}
/*
 * send the response of the cgi request
 *
 * send_cgi_head_response(enum httpv)
 * @ httpv the version type of the request
 *
 */
static void send_cgi_head_response(enum httpv httpv)
{
	
	switch (httpv){
		case V0:
			debug_print("HTTP/1.0 200 OK\r\nServer: Helight\r\n");
			break;
		case V1:
			debug_print("HTTP/1.1 200 OK\r\nServer: Helight\r\n");
			break;
		case UNV:
			debug_print("HTTP/1.0 200 OK\r\nServer: Helight\r\n");
			break;
	}
	
	return;
}
