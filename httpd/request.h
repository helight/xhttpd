/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: request.h
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#ifndef __HTTP_PROTOCOL__
#define __HTTP_PROTOCOL__

#include "xhttpd.h"

void handle_request(struct user_info *new_user);

#endif

