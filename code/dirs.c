#include <stdio.h>
#include <dirent.h>

int main(void) {
	DIR *dir = opendir(".");
	struct dirent *dp;
	int i = 0;
	while ( (dp = readdir(dir)) != NULL ) {
		if ( DT_DIR == dp->d_type ) {
			printf("D ");
		} else {
			printf("F ");
		}
		printf("%s\n", dp->d_name);
	}
	closedir(dir);
	return 0;
}