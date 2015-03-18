/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: response.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */
 
 
#include "response.h" 
#include "do_cgi.h"
#include "http.h"
#include "debug.h"

extern struct server_env s_env;

static int send_head(const char *ack, int len, const char *path, int fd);
static int common_file(char *pathname, int fd);

/*
 * hand_request send the requested file or run the CIG program.
 *
 * hand_request(struct user_info *new_user)
 * @ new_user : get or post or other.
 */
int do_response(struct user_info *new_user)
{
	struct stat statbuf;
	char	pathname[512];
	char	*file_name = new_user->path;
	int	fd = new_user->fd;
	
	memset(pathname, '\0', sizeof(pathname));		 
	strcpy(pathname, getenv("PWD"));
	strcat(pathname, "/www/");	
	if (strcasecmp(file_name, "") == 0) {
		strcat(pathname, s_env.index);		//index.htm
		debug_print("no name: %s\n", s_env.index);
		strcpy(file_name, pathname);
	} else {
		strcat(pathname, file_name);
		debug_print("no name:%s\n", pathname);
	}
	if (stat(pathname, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) { 
		debug_print("pathname: %s new_user->path:%s\n", pathname, new_user->path);
	}else{
		debug_print("pathname: %s new_user->path:%s PWD: %s\n", 
				pathname, new_user->path, getenv("PWD"));
		http_nak(fd, 404, NAK_404);
		return 0;
	}
		
	if (strstr(pathname, ".") == NULL) {
		debug_print("wrong url name\n");
		http_nak(fd, 404, NAK_404);
		return 0;
	}
			
	if (strstr(pathname, ".cgi") != NULL) {
		debug_print("A cgi file\n");
		do_cgi(new_user);
		return 0;
	}
	if (strstr(pathname, ".xcgi") != NULL) {
		debug_print("a xcgi file\n");
		do_cgi(new_user);
		return 0;
	}
	
	common_file(pathname, fd);
	
	return 0;

}//end do_response


/*
 * common_file : send the picture ,css ,html files and so on.
 *
 * common_file(char *pathname, int fd)
 * @ pathname : the name of the request file
 * @ fd : the file_desc of connect.
 *
 */ 
static int common_file(char *pathname, int fd)
{
	FILE		*in;
	struct stat	statbuf;
	int		n;
	char buf[1024];
	
	if (stat(pathname, &statbuf) == -1 || (in = fopen(pathname, "rb")) == NULL) {
		debug_print("can't find the file or can't read\n");
		http_nak(fd, 404, NAK_404);
		return 0;
	}

	send_head(ACK_200, statbuf.st_size, pathname, fd);
	
	while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
		if (n != write(fd, buf, n))
			return -1;
	}
	fclose(in);
	return 0;
}

/*
 * send_head : send the http head to the brows.
 *
 * send_head(const char *ack, int len, const char *path, int fd)
 * @ msg :
 * @ len :
 * @ path : the path of file.
 * @ fd : the file_desc of connect.
 *
 */ 
static int send_head(const char *ack, int len, const char *path, int fd)
{
	char buf[128], *dot;
	int  nwrite;
	dot = strrchr(path, '.');
	if (dot!=NULL) {
		memset(buf, '\0', sizeof(buf));
		if ((strcasecmp(dot, ".jpg") == 0 || strcasecmp(dot, ".jpeg") == 0)) {
			nwrite = strlen(CONTENT_JPEG);
			snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_JPEG, (long)len);
		} else if ((strcasecmp(dot, ".png") == 0)) {
			nwrite = strlen(CONTENT_PNG);
			snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_PNG, (long)len);
		} else if ((strcasecmp(dot, ".gif")) == 0) {
			nwrite = strlen(CONTENT_GIF);
			snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_GIF, (long)len);
		} else if ((strcasecmp(dot, ".css")) == 0) {
			nwrite = strlen(CONTENT_CSS);
			snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_CSS, (long)len);
	        } else if (((strcasecmp(dot, ".cgi")) == 0 ) || \
	        		(strcasecmp(dot, ".xcgi") == 0 )) {
	                return 0;
	        } else if ((strcasecmp(dot, ".js")) == 0) {
	                nwrite = strlen(CONTENT_JS);
	                snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_JS, (long)len);
		} else {
			nwrite = strlen(CONTENT_TEXT);
			snprintf(buf, sizeof(buf), HEADER, ack, CONTENT_TEXT, (long)len);	
		}
		nwrite = strlen(buf);
		if(nwrite != write(fd, buf, nwrite))
			return -1;
	} else {
		http_nak(fd, 404, NAK_404);
		debug_print("404 Not Found: %d",len);
	}
	return 0;
}

/*
 * 505 HTTP Version Not Supported
 *
 * http_nak(int fd, int code, const char *nak)
 * @ fd : connect desc
 * @ code : the response conumber
 * @ nak : the response code
 */
int http_nak(int fd, int code, const char *nak)
{
        int len, len2;
        char buf[1024], buf2[1024];
        
        memset(buf, '\0', sizeof(buf));
        memset(buf2, '\0', sizeof(buf2));
	snprintf(buf2, sizeof(buf2), HTML_NAK, code, code, nak);
        len2 = strlen(buf2);
        snprintf(buf, sizeof(buf), HEADER, nak, CONTENT_TEXT, (long)len2);
        len = strlen(buf);

        if(len != write(fd, buf, len)){
                printf("writer head error: %s\n", nak);
                return -1;
        }
        if(len2 != write(fd, buf2, len2)) {
                printf("writer body  error: %s\n", nak);
                return -1;
        }

        return 0;
}

