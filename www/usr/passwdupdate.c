/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: passwdupdate.c 
 * modify date: 2009-12-05
 * Author: susu1399@sina.com  
 * modify by: Helight.Xu@Gmail.com 2009-12-05
 *
 * Program definitions:update an user's info
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <md5lib.h>
#include <cgilib.h>
#include <user.h>

int main()
{
	FILE *fp = NULL;
	struct usr user = {"", ""};
//	struct usr user1 = {"", ""};
	char *id = getenv("XY_id");
	char *pwd = getenv("New_XY_pwd");
	char *pwd1 = getenv("Re_New_XY_pwd");
	int ret = 0;
//	int flag = 1;
	strcpy(user.id, id);
	strcpy(user.pwd, MDString(pwd));

	printf("Content-type:text/html\r\n\r\n");	
	if (!id || !pwd || !pwd1) {
		printf("NO input!!\n");
		return -1;
	}
	
	if (strcmp(pwd1, pwd)){
		printf("two passwords are not the same\n");
		return -1;
	}
	
	if ((fp = fopen("passwd.dat", "rb+")) == NULL){
		printf("file could not be opened\n");
		return -1;
	}
/*  	while (!feof(fp)){
		fread(&user1, sizeof(struct usr), 1, fp);
 		if (!(strcmp(id, user1.id))){
        	 	flag = 0;
         		break;
		}
		j++;
	}
*/
	ret = find_user(fp,id);
	if (ret == -1){
		printf("the system doesn't have this user\n");
		return -1;
	}

	fseek(fp, ret * sizeof(struct usr),SEEK_SET);
	fwrite(&user, sizeof(struct usr), 1, fp);
	printf("passwdupdate successfully\n");
	fclose(fp);
	return 0;
}

