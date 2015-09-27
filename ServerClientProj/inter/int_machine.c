#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include "../data/req.h"
#include "../data/funcionario.h"

int main() {
	char package[400];
	int sock_list, sock_recv;
	int addr_in_size, bytes_recv, pid, port;
	hostent *he;
	sockaddr_in inter_addr, client_addr, server_addr;
	funcionario *f;

	pid = fork();

	if((sock_list = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	port = PORTA;
	if(!pid) {
		port = 10500;
	}

	inter_addr.sin_family = AF_INET;
	inter_addr.sin_port = htons(port);
	inter_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(inter_addr.sin_zero), '\0', 8);

	if(bind(sock_list, (sockaddr *)&inter_addr, sizeof(sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	if(listen(sock_list, 10) == -1) {
		perror("listen");
		exit(1);
	}

	printf("9990 -> %d | 9991 -> %d | 9992 -> %d\n", htons(9990), htons(9991), htons(9992));

	while(1) {
		addr_in_size = sizeof(sockaddr_in);
		if((sock_recv = accept(sock_list, (sockaddr *)&client_addr, &addr_in_size)) == -1) {
			perror("accept");
			continue;
		}
		if(!fork()) {
			close(sock_list);
			while((bytes_recv = recv(sock_recv, package, sizeof(package), 0)) == -1) {
				perror("recv");
			}
			package[bytes_recv] = '\0';
			printf("Received connection from: %s\n", inet_ntoa(client_addr.sin_addr));	
			if(!pid) {
				do_echo_client(package, sock_recv, client_addr);
				close(sock_recv);
			} else if(pid > 0) {
				strcat(package, inet_ntoa(client_addr.sin_addr));
				do_echo_command(sock_recv, package, &server_addr, he, client_addr);
			}
			close(sock_recv);
			exit(0);
		}
	}
	if(!pid) {
		while(wait(NULL) > 0);
		exit(0);
	}

	while(wait(NULL) > 0);
	return 0;
}

void do_echo_client(char *pack, int sock, sockaddr_in client) {
	printf("%s -->  from port: %d\n", pack, htons(client.sin_port));
	int sock_cli;
	sockaddr_in cliente;
	hostent *he;
	//	printf("%s\n", get_ip_client(pack));
	if(!preparar_h(&sock_cli, &cliente, he, 12891, get_ip_client(pack))) return;
	while(!conectar(sock_cli, &cliente));
	take_out_ip(pack);
	enviar(sock_cli, pack);
	printf("%s -> NOW\n", pack);
	while(strcmp(pack, "end") != 0) {
		printf("%s --> IWI\n", pack);
		pack = receber(sock, pack, 400);
		take_out_ip(pack);
		enviar(sock_cli, pack);
	}

	close(sock);
	close(sock_cli);
}

void do_echo_command(int sock_cli, char *buf, sockaddr_in *serv, hostent *he, sockaddr_in client) {
	char cmd[4], *f[3];
	int sock[3], opcao = 0, i, port;
	cmd[0] = buf[0];
	cmd[1] = buf[1];
	cmd[2] = buf[2];
	cmd[3] = '\0';
	if(!strcmp(cmd, "add")) {
		if(do_echo(&sock[0], buf, serv, he, 0)) {
			enviar(sock_cli, "Mensagem enviada!\n");
		} else {
			enviar(sock_cli, "Erro ao enviar mensagem!\n");
		}
		close(sock[0]);
		close(sock_cli);
		return;
	}
	if(!strcmp(cmd, "bus")) opcao = 1;
	for(i=0; i<3; i++) {
		if(!fork()) {
			port = 9990 + i;
			do_echo(&sock[i], buf, serv, he, port);
			close(sock[i]);
			exit(0);
		}
	}
	while(wait(NULL) > 0);
	close(sock_cli);
}

void do_echo(int *sock, char *buf, sockaddr_in *serv, hostent *he, int port) {
	if(!port) {
		port = get_porta();
	}
	if(!preparar(sock, serv, he, port)) return 0;
	while(!conectar(*sock, serv));
	if(!enviar(*sock, buf)) return 0;

	return 1;
}

int get_porta() {
	struct timeval tmp;
	gettimeofday(&tmp, NULL);
	int porta = (tmp.tv_usec % 3) + 9990;
	printf("Connecting through port %d\n", porta);
	return porta;
}
