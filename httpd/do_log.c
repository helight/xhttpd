/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: do_log.c
 * modify date: 2008-11-10
 * Author: helight.xu@gmail.com
 *
 * Program definitions:
 */

#include "xhttpd.h"
#include "debug.h"
#include <stdarg.h>
#include <time.h>

#define LOG_FILE_PATH "xhttpd.log"

extern struct server_env s_env; 

/*
 * write log to log system
 *
 * write_log(char *format, ...)
 * @ format : the message that want write to the log system
 *
 * return value: 0 write success or -1 write fail
 */
void write_log(char *format, ...)
{
	FILE		*logfile;
	time_t		t;
	time_t		*ptr_time=&t;
	struct tm	*tm;
	char 		temp[NAME_LEN], temp2[NAME_LEN * 2];
	char 		datetime[] = "[%d.%m.%Y] [%H:%M.%S]";
	char 		datetime_final[NAME_LEN];
	va_list 	ap;

	va_start(ap, format);		// format it all into temp
	vsprintf(temp, format, ap);
	va_end(ap);

	time (ptr_time);
	tm = localtime((time_t*) ptr_time);
	memset(datetime_final, 0, 128);
	strftime(datetime_final, 127, datetime, tm);
	
	// format it all so we have date/time/loginfo
	sprintf(temp2, "%s - %s\n", datetime_final, temp);
	debug_print("log_file: %s \n", LOG_FILE_PATH);
	
	if ((logfile = fopen(LOG_FILE_PATH, "a+")) == NULL) {
		perror("write log!\n");
		return;
	}

	fwrite(temp2, strlen(temp2), 1, logfile);	// Save to the file
		
	fclose(logfile);		// Close file
}
