/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: set_socket.h
 * modify date: 2008-12-06
 *
 * Program definitions:
 */

#ifndef DO_CONNECT_H_
#define DO_CONNECT_H_
#include "xhttpd.h"

int listen_connect(struct server_env *s_env);

struct user_info *httpd_accept(int listenfd);

#endif /* DO_CONNECT_H_ */
