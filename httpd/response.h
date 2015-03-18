/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: response.h
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#ifndef __RESPONSE__
#define __RESPONSE__

#include "xhttpd.h"

int do_response(struct user_info *new_user);
int http_nak(int fd, int code, const char *nak);
#endif
