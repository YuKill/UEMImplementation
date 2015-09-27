#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
#include "../../data/funcionario.h"
#include "../../data/req.h"
#include "../header/servers.h"
#include "../header/db.h"
#include "../header/fifo.h"

char fifo_name[] = "s3/fifo/fifo.data";
char db[] = "s3/db/db.data";
sem_t sem_fifo;
sem_t sem_db;

int main(int agrc, char *argv[]) {
	int sock, sock_req, addr_in_size, bytes_recv;
	sockaddr_in req_addr, serv_addr;
	char package[400];
	funcionario *f;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[1]));
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

	sem_init(&sem_fifo, 0, 1);
	sem_init(&sem_db, 0, 1);

	if(bind(sock, (sockaddr *)&serv_addr, sizeof(sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if(listen(sock, BACKLOG_SERV) == -1) {
		perror("listen");
		exit(1);
	}

	inicia_fifo(fifo_name);
	init_db(db);

	if(!fork()) {
		ler_fifo();
		exit(0);
	}

	while(1) {
		addr_in_size = sizeof(sockaddr_in);
		if((sock_req = accept(sock, (sockaddr *)&req_addr, &addr_in_size)) == -1) {
			perror("accept");
			continue;
		}

		if(!fork()) {
			close(sock);
			while((bytes_recv = recv(sock_req, package, sizeof(package), 0)) == -1) {
				perror("recv");
			}
			printf("Signal received from %s\n", inet_ntoa(req_addr.sin_addr));
			package[bytes_recv] = '\0';
			sem_wait(&sem_fifo);
			adiciona(fifo_name, package, req_addr);
			sem_post(&sem_fifo);
			
			close(sock_req);
			exit(0);
		}
		close(sock_req);
	}

	while(wait(NULL) > 0);
	return 0;
}

void ler_fifo() {
	fifo *inst;
	int fda[2], sock;
	char res[30];
	funcionario *f;
	while(1) {
		sem_wait(&sem_fifo);
		inst = remove_fifo(fifo_name);
		sem_post(&sem_fifo);
		if(inst == NULL) {
			usleep(20000);
			continue;
		}
		printf("%s - %s\n", inst->package, get_cmd(inst->package));
		if(!strcmp(get_cmd(inst->package), "add")) {
			sem_wait(&sem_db);
			cadastrar_db(db, *(chartof(inst->package)));
			sem_post(&sem_db);
			printf("Cadastro Adicionado!\n");
			continue;
		}
		printf("COnnect?\n");
		if(!connect_inter(&sock, &(inst->addr))) continue;
		printf("COnectado\n");
		if(!strcmp(get_cmd(inst->package), "rem")) {
			sem_wait(&sem_db);
			if(!remover_db(db, *(chartof(inst->package)))) {
				strcpy(res, "end|");
				strcat(res, inet_ntoa(inst->addr.sin_addr));
				enviar(sock, res);
			} else {
				printf("Cadastro Removido!\n");
				enviar(sock, inst->package);
			}
			sem_post(&sem_db);
			close(sock);
			continue;
		}
		while(pipe(fda) == -1) {
			perror("pipe");
		}
		f = chartof(inst->package);
		if(!fork()) {
			close(1);
			dup(fda[1]);
			close(fda[1]);
			close(fda[0]);
			sem_wait(&sem_db);
			consulta_db(db, f->nome, f->sobrenome, f->idade);
			sem_post(&sem_db);
			exit(0);
		}
		if(!fork()) {
			char resp[400];
			close(0);
			dup(fda[0]);
			close(fda[1]);
			close(fda[0]);
			while(scanf("%s", resp) > 0) {
				strcat(resp, inet_ntoa(inst->addr.sin_addr));
				memset(&resp[strlen(resp)], '\0', 400-strlen(resp));
				printf("%s -> sending...\n", resp);
				enviar(sock, resp);
			}
			exit(0);
		}
		close(fda[0]);
		close(fda[1]);
		printf("Consulta Feita!");
		close(sock);
		while(wait(NULL) > 0);
	}
	while(wait(NULL) > 0);
}

int connect_inter(int *sock, sockaddr_in *inter) {
	hostent *he;
	if(!preparar(sock, inter, he, 10500)) return 0;
	if(!conectar(*sock, inter)) return 0;

	return 1;
}
