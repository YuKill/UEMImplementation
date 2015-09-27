#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include "funcionario.h"
#include "req.h"

int preparar_h(int *sockfd, sockaddr_in *their_addr, hostent *he, int port, char *hostname) {
	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 0;
	}

	int yes = 1;
	if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}  

	if((he = gethostbyname(hostname)) == -1) {
		perror("gethostbyname");
		return 0;
	}

	their_addr->sin_family = AF_INET;
	their_addr->sin_addr = *((in_addr *)he->h_addr);
	their_addr->sin_port = htons(port);
	memset(&(their_addr->sin_zero), '\0', 8);

	return 1;
}

int preparar(int *sockfd, sockaddr_in *their_addr, hostent *he, int port) {
	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 0;
	}

	int yes = 1;
	if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}  

	if((he = gethostbyname("localhost")) == -1) {
		perror("gethostbyname");
		return 0;
	}

	their_addr->sin_family = AF_INET;
	their_addr->sin_addr = *((in_addr *)he->h_addr);
	their_addr->sin_port = htons(port);
	memset(&(their_addr->sin_zero), '\0', 8);

	return 1;
}

int conectar(int sockfd, sockaddr_in *their_addr) {
	if(connect(sockfd, (sockaddr *)their_addr, sizeof(sockaddr)) == -1) {
		perror("connect");
		return 0;
	}
	return 1;
}

int enviar(int sockfd, char *package) {
	if(send(sockfd, package, strlen(package), 0) == -1) {
		perror("send");
		return 0;
	}
	return 1;
}

int receber(int sock, char *buf, int size) {
	int num;
	if((num = recv(sock, buf, size, 0)) == -1) {
		perror("recv");
		return 0;
	}
	buf[num] = '\0';
	return 1;
}
