#include "kernel/types.h"
#include "user/user.h"

// using while problem: running on last process read, especially in ecall of Sysread.
/*
void prime_processes(int* lp) {
	int  prime;
	int rp[2];
	int pid;
	int num;

	while (1) {
		int readv = read(lp[0], &prime, sizeof(prime));
		fprintf(1, "readv: %d", readv);
		if (readv == 0) {
			close(lp[0]);
			close(lp[1]);
			break;
		} else if (readv < 0) {
			fprintf(2, "read failed when previous prime: %d.\n", prime);
			exit(1);
		} else {
		}
		fprintf(1, "prime %d\n", prime);
		if (pipe(rp) < 0 ) {
			fprintf(2, "Creating right pipe failed.\n");
			exit(1);
		}
		pid = fork();
		if (pid == 0) {
			close(lp[0]);
			close(lp[1]);
			// update child process left pipe
			lp[0] = rp[0];
			lp[1] = rp[1];
			continue;
		}
		close(lp[1]);
		close(rp[0]);
		while (read(lp[0], &num, sizeof(num)) > 0) {
			if (num % prime != 0) {
				write(rp[1], &num, sizeof(num));
			}
		}
		close(lp[0]);
		close(rp[1]);
		wait((int*) 0);
		break;	
	
	}
	exit(0);
} 
*/

// using recursion
// puzzling problem: way of while and recursion isn't the reason of making process running and end $ character disappearing although the output is correct.
// solution above: write code "close(lp[1]) at early not in separate process in range "if (pid == o) ..." 
void prime_processes(int* lp) {
	int prime;
	int pid;
	int num;
	int rp[2];
	
	close(lp[1]);
	if (read(lp[0], &prime, sizeof(prime)) != sizeof(prime)) {
		// close(lp[1]);
		close(lp[0]);	
		exit(0);
	}
	fprintf(1, "prime %d\n", prime);

	if (pipe(rp) < 0) {
		fprintf(2, "process %d: Creating right pipe failed.\n");
	}	
	pid = fork();
	if (pid == 0) {
		close(lp[0]);
		//close(lp[1]);
		prime_processes(rp);
	} else  {
		close(rp[0]);
		//close(lp[1]);
		while (read(lp[0], &num, sizeof(num)) > 0) {
			if (num % prime != 0) {
				write(rp[1], &num, sizeof(num));
			}
		}
		close(rp[1]);
		close(lp[0]);
		wait((int*) 0);
		exit(0);
	}
}
int main(int argc, char *argv[]) {
	int i;
	int pid;
	int fp[2];
	
	if (argc != 1) {
		fprintf(2, "Usage: primes\n");
		exit(1);
	}
	
	pipe(fp);
	pid = fork();
	
	if (pid == 0) {
		prime_processes(fp);
	}
	
	close(fp[0]);
	for (i = 2; i < 36; i++) {
		write(fp[1], &i, sizeof(i));
	}
	close(fp[1]);
	wait((int*) 0);

	exit(0);
	return 0;
}
