/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: set_socket.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#include "set_socket.h"
#include "debug.h"

static int httpd_listen(short port);
static int do_socket(void);
static int do_bind(int fd, struct sockaddr *sadd, int len);
static void init_new_user(struct user_info *new_user);

static int sock_opt = 1;
extern struct server_env s_env;


/*
 * listen to connect
 *
 * listen_connect : the main module to setup the socket
 *
 */
int listen_connect(struct server_env *s_env)
{
	int  listen_fd;
	
	if ((listen_fd = httpd_listen((short)s_env->port)) == -1) {
		exit(1);
	}
	
return listen_fd;
}

/*
 * deal with the error messages of socket
 *
 * do_socket(void)
 *
 * return fd of socket or -1 if fail
 *
 */
static int do_socket(void)
{
	int fd;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		write_log("Soket error\n", 0);
		switch (errno) {
		case ENFILE:
			write_log("The  system  limit  on  the  total number \
					 of open files has been reached.\n", 0);
			break;
		case EPROTONOSUPPORT:
			write_log("The  protocol  type  or  the specified \
					protocol is not supported within this domain.", 0);
			break;
		case EACCES:
			write_log("Permission  to create a socket of the specified \
					 type and/or protocol is denied.", 0);
			break;
		case EAFNOSUPPORT:
			write_log("Don't support the specified type and/or protocol.", 0);
			break;
		case EINVAL:
			write_log("Unknow protocol.", 0);
			break;
		case ENOBUFS:
		case ENOMEM:
			write_log("Not enough memory.", 0);
			break;
		default:
			write_log("Unknow error.", 0);
			break;
		}
		return -1;
	}
	return fd;
}

/*
 * bind the ip address and the port
 *
 * do_bind(int fd, sturct sockaddr *sadd, int len)
 * @ fd
 *
 * return value: success 0 or fail -1
 */
static int do_bind(int fd, struct sockaddr *sadd, int len)
{
	int fb;
	fb = bind(fd, (struct sockaddr *)sadd, len);
	if (fd < 0){
		switch (errno) {
		case EADDRINUSE:
			write_log("The given address is already in use\n", 0);
			break;
		case EBADF:
			write_log("sock_fd is not a valid descriptor\n", 0);
			break;
		case EACCES:
			write_log("The address is protected\n", 0);
			break;
		default:
			write_log("Bind error\n", 0);
			break;
		}
		do_close(fd);
		return -1;
	}

	return 0;
}

/*
 * set up the http listen
 *
 * http_listen(shor port)
 * @ port : the listen port on sercer
 *
 * return value: success 0 or fail -1
 */
static int httpd_listen(short port)
{
	int listenfd;
	struct sockaddr_in  servaddr;

	bzero(&servaddr, sizeof(servaddr));		/*initial socket address to zero*/
	servaddr.sin_family = AF_INET;			/*use IPv4*/
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	/*use localhost IP*/
	servaddr.sin_port = htons(port);		/*convert from host byte order to network byte order*/

	listenfd = do_socket();
	if (listenfd < 0) {
		return -1;
	}
	
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) == -1) {
                debug_print("error\n");
                return -1; 
        }
 
	if (do_bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		return -1;
	}
	
	debug_print("begin to listen\n");
	
	if (listen(listenfd, LISTENQ) < 0) {
		switch (errno) {
                case EADDRINUSE:
			write_log("Another socket is already listening on the same port.", 0);
                        break;
                case EBADF:
                        write_log("Sock_fd is not a valid descriptor\n", 0);
                        break;
                case ENOTSOCK:
                        write_log("The argument sockfd is not a socket.\n", 0);
                        break;
                case EOPNOTSUPP:
                        write_log("The socket is not of a type that supports the listen() operation.\n", 0);
                        break;
                default:
                        write_log("Listen error\n", 0);
                        break;
		}
		return -1;
	}

	return listenfd;
}

/*
 * accept the http request of brows
 *
 * httpd_accept(int listenfd)
 * @ listenfd the file desc of connect socket
 *
 * return value: return new link info
 */
struct user_info *httpd_accept(int listenfd)
{
	int connect_fd;
	socklen_t len;
	struct sockaddr_in client_addr;
	struct timeval tv;
	while (1) {
		len = sizeof(client_addr);
		connect_fd = accept(listenfd,(struct sockaddr *)&client_addr, &len);
		if (connect_fd == -1) {
			switch (errno) {
			case ECONNABORTED:
				write_log("A connection has been aborted.\n", 0);
				break;
			case EPERM:
				write_log("Firewall rules forbid connection.\n", 0);
				break;
			case ENFILE:
				write_log("The system limit on the total number of \
						open files has been reached.\n", 0);
				break;
			case EINTR:
				write_log("The system call was interrupted by a signal \
				 that was caught before a valid connection arrived.", 0);
				break;
			default:
				write_log("Unknow error", 0);
				break;
			}
			continue;
		}
		break;
	}
	
	debug_print("receive a new link\n");
	
	struct user_info *new_user = (struct user_info *)malloc(sizeof(struct user_info));
	
	init_new_user(new_user);
	
	gettimeofday(&tv, NULL);
	new_user->fd = connect_fd;
	new_user->ipaddr = client_addr.sin_addr;
	new_user->port = client_addr.sin_port;
	new_user->login_time = tv.tv_sec;
	
	debug_print("client port is: %d, client ip is: %s\n",
		ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr));
	write_log("client port is: %d, client ip is: %s\n",
		ntohs(client_addr.sin_port), inet_ntoa(client_addr.sin_addr));
	return new_user;
}

static void init_new_user(struct user_info *new_user)
{
	new_user->fd = -1;
	//new_user->ipaddr = -1;
	new_user->port = -1;
	new_user->login_time = 1;
	new_user->constate = ALIVE;
	new_user->values = NULL;		/*the arg values*/
	new_user->cookie = NULL;		/*the cookie*/
	new_user->next = NULL;
	new_user->login_time = -1;		/*login time*/
	memset(new_user->brows, '\0', sizeof(new_user->brows));
	memset(new_user->filename, '\0', sizeof(new_user->filename));
	memset(new_user->path, '\0', sizeof(new_user->path));	
	/*the request path*/
}


