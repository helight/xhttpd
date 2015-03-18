/*
 * Copyright (c) 2009-~ Helight.Xu
 *
 * This source code is released for free distribution under the terms of the
 * GNU General Public License
 *
 * Author:       Helight.Xu<Helight.Xu@gmail.com>
 * Created Time: Wed 27 May 2009 11:19:11 AM CST
 * File Name:    mtdblock.c
 *
 * Description:  show the information of the system's partitions
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

enum filename {capability,removable,size,dev,range};
const char *filenames[]={"capability","removable","size","dev","range"};

enum devname {
	iocounterbits,
	generic,
	driver,
	power,
	subsystem,
	rescan,
	delete,
	device_blocked,
	evt_media_change,
	iodone_cnt,
	ioerr_cnt,
	iorequest_cnt,
	modalias,model,
	queue_depth,
	queue_type,
	rev,
	scsi_level,
	state,
	timeout,
	type,
	uevent,
	vendor
};
const char *devnames[]= {
	"iocounterbits",
	"generic",
	"driver",
	"power",
	"subsystem",
	"rescan",
	"delete",
	"device_blocked",
	"evt_media_change",
	"iodone_cnt",
	"ioerr_cnt",
	"iorequest_cnt",
	"modalias",
	"model",
	"queue_depth",
	"queue_type",
	"rev",
	"scsi_level",
	"state",
	"timeout",
	"type",
	"uevent",
	"vendor"
};

void usage(char *name)
{
	printf("<tr><td width='100px'>%s </td><td width='100px'> \
	</td><td width='400px'>source directory is rong!</td></tr>",name);
	
	exit(1);
}

int stat_file(char *depth)
{
	FILE *fp;
	char buff[1024];
	char *delim=" ";
	char *p;
	int i=0;

	if ((fp=fopen(depth,"r"))==NULL) {
		printf("<tr><td width='100px'>%s </td><td width='100px'> \
		</td><td width='400px'>Could not open source file</td></tr>",depth);
		exit(1);
	}
	
	fgets(buff,1024,fp);
	printf("<tr><td width='100px'> </td><td width='100px'>%s \
	</td><td width='400px'>成功读取磁盘的次数，成功完成读的总次数</td></tr>",strtok(buff,delim)); 
	i++;
	
	while ((p=strtok(NULL,delim))&&i++) {
		if(i==2)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>合并读次数</td></tr>",p); 
		else if(i==3)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>读扇区的次数，成功读过的扇区的总次数</td></tr>",p); 
		else if(i==4)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>读花费的毫秒数，这是所有读操作所花费的毫秒数。</td></tr>",p); 
		else if(i==5)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>写完成的次数，成功写完成的总次数。</td></tr>",p); 
		else if(i==6)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>合并写次数。</td></tr>",p); 
		//     printf("%s：合并写次数。\n",p);
		else if(i==7) 
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>写扇区的次数，成功写扇区的总次数。</td></tr>",p);  
		else if(i==8)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>写花费的毫秒数，这是所有写操作所花费的毫秒数</td></tr>",p); 
		else if(i==9)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>I／O的当前进度，只是这个域应该是0。当请求被\
			交给适当的request_queue_t时增加和请求完成时减少。</td></tr>",p); 
		else if(i==10)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>花在I/O操作上的毫秒数，这个域会增长只要field 9不为0。</td></tr>",p); 
		else if(i==11)
			printf("<tr><td width='100px'> </td><td width='100px'>%s \
			</td><td width='400px'>加权，花在I/O操作上的毫秒数，在每次I/O开始，\
			I/O结束，I/O合并时这个域都会增加。这可以给I/O完成时间和存储那些可以累计的\
			提供一个便利的测量标准。</td></tr>",p); 
	}
	fclose(fp);
	
	return 0;
}

int other_file(char *file,char *depth)
{  
	FILE *fp;
	char buf[1024];
	enum filename i;

	for (i=capability; i<=range; i++)
	{
		if (strcmp(file,filenames[i]) == 0)
			break;
	}
	switch (i) {
		case capability: 
			if((fp=fopen(depth,"r"))!=0)
				fgets(buf,1024,fp);
			printf("<tr><td width='100px'>%s</td><td width='100px'>%s \
			</td><td width='400px'>该数据表示：磁盘驱动器的尺寸.</td></tr>", \
			filenames[i],buf);   		
			break;
		case removable : 
			if((fp=fopen(depth,"r"))!=0)
				fgets(buf,1024,fp);
			printf("<tr><td width='100px'>%s</td><td width='100px'>%s \
			</td><td width='400px'>该数据表示：硬盘是否移出.</td></tr>",\
			filenames[i],buf);     
			break;               
		case size      : 
			if((fp=fopen(depth,"r"))!=0)
				fgets(buf,1024,fp);
			printf("<tr><td width='100px'>%s</td><td width='100px'>%s \
			</td><td width='400px'>该数据表示：读出磁盘容量的大小。</td></tr>",\
			filenames[i],buf);     
			break;
		case dev       : 
			if((fp=fopen(depth,"r"))!=0)
				fgets(buf,1024,fp);
			printf("<tr><td width='100px'>%s</td><td width='100px'>%s \
			</td><td width='400px'>该数据表示：读出设备块信息。冒号前的数据是主\
			设备号，冒号后的数据次设备号</td></tr>",filenames[i],buf);     
			break;
		case range     :
			if((fp=fopen(depth,"r"))!=0)
				fgets(buf,1024,fp);
			printf("<tr><td width='100px'>%s</td><td width='100px'>%s \
			</td><td width='400px'>该数据表示：读出次设备号的最大序号.</td></tr>",\
			filenames[i],buf);     
			break;
		default        :
			printf("<tr><td width='100px'>!!!</td><td width='100px'> wrong!\
			</td><td width='400px'>参数传递错误</td></tr>"); 
		break;
	} 
	
	return 0;
}

void devicefile(char *device)
{
	FILE *fp;
	char path[1024];
	char buf[256];
	enum devname i;
	DIR *dirp;
	struct dirent *direntp;

	if((dirp=opendir(device))==NULL)
	{
	printf("<tr><td width='100px'>%s </td><td width='100px'> </td><td width='400px'>Could not open source file</td></tr>",device);
	}
	while((direntp=readdir(dirp))!=NULL)
	{
	if((strcmp(direntp->d_name,".")!=0)&&(strcmp(direntp->d_name,"..")!=0)&&(strcmp(direntp->d_name,"rescan"))&&(strcmp(direntp->d_name,"delete")))
	{
	for(i=iocounterbits;i<=vendor;i++)
	{
	if(strcmp(direntp->d_name,devnames[i])==0)
	break;
	}
	switch(i)
	{
	case iocounterbits: 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示io发生的次数.</td></tr>",devnames[i],buf);  
	break;
	case generic      :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>  该文件是链接文件，generic</td></tr>",devnames[i],buf);  
	break;
	case driver       : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件是链接文件，表示驱动的意思。</td></tr>",devnames[i],buf); 
	break;
	case power        : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件是目录文件，表示能量，内容是running</td></tr>",devnames[i],buf); 
	break;
	case subsystem    :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件是链接文件，子系统，链接到block目录下去。</td></tr>",devnames[i],buf); 
	break;
	case rescan       :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件是普通文件，是只能用户读文件，故不做相应分析。</td></tr>",devnames[i],buf); 
	break;
	case delete       : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 删除</td></tr>",devnames[i],buf);  
	break;
	case device_blocked :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 块设备.</td></tr>",devnames[i],buf);  
	break;
	case evt_media_change :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 检查可移动介质是否已经发生变化.</td></tr>",devnames[i],buf);  
	break;
	case iodone_cnt   :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示已经发生的io的次数.</td></tr>",devnames[i],buf);  
	break;
	case ioerr_cnt    :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示io出错的次数.</td></tr>",devnames[i],buf);  
	break;
	case iorequest_cnt: 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>   该数据表示io请求的次数</td></tr>",devnames[i],buf);   
	break;
	case modalias     : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：还不晓得～～。</td></tr>",devnames[i],buf);  
	break;
	case model        : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>  该数据表示：模型。</td></tr>",devnames[i],buf);  
	break;
	case queue_depth  : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：队列深度.</td></tr>",devnames[i],buf);  
	break;
	case queue_type   : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：队列类型</td></tr>",devnames[i],buf);  
	break;
	case rev          : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>  该数据表示：需要去Google~~.</td></tr>",devnames[i],buf);  
	break;
	case scsi_level   : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：总线类型.</td></tr>",devnames[i],buf);  
	break;
	case state        : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>  该数据表示：状态.</td></tr>",devnames[i],buf);  
	break;
	case timeout      :
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：时钟次数.</td></tr>",devnames[i],buf);  
	break;
	case type         : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'> 该数据表示：类型</td></tr>",devnames[i],buf);  
	break;
	case uevent       : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件的信息表示：触发时候发生的事件</td></tr>",devnames[i],buf);  
	break;
	case vendor       : 
	sprintf(path,"%s/%s",device,devnames[i]);
	if((fp=fopen(path,"r"))!=0)
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>  该数据表示：这个就真的不知道咯～～。</td></tr>",devnames[i],buf);  
	break;  
	default           : 

	printf("<tr><td width='100px'> </td><td width='100px'> </td><td width='400px'> *********device文件分析结束*********</td></tr>");
	break;
	}
	}
	}
}


char buf[1024];
void openfile(char load[])
{
	FILE *fp;

	if((fp=fopen(load,"r"))==NULL){
		printf("<tr><td width='100px'> </td><td width='100px'> \
		</td><td width='400px'> Could not open the file!</td></tr>");
		exit(1);
	}else{
		fgets(buf,1024,fp);
		//printf("%s     ",buf);
	}
}

void iosched(char path[])   //处理iosched目录文件
{
	char s[256];
	char t[256];
	DIR *dirfp;
	struct dirent *direntfp;

	if((dirfp=opendir(path))==NULL)
	{
	printf("<tr><td width='100px'> %s</td><td width='100px'> </td><td width='400px'> Could not open the file!</td></tr>",path);      exit(1);
	}
	strcpy(s,path);
	if(s[strlen(s)-1]!='/');
	strcat(s,"/");
	while((direntfp=readdir(dirfp))!=NULL)
	{
	if(strcmp(direntfp->d_name,"back_seek_max")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td>\
	<td width='400px'>向后寻找最大数, 用KiB描述.</td></tr>",direntfp->d_name,buf); 
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"back_seek_penalty")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>向后寻找最大数, 用KiB描述.</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"fifo_expire_async")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>3.</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"fifo_expire_sync")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>4</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"quantum")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>max queue in one round of service.第一轮服务的最大队列。</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"slice_async")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>Base length of an asynchronous queue timeslice (that is, how long the queue has to dispatch requests each round). The actual timeslice length is scaled by the I/O priority.相应的长度异步队列时间片（即多久队列已派遣要求每一轮） 。实际的时间片长度比例由I / O的优先事项。</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"slice_async_rq")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>The base number of requests per round for asynchronous queues. Like slice_async, the actual maximum is a function of slice_async_rq and I/O priority.</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"slice_idle")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>How long to wait for processes to produce more I/O before switching queues. This is for anticipation of sequential I/O, and more even disk time distribution for processes doing back to back synchronous I/Os.</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	if(strcmp(direntfp->d_name,"slice_sync")==0)
	{
	strcpy(t,s);
	strcat(t,direntfp->d_name);
	openfile(t);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>Same as slice_async, but for synchronous requests.</td></tr>",direntfp->d_name,buf);
	t[255]='\0';
	}
	}
	closedir(dirfp);
}

void queuefile(char queue[])
{
	char sourcefile[256];
	DIR *dirp;
	struct dirent *direntp;

	if((dirp=opendir(queue))==NULL)	{
		printf("<tr><td width='100px'>%s</td><td width='100px'>%sdd</td>\
		<td width='400px'>Could not open directory</td></tr>", queue, queue);
		exit(1);
	}
	while((direntp=readdir(dirp))!=NULL)
	{
	strcpy(sourcefile,queue);
	if(sourcefile[strlen(sourcefile)-1]!='/');
	strcat(sourcefile,"/");

	if(strcmp(direntp->d_name,".")!=0&&(strcmp(direntp->d_name,"..")!=0))
	{
	strcat(sourcefile,direntp->d_name);

	if(strcmp(direntp->d_name,"max_sectors_kb")==0)
	{
	openfile(sourcefile);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%sdd</td><td width='400px'>能处理的最大尺寸（以扇区为单位）</td></tr>",direntp->d_name,buf);
	}
	else if(strcmp(direntp->d_name,"read_ahead_kb")==0)
	{
	openfile(sourcefile);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s</td><td width='400px'>预读硬盘大小（以kb为单位）</td></tr>",direntp->d_name,buf);
	}
	else if(strcmp(direntp->d_name,"max_hw_sectors_kb")==0)
	{
	openfile(sourcefile);
	printf("<tr><td width='100px'>%s</td><td width='100px'> %s</td><td width='400px'>能处理的DMA的最的尺寸（以扇区为单位）</td></tr>",direntp->d_name,buf); 
	}
	else if(strcmp(direntp->d_name,"nr_requests")==0)
	{
	openfile(sourcefile);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>最大请示数</td></tr>",direntp->d_name,buf); 
	}
	else if(strcmp(direntp->d_name,"scheduler")==0)
	{
	openfile(sourcefile);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>规则</td></tr>",direntp->d_name,buf); 
	}
	else if(strcmp(direntp->d_name,"iosched")==0)
	{
	iosched(sourcefile);
	}
	else
	printf("<tr><td width='100px'> </td><td width='100px'> </td><td width='400px'>**********queue文件分析结束！！！***********</td></tr>"); 
	}
	}
	closedir(dirp);
}



void sdafile(char pt[])
{
	FILE *fp;
	char buf[1024];
	char path[1024];
	DIR *dir;
	struct dirent *dirent;

	char *del=" ";
	char *t;
	int j;

	if((dir=opendir(pt))==NULL){
		printf("<tr><td width='100px'>%s</td><td width='100px'>%sdd</td>\
		<td width='400px'>Could not open directory</td></tr>", pt, pt); 
		exit(1);
	}
	while((dirent=readdir(dir))!=NULL)
	{
	strcpy(path,pt);
	if(path[strlen(path)-1]!='/');
	strcat(path,"/");
	if(strcmp(dirent->d_name,".")!=0&&strcmp(dirent->d_name,"..")!=0)
	{
	if(strcmp(dirent->d_name,"holders")==0)
	{
	printf("<tr><td width='100px'>%s</td><td width='100px'> </td><td width='400px'>该文件是目录文件，且为空。</td></tr>",dirent->d_name);
	}
	else if(strcmp(dirent->d_name,"subsystem")==0)
	{
	printf("<tr><td width='100px'>%s</td><td width='100px'> </td><td width='400px'>该文件是符号链接文件，连接到/sys/block目录下</td></tr>",dirent->d_name);
	}
	else if(strcmp(dirent->d_name,"uevent")==0)
	{
	printf("<tr><td width='100px'>%s</td><td width='100px'> </td><td width='400px'>该文件是用户只读文件，存放设备热插拔事件发生时的信息</td></tr>",dirent->d_name);
	}
	else if(strcmp(dirent->d_name,"dev")==0)
	{
	strcat(path,dirent->d_name);
	if((fp=fopen(path,"r"))==NULL)
	{
	printf("Could not open the file:%s\n",dirent->d_name);
	exit(1);
	}
	if(fgets(buf,1024,fp)>0)
	{
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>该文件硬盘信息，表示设备类型。前一个数字表示主设备号，后一个数字表示次设备号</td></tr>",dirent->d_name,buf);
	} 
	fclose(fp);
	}
	else if(strcmp(dirent->d_name,"start")==0)
	{
	strcat(path,dirent->d_name);
	if((fp=fopen(path,"r"))==NULL)
	{
	printf("Could not open the file:%s\n",dirent->d_name);
	exit(1);
	}
	if(fgets(buf,1024,fp)>0){
		printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td>\
		<td width='400px'>该文件表示%s分区的开始位置</td></tr>",dirent->d_name, buf, buf);
	} 
	fclose(fp);
	}
	else if(strcmp(dirent->d_name,"size")==0)
	{
	strcat(path,dirent->d_name);
	if((fp=fopen(path,"r"))==NULL)
	{
	printf("Could not open the file:%s\n",dirent->d_name);
	exit(1);
	}
	if(fgets(buf,1024,fp)>0){
		printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td>\
		<td width='400px'>该文件表示%s分区的大小</td></tr>", dirent->d_name, buf, buf);
	}
	fclose(fp);
	}
	else if(strcmp(dirent->d_name,"stat")==0)
	{
	strcat(path,dirent->d_name);    

	j=0;
	if((fp=fopen(path,"r"))==NULL)
	{
	printf("Could not open sourcefile:%s\n",path);
	exit(1);
	}
	else 
	{
	fgets(buf,1024,fp);
	printf("<tr><td width='100px'>%s</td><td width='100px'>%s </td><td width='400px'>Field  1 -- # of reads issued /This is the total number of reads issued to this partition</td></tr>",dirent->d_name,strtok(buf,del));
	j++;
	while((t=strtok(NULL,del))&&j++)
	{
	if(j==2)
	printf("<tr><td width='100px'> </td><td width='100px'>%s </td><td width='400px'> Field  2 -- # of sectors read This is the total number of sectors requested to be read from this partition</td></tr>" ,t);           
	else if(j==3)
	printf("<tr><td width='100px'> </td><td width='100px'>%s </td><td width='400px'>Field  3 -- # of writes issued This is the total number of writes issued to this partition</td></tr>", t);           
	else if(j==4)
	printf("<tr><td width='100px'> </td><td width='100px'>%s </td><td width='400px'>Field  4 -- # of sectors written This is the total number of sectors requested to be written to this partition</td></tr>", t);  
	}
	}
	fclose(fp);   
	}

	}
	}
	closedir(dir);
}



void dealwith(char *sourcedir)
{
	char sourcefile[256];
	static char name[256];
	DIR *dirp;
	struct dirent *direntp;
	struct stat st;

	printf("<html><head></head><body><div align='left'>");
	printf("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");
	printf("<div><table style ='border-width:5px; border-collapse:collapse; border-color:#222796';\
	border='2'><tr><td width='100px'>文件名</td><td width='100px'>内容</td><td width='400px'>解析 </td></tr>");
	if ((dirp=opendir(sourcedir)) == NULL) {
		printf("Could not open directory:%s \n",sourcedir);
		exit(1);
	}

	while ((direntp = readdir(dirp)) != NULL) {
		strcpy(name,direntp->d_name);
		strcpy(sourcefile,sourcedir);
		if(sourcefile[strlen(sourcefile)-1]!='/');
		strcat(sourcefile,"/");

		if(strcmp(direntp->d_name,".")!=0&&(strcmp(direntp->d_name,"..")!=0)){
		strcat(sourcefile,direntp->d_name);

		if(lstat(sourcefile,&st)==-1){
			printf("error:stat\n");
			exit(1);
		}

		if(S_ISDIR(st.st_mode))	{  
			if(strcmp(name,"holders")==0){  
				printf("<tr><td width='100px'>%s</td><td width='100px'> \
				</td><td width='400px'>该文件为空,表示持有者</td></tr>",name);
			} else if(strcmp(name,"slaves")==0){
				printf("<tr><td width='100px'>%s</td><td width='100px'>目录文件\
				</td><td width='400px'>该文件为空，表示队列</td></tr>",name);
			}else if((strcmp(name,"sda1")==0)||(strcmp(name,"sda2")==0)||strcmp(name,"sda5")==0){
				printf("<tr><td width='100px'>%s</td><td width='100px'>目录文件</td>\
				<td width='400px'>磁盘分区目录，通过该结果可了解硬盘被分为四个分区，分别为0、1、2、5分区\
				</td></tr>",name);
				sdafile(sourcefile);
				printf("<tr><td width='100px'>*******************</td>\
				<td width='100px'>***********************</td>\
				<td width='400px'>**********%s文件分析结束！！！************</td></tr>",name);
			}else if(strcmp(name,"queue")==0){
				printf("<tr><td width='100px'>%s</td><td width='100px'>目录文件\
				</td><td width='400px'>队列文件</td></tr>",name);
				queuefile(sourcefile);//需要插入“queue的功能函数”
				printf("<tr><td width='100px'>********************</td>\
				<td width='100px'>***********************</td>\
				<td width='400px'>**********%s文件分析结束！！！************</td>\
				</tr>",name);          
			}      
		}else if(S_ISLNK(st.st_mode)){
			if(strcmp(name,"subsystem")==0)	{
				printf("<tr><td width='100px'>%s</td>\
				<td width='100px'>链接文件</td><td width='400px'>\
				该文件是子系统链接文件。链接到/sys/block下去。故不做分析。\
				</td></tr>",name);
			} 
			if(strcmp(name,"device")==0){
				printf("<tr><td width='100px'>%s</td><td width='100px'> \
				</td><td width='400px'>该文件是设备链接文件.\
				指向/sys/devices/pci0000:00中嵌入的一个子目录。</td></tr>",name);
				devicefile(sourcefile);
				printf("<tr><td width='100px'>********************</td>\
				<td width='100px'>**********************</td>\
				<td width='400px'>**********%s文件分析结束！！！************</td></tr>",name); 
			}
		}else{
			if( S_IWUSR & (st.st_mode)){
				printf("<tr><td width='100px'>%s</td>\
				<td width='100px'> </td><td width='400px'>\
				该文件为只能用户写文件，在有设备插入时被写入信息</td></tr>",name);        
			}else{
				if(strcmp(name,"stat")==0){
				printf("<tr><td width='100px'>%s</td><td width='100px'> </td>\
				<td width='400px'><body text='black'>读取硬盘统计的信息</td></tr>",name);
				stat_file(sourcefile);  //功能函数调用
				printf("<tr><td width='100px'>********************</td>\
				<td width='100px'>**********************</td>\
				<td width='400px'>**********%s文件分析结束！！！************</td></tr>",name); 
			}else{         
				other_file(name,sourcefile);  //功能函数调用
				printf("<tr><td width='100px'>*********************</td>\
				<td width='100px'>**********************</td>\
				<td width='400px'>*******%s文件分析结束！！！*********</td></tr>",name); 
			}   
			}
		}
		}
	}
	closedir(dirp);
	printf("</table></div>"); 
	printf("</body></html>");
}

int main(int argc,char * *argv)
{
	char pathfile[128]; 
	int i = 0;

	printf("Content-type:text/html\r\n\r\n");
	for (i = 0; i < 6; i++) {
		snprintf(pathfile, sizeof(pathfile), "%s%d", "/sys/block/mtdblock", i);
		printf("%s\n", pathfile);
		dealwith(pathfile);
	}

	exit(0);
}
