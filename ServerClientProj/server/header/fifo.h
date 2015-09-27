#ifndef FIFO_H
#define FIFO_H

#include <sys/socket.h>
#include "../../data/req.h"

typedef struct {
	char package[400];
	sockaddr_in addr;
	short prox;
} fifo;

void inicia_fifo(char arq[]);
void adiciona(char arq[], char entra[], sockaddr_in cliente);
fifo *remove_fifo(char arq[]);
#endif
