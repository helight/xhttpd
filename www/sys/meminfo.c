/*
 * XiYou Simple HTTP server
 *
 * Copyright (c) 2008-~  Xiyou Linux Group
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       huzhenbi<huzhenbi05@sina.com>
 * Created Time: Wed 27 May 2009 11:19:11 AM CST
 * File Name:    meminfo.c
 *
 * Description:  show the information of the system memery
 
 * Modified: Helight.Xu<Helight.Xu@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int memtotal ;

int main()
{
	char s[255];
	char alph[100];
	int data;
	int i = 0;
	int j;
	FILE *fp;
	int count = 1;
	
	fp=fopen("/proc/meminfo","r");
	/* the length of every line
	len = strlen(s);
	printf("\n*****%d\n", len); //26
	*/
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head></head><body><div align='left'>");
	printf("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");
	while (!feof(fp) && count <= 23) {
		fgets(s, 255, fp) ;
		count++ ;
		//READ alpha
		 i = 0;
		j = 0;
	while (i < 26 && (isalpha(s[i]) || s[i] == '_')) {
		alph[i] = s[i];
		j++;
		i++ ;
	}
	alph[j] = '\0' ;

	//READ digit
	i = 0;
	j = 0;
	data = 0;
	while (i < 26) {
		if (isdigit(s[i])) {
			data = data * 10 + s[i] - 48 ;
			j++;
		}
		i++ ;
	}

	printf("<div><table style='border:2px; border-color:red'; >");
	if (!strcmp(alph, "MemTotal")) {
		memtotal = data;
		printf("<tr><td width='180px'>所有可用RAM大小: %d KB</td>", data);  
		printf("<td  style='width: %dpx; helight:20px; background-color:#f60;'>\
				</td></tr>", 220);

	} else if (!strcmp(alph, "MemFree")) {
		printf("<tr><td width='180px'>未使用的内存: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#5791fc;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Buffers")) {
		printf("<tr><td width='180px'>用来给文件做缓冲大小: %d KB</td>", data);   
		printf("<td style='width: %dpx; helight:20px; color:#222796;\
			background-color:#556600;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Cached")) {
		printf("<tr><td width='180px'>高速缓冲用的内存大小: %d KB</td>", data);  
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:fuchsia;' ></td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "SwapCached")) {
		printf("<tr><td width='180px'>高速缓冲交换空间大小: %d KB</td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:olive;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Active")) {
		printf("<tr><td width='180px'>经常使用页面大小: %d KB</td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Inactive")) {
		printf("<tr><td width='180px'>不经常使用页面大小: %d KB</td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "HighTotal")) {
		printf("<tr><td width='180px'>高位内存总大小: %d KB</td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "HighFree")) {
		printf("<tr><td width='180px'>高位内存可用大小: %d KB </td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "LowTotal")) {
		printf("<tr><td width='180px'>低位内存总大小: %d KB</td>", data); 
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "LowFree")) {
		printf("<tr><td width='180px'>低位可用内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
		background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "SwapTotal")) {
		printf("<tr><td width='180px'>交换空间的总和: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:teal;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "SwapFree")) {
		printf("<tr><td width='180px'>未使用的交换空间大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Dirty")) {
		printf("<tr><td width='180px'>等待写回磁盘的内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Writeback")) {
		printf("<tr><td width='180px'>正写回磁盘的内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Mapped")) {
		printf("<tr><td width='180px'>影射文件的大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "Slab")) {
		printf("<tr><td width='180px'>内核数据结构缓存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "CommitLimit")) {
		printf("<tr><td width='180px'>系统可分配内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);


	} else if (!strcmp(alph, "Committed_AS")) {
		printf("<tr><td width='180px'>确保内存不溢出的RAM估计值: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "PageTables")) {
		printf("<tr><td width='180px'>内存分页页面索引表大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "VmallocTotal")) {
		printf("<tr><td width='180px'>可以vmalloc虚拟内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "VmallocUsed")) {
		printf("<tr><td width='180px'>已使用的虚拟内存大小: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (!strcmp(alph, "VmallocChunk")) {
		printf("<tr><td width='180px'>最大的vmalloc空闲区间: %d KB</td>", data);
		printf("<td style='width: %dpx; helight:20px; color:#222796; \
			background-color:#22444b;' > </td></tr>",data * 220 / memtotal);

	} else if (feof(fp)) {
		break;
	} else {
		printf("<tr><td>ERROR</tr></tr>");
  	}
   }

fclose(fp);
printf("</table></div>");
printf("</div></body></html>");

return 0;
 
}

