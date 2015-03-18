/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: adduser.c 
 * modify date: 2009-12-05
 * Author: susu1399@sina.com  
 * modify by: Helight.Xu@Gmail.com 2009-12-05
 *
 * Program definitions: add an user to login the system
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <md5lib.h>
#include <cgilib.h>
#include <user.h>

int main()
{
	struct usr user = {"", ""};
//	struct usr user1 = {"", ""};
	char *id = getenv("XY_id");
	char *pwd = getenv("XY_pwd");
	char *pwd1 = getenv("XY_pwd1");
	FILE *fp = NULL;
//	int flag = 1;
	int ret = 0;
	strcpy(user.id, id);
	strcpy(user.pwd, MDString(pwd));

	printf("Content-type:text/html\r\n\r\n");
	if (!id || !pwd || !pwd1) {
		printf("NO input!!\n");
		return -1;
	}

	if (strcmp(pwd1,pwd)){
		printf("two passwords are not the same\n");
		return -1;
	}
	
	if ((fp = fopen("passwd.dat", "ab+")) == NULL){		
		printf("file could not be opened\n");
		return -1;
	}
/* 	while (!feof(fp)){
		fread(&user1, sizeof(struct usr), 1 ,fp);
		printf("user1.id = %s user1.pwd =  %s\n", user1.id, user1.pwd);
		if (!(strcmp(id, user1.id))){
         		flag = 0;
         		break;
		}
	}*/
	ret = find_user(fp, id);
	if (ret != -1){
		printf("the system has already had this user\n");
		return -1;
	}
	
	rewind(fp);
	fwrite(&user, sizeof(struct usr), 1, fp);
	printf("add user successfully\n");
	fclose(fp);
	return 0;
}
