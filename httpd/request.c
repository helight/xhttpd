/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: request.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#include "request.h"
#include "response.h"
#include "http.h"
#include "debug.h"

char msg[4096];				/*receive the http head*/

static int read_request_msg(int connfd);
static int analys_post_value(struct user_info *new_user, char *str);
static int analys_request(char *msg, struct user_info *new_user);

/*
 * analys the method, request file name and the protocol.
 *
 * do_request(struct user_info *new_user)
 * @ connfd : the file_desc of connect.
 *
 */

void handle_request(struct user_info *new_user)
{
	int ret;
		
	debug_print("to get http head\n");
	
	ret = read_request_msg(new_user->fd);	
	if (ret < 0) 
		return;
		
	debug_print("to analys the http head\n");
	
	ret = analys_request(msg, new_user);
	if (ret < 0)
		return;
		
	debug_print("response the request\n");	
	do_response(new_user);
	
	return;
}

/*
 * analys the post method, and anslys the argc that pass to the server
 *
 * post_method(struct user_info *new_user, char *str)
 * @ new_user : the point to the struct that contain the info about this request
 * @ str : the request message
 *
 */
static int read_request_msg(int connfd)
{
	int n = 0;
	
	memset(msg, 0, sizeof(msg));
	debug_print("the length of msg: %d\n", (int)sizeof(msg));
	/*
	if (read(connfd, msg, sizeof(msg)-1) <= 0) {
		debug_print("\nUnkown request\t\t");
		return -1;
	}
	n = strlen(msg);
	msg[n] = '\0';
	debug_print("\nlen: %d  msg:%s \n", n, msg);
	*/
	if (recv(connfd, msg, sizeof(msg)-1, 0) <= 0) {
		debug_print("\nUnkown request\t\t");
		return -1;
	}
	n = strlen(msg);
	msg[n] = '\0';
	debug_print("\nlen: %d  msg:%s \n", n, msg);
	
#ifdef Debug2
	int i = 0;
	printf("\n\n\n");
	while(i < n){
		debug_print("%02x,%c--", msg[i],msg[i]);
		if (i%8 == 0)
			printf("\n");
		i++;
	}

	printf("\n\n\n");
#endif
	debug_print("get the http head\n");
	//http_nak(connfd, 404, NAK_404);
	return n;
}

/*
 * analys the method, request file name and the protocol.
 *
 * get_path_cmd(char *msg, struct user_info *new_user)
 * @ msg : the request head
 * @ cmd : the request method
 * @ path : the request file name
 *
 */
static int analys_request(char *msg, struct user_info *new_user)
{
	int 	ret; 
	char	*token, *token1;
	char	*ch = NULL;

	token = strtok(msg," ");
	if (strncmp(token, "GET", 3) == 0)
		new_user->method = GET;
	else if (strncmp(token, "POST", 4) == 0)
		new_user->method = POST;
	else
		new_user->method = UNKNOW;

	token = strtok(0, " ");
	token++;
	if ((ch = strchr(token, '?')) != NULL) {
		new_user->values = ++ch;
		strncpy(new_user->path, token, ch - token - 1);
	} else {
		if (strlen(token) < sizeof(new_user->path))
			strcpy(new_user->path, token);
		else
			return -1;
	}
	
	debug_print("new_user->path: %s \n", new_user->path);
	
	token = strtok(0, "");
	if (strstr(token, "1.0"))
		new_user->httpv = V0;
	else if (strstr(token, "1.1"))
		new_user->httpv = V1;
	else
		new_user->httpv = UNV;
	debug_print("here is http version\n");	

	//debug_print("analys: %s \n",  token);
	ret = analys_post_value(new_user, token);
	debug_print("analys_post_value: %d \n",  ret);
		
	if ((token1 = strstr(token, "Cookie:")) !=NULL) {
		ch = strstr(token1, "\n");
		*ch = '\0';
		debug_print("Cookie:%s\n", token1 + 8);
		new_user->cookie = token1 + 8;
		token = ch + 1;
	}

	if (ret < 0)
		return -1;
	
        debug_print("analys cmd --%d path:--%s http: --%d \n",
        		new_user->method, new_user->path, new_user->httpv);
		
	return 0;
}

/*
 * analys the post method, and anslys the argc that pass to the server
 *
 * post_method(struct user_info *new_user, char *str)
 * @ new_user : the point to the struct that contain the info about this request
 * @ str : the request message
 *
 */
static int analys_post_value(struct user_info *new_user, char *str)
{
	char cr[]="\n\r\n";
	
	debug_print("method here: %d\n", new_user->method);
	if (new_user->method == POST) {
		debug_print("POST values %d : %s \n",__LINE__, str);
		str = strstr(str, cr);
		debug_print(" POSTvalues here: %s\n", str);
		if (str) {
			//strcat(new_user->path, "?");
			*(str + strlen(str) + 1) = '\0';
			new_user->values = str+3;
			//strcat(new_user->path, str+3);
			debug_print("post atg: %s new_user->values: %s\n\n", \
				new_user->path, new_user->values);
		}
	} 
	debug_print("values here\n");
	
	return 0;	
}


