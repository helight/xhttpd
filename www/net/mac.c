/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: mac.c
 * modify date: 2008-11-10
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 *
 * Description:
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <time.h>

int main(int argc, char *argv[])
{
	struct ifreq ifreq;
	int sock;
	//char *cookie = getenv("HTTP_COOKIE");
	//int len = atoi(getenv("CONTENT_LENGTH"));


	printf("Set-Cookie:user=helight.xu\r\n");
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>MAC</title><meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center><h1>MAC ADDRESS</h1></div><hr> <div>\n");
	if ((sock=socket(AF_INET,SOCK_STREAM,0)) < 0) {
                perror("socket");
                return 2;
        }
        strcpy(ifreq.ifr_name,"eth0");
        if (ioctl(sock,SIOCGIFHWADDR,&ifreq)<0) {
                perror("ioctl");
                return 3;
        }
        printf("The MAC of eth0: %02x:%02x:%02x:%02x:%02x:%02x<br>\n",
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);

        time_t the_time;
        the_time=time((time_t *)0); /*调用time系统调用*/
        printf("The time is %ld\n",the_time);

	printf("</div><hr></div></body></html>\n");
	
	return 0;
}
