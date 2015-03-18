#include <stdio.h>

struct usr {
         char id[32];
         char pwd[32];
};
	
int find_user(FILE *fp,char *id)
{
	int ret = 0;
	int flag = 1;
	struct usr user;
	while (!feof(fp)){
		fread(&user,sizeof(struct usr),1,fp);
 		if (!(strcmp(id,user.id))){
         		flag = 0;
         		break;
		}
		ret++;
	}
	if (flag == 1) {
		ret = -1;
	}
	return ret;
}
