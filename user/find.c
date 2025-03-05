#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char* fmtname(char* path);
void find(char* path, char* des_filename);
int main(int argc, char* argv[]) {

	char* path;
	int i;
	if (argc < 3) {
		fprintf(2, "Usage: find path [file...]\n");
		exit(1);
	}
	path = argv[1];
	for(i = 2; i < argc; i++) {
		find(path, argv[i]);
	}

	// solve problem below
	exit(0);

	// use return to report error: cannot open stat N. Very strange!
	// return
}
void find(char* path, char* des_filename) {

	int fd;
	struct stat st;
	char buf[512], *p;
	memset(buf, 0, 512);
	struct dirent de;

	// check correctness of path
	if ((fd = open(path, 0)) < 0) {
		fprintf(1, "find: cannot open %s\n", path);
		return;
	}	

	// check correctness of path inode metadata 
	if (fstat(fd, &st) < 0) {
		fprintf(1, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}
	switch(st.type){
		case T_FILE:
			if (strcmp(fmtname(path), des_filename) == 0) {
				fprintf(1, "%s\n", path);
			}				
			break;
		case T_DIR:
			// strlen(path) + 1('/') + max fixedsize of entry + 1('\0')
			if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
				fprintf(1, "find: path too long\n");
				break;
			}
			strcpy(buf, path);
			p = buf + strlen(path);
			*p++ = '/';

			while (read(fd, &de, sizeof(de)) == sizeof(de)) {
				// inode number of directory entry should be >= 1 in generally  
				if (de.inum == 0)
					continue;
				if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
					continue;
				}
				// append name of direcotry entry to parent directory
				memcpy(p, de.name, DIRSIZ);

				// necessary use buf for multiple entries of directory
				p[DIRSIZ] = 0;
			
				find(buf, des_filename);
			}	
		break;
	}
	// forgetting close fd cost lot of my time
	close(fd);

}

char* fmtname(char* path) {
	static char buf[DIRSIZ+1];
	char* p;

	// p points the last character of path
	p = path + strlen(path);

	// make p points the last '/'
	while (*p!='/')
		p--;

	// make p points the first character after the last slash.
	p++;

	if (strlen(p) > DIRSIZ)
		return p;
	// get file name not including directory
	strcpy(buf, p);
	// blank-padded name
	buf[strlen(buf)]= '\0';
	return buf;

}
