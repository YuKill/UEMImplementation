#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/shm.h>
#include <semaphore.h>
#include "funcionario.h"
#include "req.h"


int send_funcionario(char cmd[], funcionario cad) {
	int sock, ends = 1;
	sockaddr_in inter;
	hostent *he;
	char package[400];


	strcpy(package, cmd);
	strcat(package, ftochar(cad));

	if(!preparar(&sock, &inter, he, PORTA)) return 0;
	inter.sin_port = htons(PORTA);
	if(!conectar(sock, &inter)) return 0;
	if(!enviar(sock, package)) return 0;

	if(!strcmp(cmd, "add")) {
		do_add(sock);
		ends = 5;
	}

	close(sock);

	int sock_list, sock_recv;
	int addr_in_size;
	sockaddr_in inter_addr, client_addr;

	if(!preparar(&sock, &inter_addr, he, 12891)) return 0;
	if(bind(sock, (sockaddr *)&inter_addr, sizeof(sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}   
	if(listen(sock, 50) == -1) {
		perror("listen");
		exit(1);
	}  
	while(ends < 4) {
		addr_in_size = sizeof(sockaddr_in);
		while((sock_recv = accept(sock, (sockaddr *)&client_addr, &addr_in_size)) == -1) {
			perror("accept");
		}
		ends++;
		if(!fork()) {
			close(sock);
			printf("Received connection from: %s\n", inet_ntoa(client_addr.sin_addr));

				char f[400];
				funcionario *func;
				receber(sock_recv, f, 400);
				if(strcmp(get_cmd(f), "end")) {
					if(!strcmp(cmd, "rem")) {
						printf("Removido com Sucesso!\n");
						break;
					} else {
						func = chartof(f);
						print_func(*func);
						receber(sock_recv, f, 400);
					}
				}
//				receber_enquanto(sock_recv, cmd);
			close(sock_recv);
			exit(0);
		}

		close(sock_recv);
	}
	close(sock);
	while(wait(NULL) > 0);
	return 1;
}

void do_add(int sock) {
	char *msg = (char *) malloc(100);
	receber(sock, msg, 100);
}
/*
void receber_enquanto(int sock, char cmd[]) {
	char f[400];
	funcionario *func;
	receber(sock, f, 400);
	printf("%s\n", f);
	while(strcmp(f, "end")) {
		if(!strcmp(cmd, "rem")) {
			printf("Removido com Sucesso!\n");
			return;
		} else {
			func = chartof(f);
			print_func(*func);
		}
	}
	sem_wait(&sem);
	*ends++;
	sem_post(&sem);
	printf("%d -> end\n", *ends);
}
*/
char *ftochar(funcionario func) {
	char *buf = (char *) malloc(400);
	sprintf(buf, "|%s|%s|%s|%s|%d|", func.nome, func.sobrenome, func.cidade, func.estado, func.idade);
	return buf;
}

void print_func(funcionario f) {
	printf("Funcionario: %s %s\n", f.nome, f.sobrenome);
	printf("De: %s-%s\n", f.cidade, f.estado);
	printf("%d anos\n\n", f.idade);
}

char* get_ip_client(char *pack) {
	char *ip = (char *) malloc(20);
	int i, flag = 0, j = 0;
	for(i=strlen(pack)-1; i>=0; i--) {
		if(pack[i] == '|') break;
	}
	for(i=i+1; i<strlen(pack); i++) {
		ip[j] = pack[i];
		ip[j+1] = '\0';
		j++;
	}
	return ip;
}

char *get_cmd(char *pack) {
	char *cmd = (char *) malloc(4);
	strncpy(cmd, pack, 3); 
	cmd[3] = '\0';
	return cmd;
}

void take_out_ip(char *pack) {
	char *ip = (char *) malloc(20);
	int i, flag = 0, j = 0;
	for(i=strlen(pack)-1; i>=0; i--) {
		if(pack[i] == '|') break;
	}
	pack[i] = '\0';
	return;
}

funcionario *chartof(char *buf) {
	int i = 0, flag = 0, j = 0;
	char idade[10];
	funcionario *func = (funcionario *) malloc(sizeof(funcionario));
	for(i=0; i<strlen(buf); i++) {
		if(buf[i] == '|') {
			j = 0;
			flag++;
		} else {
			switch(flag) {
				case 0:
					continue;
				case 1:
					func->nome[j] = buf[i];
					func->nome[j+1] = '\0';
					j++;
					break;
				case 2:
					func->sobrenome[j] = buf[i];
					func->sobrenome[j+1] = '\0';
					j++;
					break;
				case 3:
					func->cidade[j] = buf[i];
					func->cidade[j+1] = '\0';
					j++;
					break;
				case 4:
					func->estado[j] = buf[i];
					func->estado[j+1] = '\0';
					j++;
					break;
				case 5:
					idade[j] = buf[i];
					idade[j+1] = '\0';
					j++;
					break;
				default:
					i = 2000;
			}
		}
	}

	func->idade = atoi(idade);
	return func;
}

