#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
	printf("%s\n", argv[0]);
	execlp("ls", "ls",NULL);
}
