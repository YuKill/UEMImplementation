#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

sem_t sem;

int main() {
	char a[40], b[40];
	scanf("%s", a);
	printf("%s\n", a);

}
