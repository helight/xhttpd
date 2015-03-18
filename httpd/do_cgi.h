/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: do_cgi.h
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */
#ifndef __DO_CGI__
#define __DO_CGI__

#include "xhttpd.h"

int do_cgi(struct user_info *new_user);

#define REQUEST_METHOD 	"REQUEST_METHOD"		//请求类型，如 "GET" 或 "POST"
#define CONTENT_TYPE 	"CONTENT_TYPE"			//被发送数据的类型
#define CONTENT_LENGTH 	"CONTENT_LENGTH"		//客户端向标准输入设备发送的数据长度，单位为字节
#define Q_STRING 	"QUERY_STRING"			//查询参数，如 "id=10010&sn=liigo"
#define SCRIPT_NAME 	"SCRIPT_NAME"			//CGI脚本程序名称，如 "/xyz.efcgi"
#define PATH_INFO 	"PATH_INFO"			//CGI脚本程序的附加路径
#define PATH_TRANSLATED "PATH_TRANSLATED" 		//PATH_INFO对应的绝对路径
#define REMOTE_ADDR 	"REMOTE_ADDR"			//发送此次请求的主机IP
#define REMOTE_HOST 	"REMOTE_HOST"			//发送此次请求的主机名
#define REMOTE_USER 	"REMOTE_USER"			//已被验证合法的用户名
#define REMOTE_IDENT 	"REMOTE_IDENT"			//WEB服务器的登录用户名
#define AUTH_TYPE 	"AUTH_TYPE"			//验证类型
#define GATEWAY_INTERFACE 	"GATEWAY_INTERFACE"	//服务器遵守的CGI版本，如 "CGI/1.1"
#define SERVER_NAME 	"SERVER_NAME"			//服务器主机名、域名或IP
#define SERVER_PORT 	"SERVER_PORT"			//服务器端口号
#define SERVER_PROTOCOL "SERVER_PROTOCOL"		//服务器协议，如 "HTTP/1.1"
#define SERVER_SOFTWARE "SERVER_SOFTWARE"		//服务器软件的描述文本
#define DOCUMENT_ROOT 	"DOCUMENT_ROOT"			//文档根目录
#define HTTP_ACCEPT 	"HTTP_ACCEPT"			//客户端可以接收的MIME类型，以逗号(,)分隔
#define HTTP_USER_AGENT "HTTP_USER_AGENT"		//发送此次请求的WEB浏览器
#define HTTP_REFERER 	"HTTP_REFERER"			//调用此脚本程序的文档
#define HTTP_COOKIE 	"HTTP_COOKIE"			//获取COOKIE键值对，多项之间以分号(;)分隔，如 "key1=value1; key2=value2"

#endif
