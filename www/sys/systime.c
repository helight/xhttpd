#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
	char *cookie = getenv("HTTP_COOKIE");
        time_t the_time;

        the_time = time((time_t *)0); /*调用time系统调用*/
	printf("Content-type:text/html\r\n\r\n");
	printf("<html><head><title>SYSTEM TIME</title>"
		"<meta http-equiv='Content-Type' content='text/html;"
		" charset=utf-8'/></head>");
	printf("<body><div width=900px><div align=center>"
		"<h1>SYSTEM TIME</h1></div><hr> <div>\n");	
        printf("The time is %s<br><br><br>", ctime(&the_time));
        printf("cookie %s<br>", cookie);
	printf("</div><hr></body></html>\n");	
	return 0;
}
