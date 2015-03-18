/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: http.h
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */
 
 
#ifndef __HTTP__
#define __HTTP__


#define  HEADER	"HTTP/1.1 %s\r\nServer: Helight\r\n%s\r\nContent-Length: %ld\r\n\r\n"
#define  CONTENT_TEXT	"Content-Type: text/html"
#define  CONTENT_CSS	"Content-Type: text/css"
#define  CONTENT_JS	"Content-Type: text/javascript"
#define  CONTENT_JPEG	"Content-Type: image/jpeg"
#define  CONTENT_PNG	"Content-type: image/png"
#define  CONTENT_GIF	"Content-type: image/gif"

#define HTML_NAK	"<!DOCTYPE html PUBLIC \"-//IEIF//DTD HTML 2.0//EN\">\n"\
						"<html><head><title>Error %d</title>\n"\
						"</head><body>\n"\
						"<h2>Error %d</h2>"\
						"<h2>Helight Server: %s</h2>"\
						"</body></html>\r\n"
#define CGI_NAK	"Content-type:text/html\r\n\r\n"\
			"<html><head><title>Error %d</title>\n"\
			"</head><body>\n"\
			"<h2>Error %d</h2>"\
			"<h2>Helight Server: %s</h2>"\
			"</body></html>\r\n"
			
#define ACK_200		"200 OK"
#define NAK_403		"403 Permission Denied"
#define NAK_404		"404 Fesource Not Found"
#define NAK_405		"405 Method Not Allowed"
#define NAK_414		"414 Request-URI Too Long"
#define NAK_505		"505 HTTP Version Not Supported"

#endif
