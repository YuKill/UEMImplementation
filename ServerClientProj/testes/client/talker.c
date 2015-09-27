#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct in_addr in_addr;
typedef struct hostent hostent;
typedef struct sockaddr sockaddr;
int main(int argc, char *argv[]) {
	hostent *he;
	sockaddr_in destaddr;
	char buff[100];
	int sockfd, numbytes, sockfd1, sockfd2;

	if((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}


	prep(&sockfd, &destaddr, he);	
	//	prep(&sockfd1, &destaddr, he);	
	//	prep(&sockfd2, &destaddr, he);	

	int i = 0;
	for(i=0; i<3; i++) {
		if(!fork()) {
			sprintf(buff, "HelloWorld from process %d\n", getpid());
			if(send(sockfd, buff, strlen(buff), 0) == -1) {
				perror("send");
				exit(1);
			}
			printf("Sent from %d\n", getpid());
			exit(0);
		}
	}

	while(wait(NULL) >0);

	close(sockfd);
	return 0;
}

void prep(int *sock, sockaddr_in *destaddr, hostent *he) {

	if((*sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	destaddr->sin_family = AF_INET;
	destaddr->sin_port = htons(9090);
	destaddr->sin_addr = *((in_addr *)he->h_addr);
	memset(&(destaddr->sin_zero), '\0', 8);

	if(connect(*sock, (sockaddr *)destaddr, sizeof(sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}
