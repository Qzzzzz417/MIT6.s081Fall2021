#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXARGLEN 256
void get_single_arg(char* buf, int* offset, int* start, int* len);
int main(int argc, char* argv[]) {
	
	char buf[512];
	char addarg[MAXARG];
	char command[MAXARGLEN + 1];
	int offset;
	int start, len;

							
	memset(addarg, 0, MAXARG);

	memset(command, 0, MAXARGLEN+1);
	if (argc < 2) {
		fprintf(2, "Usage: xargs command [commandargument...]\n");
		exit(1);
	}
	memcpy(command, argv[1], sizeof(argv[1]));
	while (read(0, buf, MAXARG)) {
//		offset = 0;
//		start = 0;
//		len = 0;
//		while (offset < strlen(buf)) {
//			get_single_arg(buf, &offset, &start, &len);
//			memcpy(addarg, buf + start, len);
//			fprintf(1, "addarg: %s\n", addarg);
			exec_command(commmand, argv, 2, argc - 1, addarg);		
		}		
	}

	
	exit(0);
}
void get_single_arg(char* buf, int* offset, int* start, int* len) {
	int i;
	*start = *offset;
	*len = 0;
	for (i = *offset; i < strlen(buf);i++) {
		if (buf[i] == '\n') {
			*offset = i + 1;
			break;
		}	
		(*len)++;
		(*offset)++;
	}
}
void exec_command(char* command, char* argv[], int start, int end, char* addarg) {
	
	 }
