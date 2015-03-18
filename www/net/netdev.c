/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: netdev.c
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
#include <arpa/inet.h>

#include <cgilib.h>

#define IP_LEN 16
#define ERROR "读取本机ip出错"
#define MAXINTERFACES 16
static char armserver_ip[IP_LEN];
/* initialized in main.c */
char hostclient_ip[IP_LEN];

const char *get_localhost_ip();


int main(int argc, char *argv[])
{
	int ret = 0;
	char *cookie = getenv("HTTP_COOKIE");
	
	//int len = atoi(getenv("CONTENT_LENGTH"));

	printf("Content-type:text/html\r\n\r\n");
	/*if (strstr(cookie, "user=loginok") == NULL) {
		printf("<html><head><title>403</title><meta \
			http-equiv='Content-Type' content='text/html;\
			 charset=utf-8' />");
		printf("<body><div width=900px><div align=center>\
			<h1>403</h1></div> \n");
		printf("</body></html>\n");
	
		return 0;
	}*/
	ret = cookie_check(cookie, "user=loginok");
	if (ret)
		return -1;
	//printf("Set-Cookie:user=helight.xu\r\n");
	printf("<html><head><title>MAC</title><meta http-equiv='Content-Type'\
		 content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center><h1>MAC ADDRESS</h1>\
		</div><hr> <div>\n");

	get_localhost_ip();

        time_t the_time;
        the_time=time((time_t *)0); /*调用time系统调用*/

        printf("The time is %ld\n",the_time);

	printf("</div><hr></div></body></html>\n");
	
	return 0;
}
/*
 * 读取server端的ip地址
 * @return: 如果正确获取，则返回ip地址，否则返回NULL
 */
const char *get_localhost_ip()
{
	int fd, interface;
	struct ifreq buf[MAXINTERFACES];
	struct ifconf ifc;

	if (strlen(armserver_ip) > 0)
		return armserver_ip;
	if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("ESYSERR ERROR\n");
		return NULL;
	}
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
  	if (ioctl (fd, SIOCGIFCONF, (char *)&ifc) == -1) {
  		printf("ESYSERR ERROR\n");
		goto out;
	}
	/* 获取接口信息 */
	interface = ifc.ifc_len / sizeof(struct ifreq);
	printf("interface num is interface=%d<br>\n",interface);
	/* 根据借口信息循环获取设备IP和MAC地址 */
	while (interface-- > 0) {
		/* 获取设备名称 */
		if (strcmp(buf[interface].ifr_name, "eth0") ||
		    strcmp(buf[interface].ifr_name, "eth1")) {
			printf("net device %s\n", buf[interface].ifr_name);
			/* 判断网卡类型 */
			if ((ioctl (fd, SIOCGIFFLAGS, (char *)&buf[interface])) == -1) {
				printf("ESYSERR ERROR\n");
				goto out;
			}
			if (buf[interface].ifr_flags & IFF_PROMISC) {
				printf("the interface is PROMISC<br>\n");
			} else {
				printf("cpm: ioctl device %s<br>\n", buf[interface].ifr_name);
	 		}
			/* 判断网卡状态 */
			if (buf[interface].ifr_flags & IFF_UP) {
				printf("the interface status is UP<br>\n");
			} else {
				printf("the interface status is DOWN<br>\n");
			}
			/* 获取当前网卡的IP地址 */
			if (!(ioctl (fd, SIOCGIFADDR, (char *)&buf[interface]))) {
				printf("print IP address is: %s<br>",\
				inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr));\
				strncpy(armserver_ip,\
				inet_ntoa(((struct sockaddr_in*)(&buf[interface].ifr_addr))->sin_addr),\
				16);
				printf("The MAC of eth0: %02x:%02x:%02x:%02x:%02x:%02x<br>\n",
		                        (unsigned char)buf[interface].ifr_hwaddr.sa_data[0],
        		                (unsigned char)buf[interface].ifr_hwaddr.sa_data[1],
                		        (unsigned char)buf[interface].ifr_hwaddr.sa_data[2],
                        		(unsigned char)buf[interface].ifr_hwaddr.sa_data[3],
	                        	(unsigned char)buf[interface].ifr_hwaddr.sa_data[4],
	        	                (unsigned char)buf[interface].ifr_hwaddr.sa_data[5]);

				close(fd);
				return armserver_ip;
			}
		}
	}
out:
	close (fd);
	return NULL;
}
