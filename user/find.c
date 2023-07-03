#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*refer user/ls.c notice:fmtname in ls.c return basename with lots of tail blank*/
void find(char *path, char *target);
char* basename(char *path);


int main(int argc, char *argv[])
{
	char *path;
	char *target;

	if (argc < 3) {
		printf("wrong args number\r\n");
		exit(1);
	}

	path = argv[1];
	target = argv[2];

	find(path, target);

	exit(0);
}


void find(char *path, char *target)
{
	int fd;
	struct stat st;
	struct dirent de;
	char buf[100];
	char *p;

	if ((fd = open(path, 0)) < 0) {
		fprintf(2, "find: cannot open %s\n", path);
		return;
	}

	if (fstat(fd, &st) < 0) {
		fprintf(2, "find: cannot stat %s\n", path);
		close(fd);
		return;
	}


	if (st.type == T_DEVICE || st.type == T_FILE) {
		if (strcmp(basename(path), target) == 0) {
			printf("%s\r\n", path);
		}
		close(fd);
		return;

	} else if (st.type == T_DIR) {
		strcpy(buf, path);
		p = buf + strlen(path);
		*p++ = '/';
		/* traverse each dirent,no matter file-dentry or directory-dentry */
		while (read(fd, &de, sizeof(de)) == sizeof(de)) {
			if (strcmp(basename(de.name), ".") == 0 || \
				(strcmp(basename(de.name), "..") == 0)) {
					continue;
				}

			// invalid file or directory
			if (de.inum == 0)
				continue;

			memmove(p, de.name, DIRSIZ);
			p[DIRSIZ] = 0;
			find(buf, target);

			// recover buf (has been changed in deeper recursion)
			memset(buf, 0, sizeof(buf));
			strcpy(buf, path);
			p = buf + strlen(path);
			*p++ = '/';
		}
		close(fd);
	} else {
		return;
	}
}

char* basename(char *path)
{
	char *p;

	// Find first character after last slash.
	for(p=path+strlen(path); p >= path && *p != '/'; p--)
	;
	p++;

	return p;
}