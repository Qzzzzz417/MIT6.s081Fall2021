#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXARGLEN 256

int readline(char* new_argv[], int ori_argc); 
int divline(char* new_argv[], int ori_argc, char* buf, int len); 
// much source code reference: https://github.com/PKUFlyingPig/MIT6.S081-2020fall/blob/master/reports/Utils.md
int main(int argc, char* argv[]) {

	char* new_argv[MAXARG];

	char* command;

	if (argc < 2) {
		fprintf(2, "Usage: xargs command [args...]\n");
		exit(1);
	}


	command = malloc(strlen(argv[1]) + 1);
	memcpy(command, argv[1], strlen(argv[1]) + 1);

	// append original arguments 
	for (int i = 1; i < argc; i++) {
		new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
		new_argv[i - 1] = argv[i];
	}

	// append additional arguments
	while (readline(new_argv, argc - 1)) {
		
		int pid;
		pid = fork();
		if (pid == 0) {
			exec(command, new_argv);
			fprintf(2, "exec failed.\n");
			exit(1);
		}
		// why fork: shell call user program using child process.
		wait(0);
		
	}


	exit(0);
}

int readline(char* new_argv[], int ori_argc) {
	char buf[MAXARGLEN];
	int len = 0;
	int	ri = 0;
	memset(buf, 0, MAXARGLEN);
	while (read(0, buf+ri, 1)) {
		if (buf[ri] == '\n') {
			break;
		}
		len++;
		ri++;
	}

	return divline(new_argv, ori_argc, buf, len);
}

int divline(char* new_argv[], int ori_argc, char* buf, int len) {
	int start = 0;
	int singlen = 0;
	int count = 1;
	if (len == 0) {
		return 0;
	}
	for (int i = 0; i < len; i++) {
		if (buf[i] == ' ') {
			new_argv[ori_argc] = malloc(singlen+1);
			memcpy(new_argv[ori_argc++], buf+start, singlen+1);
			count++;
			start = i+1;
			singlen = 0;
			continue;
		}
		singlen++;

	}
	new_argv[ori_argc] = malloc(singlen);
	memcpy(new_argv[ori_argc], buf+start, singlen);
	new_argv[ori_argc][singlen] = 0;
	return count;
}
