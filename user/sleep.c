#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
	int ret;
	if (argc < 2) {
		printf("Error: sleep must specify time interval\r\n");
		exit(0);
	}

	ret = sleep(atoi(argv[1]));

	exit(ret);
}