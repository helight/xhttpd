/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author: huzhenbi<huzhenbi05@sina.com>
 * Created Time: Wed 27 May 2009 11:19:11 AM CST
 * File Name:    pmaps.c
 *
 * Description:  show the memery information of each process

 * Modified: Helight.Xu<Helight.Xu@gmail.com>>
 */

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <ctype.h>

int main()
{
    DIR  * dir ;
    DIR  * dir1;
    FILE * fp;
    struct dirent  * ptr ;
    struct dirent  * ptr1;
    char s[1024];
    char path[50];
    char path1[50];
    int i;
    char * maps1;
    char * maps2;
    char * maps3;
    char * maps4;
    char * maps5;
    char * maps6;
    char maps21[10];
    char maps61[10];
    char maps62[10];

    printf("Content-type:text/html\r\n\r\n");
    printf("<html><head></head><body><div align='left'>");
    printf("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

    if ((dir = opendir("/proc")) == NULL) {
        perror(" opendir") ;
        return -1;
    }
    while ((ptr = readdir(dir)) != NULL) {
        if (isdigit(ptr->d_name[0])) {

            printf("<div><table style='border-width:2px; border-collapse:collapse; \
                   border-color:red'; border='2'><tr><td>process number:%s</td></tr>\
                   </table></div>", ptr->d_name);
            strcpy(path, "/proc/");
            strcat(path, ptr->d_name);

            if ((dir1 = opendir(path)) == NULL) {
                perror("opendir") ;
                return -1;
            }
            while ((ptr1 = readdir(dir1)) != NULL) {
                if (!strcmp(ptr1->d_name, "maps")) {
                    printf("<div><table style ='border-width:5px; border-collapse:collapse; \
                           border-color:#222796'; border='2'><tr><td width='550px'>文件名</td>\
                           <td width='50px'>类型</td><td width='140px'>内存映像 </td>\
                           <td width='80px'>虚存偏移量 </td><td width='50px'>inode</td></tr></div>");

                    strcpy(path1, path);
                    strcat(path1, "/maps");
                    fp = fopen(path1, "r");
                    while (!feof(fp)) {
                        fgets(s, 1024, fp);
                        maps1 = strtok(s, " ");
                        maps2 = strtok(NULL, " ");
                        maps3 = strtok(NULL, " ");
                        maps4 = strtok(NULL, " ");
                        maps5 = strtok(NULL, " ");
                        maps6 = strtok(NULL, " ");

                        maps4 = maps4;
                        if ((maps2 != NULL) && (maps6 != NULL)) {
                            strcpy(maps21, maps2);
                            for (i = 0; i <= 5; i++) {
                                maps61[i] = maps6[i];
                            }
                            maps61[i] = '\0';
                            for (i = 0; i <= 6; i++) {
                                maps62[i] = maps6[i];
                            }
                            maps62[i] = '\0';    //(1)shuzu  (2)\0

                            if (!strcmp(maps61, "[heap]")) {
                                printf("<tr><td width='550px'>&nbsp</td><td width='50px'>堆</td>\
                                       <td width='140px'>%s </td><td width='80px'>%s</td>\
                                       <td width='50px'>%s</td></tr>", maps1, maps3, maps5);

                            } else if (!strcmp(maps62, "[stack]")) {
                                printf("<tr><td width='550px'>&nbsp</td><td width='50px'>栈</td>\
                                       <td width='140px'>%s </td><td width='80px'>%s</td><td width='50px'>%s\
                                       </td></tr>", maps1, maps3, maps5);
                            }  else if (maps21[1] == 'w') {
                                printf("<tr><td width='550px'>%s</td><td width='50px'>\
                                       数据段</td><td width='140px'>%s </td><td width='80px'>%s\
                                       </td><td width='50px'>%s</td></tr>", maps6, maps1, maps3, maps5);
                            } else if (maps21[1] == '-') {
                                printf("<tr><td width='550px'>%s</td><td width='50px'>\
                                       程序段</td><td width='140px'>%s </td><td width='80px'>%s\
                                       </td><td width='50px'>%s</td></tr>", maps6, maps1, maps3, maps5);
                            }
                        }
                    }
                    fclose(fp);
                    printf("</table>");
                }
            }
            closedir(dir1);
        }
    }
    closedir(dir);
    printf("</div></body></html>");

    return 0;
}

