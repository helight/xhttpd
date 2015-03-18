#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int 	fk;
	int	fp[2];
	int	ret = 0;
	
	if ((ret = pipe(fp)) < 0) {
		perror("pipe:\n");
		return -1;
	}
	
	if ((fk = fork()) < 0) {
		perror("fork\n");
		exit(0);
	}
	if (fk == 0) {
		usleep(10);
		char buf[1024];
		close(fp[1]);
		close(0);
		dup2(fp[0], 0);
		//close(1);	//stdout
		//dup2(fd, 1);  //stdout
		read(0, buf, sizeof(buf));
		//read(fp[0], buf, sizeof(buf));
		printf("buf:%s\n", buf);
		exit(0);
	} else if (fk > 0) {
		char *buf = NULL;
		char values[] = "hello world!";
		close(fp[0]);
		if (buf)
			write(fp[1], "NULL", 5);
		else
			write(fp[1], values, sizeof(values));
			wait(NULL);
			return 0;
	}
	
	return 0;
}
