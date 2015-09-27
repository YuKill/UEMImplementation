#include <stdlib.h>
#include <sys/types.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
	key_t key;

	key = ftok("pr.c", 'G');
	int shmid, *x;

	shmid = shmget(key, atoi(argv[1]), 0644|IPC_CREAT);
	if((x = shmat(shmid, (void *)0, 0)) == -1) { 
		printf("fuck\n");
		exit(1);
	}
	*x = 8;
	if(!fork()) {
		*x = 10;
		exit(0);
	}
	printf("%d\n", *x);
	while(wait(NULL) > 0);
	shmdt(x);
	
}
