#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int rece2send(int *piper, int *pipew, int size);

int main(int argc, char *argv[])
{
	int pid;
	int pipe1[2];
	int pipe2[2];
	char ball[] = {'P','i','n','g','P','o','n','g'};

	pipe(pipe1);
	pipe(pipe2);

	write(pipe2[1], ball, sizeof(ball));

	pid = fork();
	if (pid < 0) {
		printf("fork failed\r\n");
		exit(1);
	}

	if (pid == 0)
		rece2send(pipe2, pipe1, sizeof(ball));
	else
		rece2send(pipe1, pipe2, sizeof(ball));

	exit(0);
}

int rece2send(int *piper, int *pipew, int size)
{
	int cnt = 0;
	char buf[size+1];
	close(piper[1]);
	close(pipew[0]);

	while (cnt < 5) {
		read(piper[0], buf, size);
		buf[size] = 0;
		printf("PID:%d get: %s\r\n", getpid(), buf);
		printf("PID:%d put: %s\r\n \r\n", getpid(), buf);
		if (write(pipew[1], buf, size) != size) {
			printf("write pipe failed\r\n");
			exit(1);
		}

		sleep(1);
		cnt++;
	}

	close(piper[0]);
	close(pipew[1]);

	return 0;
}