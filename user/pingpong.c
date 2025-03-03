#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
	int p[2];
	int pid;
	char bytemsg;

	if (argc != 1) {
		fprintf(2, "Usage: pingpong...");
		exit(1);
	}

	if (pipe(p) < 0) {
		fprintf(2, "Create pipe failed.\n");
		exit(1);
	}

	pid = fork();
	if (pid == 0) {
		if (read(p[0], &bytemsg, 1) < 0) {
			fprintf(2, "read bytemsg from pipe failed.\n");
			exit(1);
		}
		fprintf(1, "%d: received ping\n", getpid());
		if (write(p[1], &bytemsg, 1) < 0) {
			fprintf(2, "Child writing to pipe failed.\n");
			exit(1);
		}	
		close(p[0]);
		close(p[1]);
		exit(0);
	}

	if (write(p[1], (char *)0, 1) < 0) {
		fprintf(2, "Parent writing to pipe failed.\n");
		exit(1);
	}

	if (read(p[0], &bytemsg, 1) < 0) {
		fprintf(2, "Parent writing to pipe failed.\n");
		exit(1);
	}
	if (wait((int *)0) < 0) {
		fprintf(2, "Waiting child process failed.\n");
		exit(1);
	}

	close(p[0]);
	close(p[1]);

	fprintf(1, "%d: received pong\n", getpid());
	exit(0);	
}
