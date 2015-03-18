/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License.
 *
 * file name: decode.c
 * modify date: 2009-11-28
 * Author: Helight.Xu<Helight.Xu@gmail.com>
 *
 * Description:
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void decode(char *ch);

int main()
{
	char ch[]="123%E6%88%91567";
	char chat[20];
	int i = 0, j = 0, n = 0;
	memset(chat, '\0', sizeof(chat));	
	decode(ch);
	printf("decode: %s", ch);	
	
	setenv("XY_KK","zhwen许", 1);
	char *kk = getenv("XY_KK");
	printf("kk: %s \n",kk);
	unsetenv("XY_KK");
	return 0;
}

static void decode(char *ch)
{
        int n = 0, i = 0, j = 0;
        n = strlen(ch);
        for(i=0; i<n; i++){
                if(ch[i] == '%'){
                        i++;
                        if(ch[i] >= 'a') ch[i] = ch[i] - 'a'+10;
                        else if(ch[i] >= 'A') ch[i] = ch[i] - 'A'+10;
                        else if(ch[i] >= '0') ch[i] = ch[i] - '0';

                        if(ch[i+1] >= 'a') ch[i+1] = ch[i+1] - 'a'+10;
                        else if(ch[i+1] >= 'A') ch[i+1] = ch[i+1] - 'A'+10;
                        else if(ch[i+1] >= '0') ch[i+1] = ch[i+1] - '0';

                        printf("%02x-%02x ",ch[i], ch[i+1]);
                        ch[j] = ch[i]*16 + ch[i+1];
                        printf("kk:%02x the I:%d the J:%d\n", ch[j], i, j);
                        j++; i++;
                } else{
                        ch[j++] = ch[i];
                }

        }
	ch[j]='\0';
} 

 
