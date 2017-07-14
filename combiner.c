#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

void main(void)
{
	int fd[2];
	pipe(fd);
	pid_t pid = fork();
	if(pid == 0)
	{
		#define BUF_SIZE2 20
		char *temp_1 = (char *)malloc(sizeof(int)), *temp_2 = (char *)malloc(sizeof(int));
		close(fd[0]);	
		sprintf(temp_1, "%d", fd[0]);
		sprintf(temp_2, "%d", fd[1]);
		execlp("./map", "map", temp_1, temp_2, NULL);
	}
	else
	{
		#define BUF_SIZE 20
		close(fd[1]);
		wait(pid);
		pid_t pid2 = fork();
		if(pid2 == 0)
		{
			char *temp_3 = (char *)malloc(sizeof(int)), *temp_4 = (char *)malloc(sizeof(int));
			close(fd[1]);
			sprintf(temp_3, "%d", fd[0]);
			sprintf(temp_4, "%d", fd[1]);
			execlp("./reduce", "reduce", temp_3, temp_4, NULL);
		}
		else
		{
			wait(pid2);
			printf("Parent closing.\n");
		}

	}
}
