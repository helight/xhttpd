/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: index.c
 * modify date: 2009-11-28
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 * modify by: susu1399@sina.com 2009-12-05
 *
 * Description:
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <cgilib.h>
#include <md5lib.h>
#include <user.h>

int main(int args, char *argv[])
{
	FILE *fp = NULL;
	struct usr user = {"", ""};
	char buf[1024];
	char *id = getenv("XY_id");
	char *pwd = getenv("XY_pwd");
	int flag = 1;
//	int ret = 0;

	printf("Content-type:text/html\r\n\r\n");
	
	if ((fp = fopen("passwd.dat","r")) == NULL){
		printf("file can not be opened!\n");
                return -1;	
	}
	
	while (!feof(fp)) {
		if (sizeof(struct usr) == fread(&user,sizeof(struct usr),1,fp)) {
			if (!(strcmp(user.id, id))) {
				if(!(strcmp(user.pwd,MDString(pwd)))){
					flag = 0;
					break ;
				}
			}
		}
	}
	if (flag == 1){
        	login_error("login.htm");
		printf("string:%s\n", MDString(id));
		return -1;
	}
	printf("Set-Cookie:user=loginok\n");	

        if ((fp = fopen("index.htm","r")) == NULL) {
                printf("can not open! index.htm\n");
                return -1;
        }

	do { 
		if (fgets(buf, sizeof(buf),fp))
			printf("%s", buf);
	} while (feof(fp) == 0);

        fclose(fp);

        return 0;

}                                  

