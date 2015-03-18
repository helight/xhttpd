#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *XY_led = NULL;
	int status = 0;
	XY_led = getenv("XY_led");
	
	if(XY_led != NULL)
		status = atoi(XY_led);
	else
		status = -1;

	//sprintf(buf, "Set-Cookie:time=%s;\r\n", "helight");
	//printf(buf);
	printf("Content-type: text/html\r\n\r\n");

	printf("<html><head><title>Led控制</title>\
		<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/></head>");

	printf("<body><div width=900px><div align=center>\
		<h1>Led控制</h1></div><hr> <div>");
	printf("star\n");
	if(status == 1){
		char cmd[] = "insmod /mnt/led/led.ko";
		system(cmd);
		printf("led on!!!<br>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_on.gif'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_on.gif'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_on.gif'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_on.gif'\
			 width='38' heigth='23'></span><br>");
	} else if (status == 0){
		char cmd[] = "rmmod led";
		system(cmd);
		printf("led off!!!<br>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_off.jpg'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_off.jpg'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_off.jpg'\
			 width='38' heigth='23'></span>");
		printf("<span style='width:40px;height:50px;'>\
			<img border='0' src='../img/led_off.jpg'\
			 width='38' heigth='23'></span><br>");
	} 

	printf("status:%d", status);

	printf("<form action=led.xcgi method=post>");
	printf("<input name='XY_led' value='1' checked='checked'\
		 type='radio'><label >ON &nbsp;</label>");
	printf("<input name='XY_led' value='0' type='radio'>\
		<label >OFF &nbsp;</label>");

	printf("<br><input type=submit value=配置 >\
		<input type=reset value=重置 ></form>\
		</div><hr></div></body></html>\n");
return 0;
}
