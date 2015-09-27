#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include "../header/fifo.h"
#include <stdio.h>
#include <string.h>


void inicia_fifo(char arq[]) {
	FILE *arq_ini;
	short a;
	
	a=0;
	arq_ini = fopen(arq, "w+b");
	fseek(arq_ini, 0, SEEK_SET);
	fwrite(&a, sizeof(short), 1, arq_ini);
	fwrite(&a, sizeof(short), 1, arq_ini);
	fwrite(&a, sizeof(short), 1, arq_ini);
	fclose(arq_ini);
}

void adiciona(char arq[], char entra[], sockaddr_in cliente) {
	FILE *arq_ent;
	fifo aux;
	short pos, pf, uf;
	
	arq_ent = fopen(arq, "r+b");
	strcpy(aux.package, entra);
	aux.addr = cliente;
	aux.prox = 0;
	fseek(arq_ent, 0, SEEK_SET);
	fread(&pf, sizeof(short), 1, arq_ent);
	fread(&uf, sizeof(short), 1, arq_ent);
	fread(&pos, sizeof(short), 1, arq_ent);
	if ((pf == 0) && (uf == 0)) {
		add_inicio(aux, arq_ent, pos);		
	}else	if (pos == 0) {
		add_final(aux, arq_ent);	
	}else {
		add_meio(aux, pos, arq_ent);
	}
	fclose(arq_ent);
	
}

short calc_bs(short pos) {
	return(pos-1)*sizeof(fifo)+6;
}

short calc_pos(short x) {
	return((x+sizeof(fifo)-6)/sizeof(fifo));
}

fifo *remove_fifo(char arq[]) {
	FILE *arq_ent;
	short prim, np, nr, ar, par, ult;
	fifo *out = (fifo *) malloc(sizeof(fifo)), ler, ler1;

	arq_ent = fopen(arq, "r+b");
	fseek(arq_ent, 0, SEEK_SET);
	fread(&prim, sizeof(short), 1, arq_ent);
	fread(&ult, sizeof(short), 1 , arq_ent);
	if (prim != 0) {
		if (ult == prim) {
			fseek(arq_ent, 2, SEEK_SET);
			ar = 0;
			fwrite(&ar, sizeof(short), 1, arq_ent);
		}
		ar = prim;
		prim = calc_bs(prim);
		fseek(arq_ent, prim, SEEK_SET);
		fread(&ler, sizeof(fifo), 1, arq_ent);
		np = ler.prox;
		fseek(arq_ent, 0, SEEK_SET);
		fwrite(&np, sizeof(short), 1, arq_ent);
		*out = ler;
		fseek(arq_ent, 4, SEEK_SET);
		fread(&par, sizeof(short), 1, arq_ent);
		fseek(arq_ent, 4, SEEK_SET);
		fwrite(&ar, sizeof(short), 1, arq_ent);
		ar = calc_bs(ar);
		fseek(arq_ent, ar, SEEK_SET);
		ler.prox = par;
		fwrite(&ler, sizeof(fifo), 1, arq_ent);
		fclose(arq_ent);
		return(out);
	}else {
		fclose(arq_ent);
		return(NULL);
	}
}

void add_inicio(fifo aux, FILE *arq_ent, short pos) {
	fifo ler, nex;
	short pf, uf, ur, aux2;
	if (pos != 0) {
			
		aux2 = pos;
		aux2 = calc_bs(pos);
		fseek(arq_ent, aux2, SEEK_SET);
		fread(&nex, sizeof(fifo), 1, arq_ent);
		ur = nex.prox;
		fseek(arq_ent, 4, SEEK_SET);
		fwrite(&ur, sizeof(short), 1, arq_ent);
		fseek(arq_ent, aux2, SEEK_SET);
		fwrite(&aux, sizeof(fifo), 1, arq_ent);
		fseek(arq_ent, 0, SEEK_SET);
		fwrite(&pos, sizeof(short), 1, arq_ent);
		fwrite(&pos, sizeof(short), 1, arq_ent);
		
	}else {
		fseek(arq_ent, 6, SEEK_SET);	
		fwrite(&aux, sizeof(fifo), 1, arq_ent);
		aux2 = ftell(arq_ent) - sizeof(fifo);
		aux2 = calc_pos(aux2);
		fseek(arq_ent, 0, SEEK_SET);
		fwrite(&aux2, sizeof(short), 1, arq_ent);
		fseek(arq_ent, 2, SEEK_SET);
		fwrite(&aux2, sizeof(short), 1, arq_ent);	
	}
	
	
}


void add_final(fifo aux, FILE *arq_ent) {
	short x, aux2;	
	fifo f;
	
	fseek(arq_ent, 0, SEEK_END);
	fwrite(&aux, sizeof(fifo), 1, arq_ent);
	x = ftell(arq_ent) - sizeof(fifo);
	x = calc_pos(x);
	fseek(arq_ent, 2, SEEK_SET);
	fread(&aux2, sizeof(short), 1, arq_ent);
	fseek(arq_ent, 2, SEEK_SET);
	fwrite(&x, sizeof(short), 1, arq_ent);
	aux2 = calc_bs(aux2);
	fseek(arq_ent, aux2, SEEK_SET);
	fread(&f, sizeof(fifo), 1, arq_ent);
	f.prox = x;
	fseek(arq_ent, aux2, SEEK_SET);
	fwrite(&f, sizeof(fifo), 1, arq_ent);
}

void add_meio(fifo aux, short pos, FILE *arq_ent) {
	short x, y, u;
	fifo ler;
	
	x = calc_bs(pos);
	fseek(arq_ent, x, SEEK_SET);
	fread(&ler, sizeof(fifo), 1, arq_ent);
	u = ler.prox;
	fseek(arq_ent, x, SEEK_SET);
	fwrite(&aux, sizeof(fifo), 1, arq_ent);
	fseek(arq_ent, 4, SEEK_SET);	//att ultimo removido
	fwrite(&u, sizeof(short), 1, arq_ent);
	fseek(arq_ent, 2, SEEK_SET);	//att ultimo add
	fread(&y, sizeof(short), 1, arq_ent);
	y = calc_bs(y);
	fseek(arq_ent, 2, SEEK_SET);
	fwrite(&pos, sizeof(short), 1, arq_ent);
	fseek(arq_ent, y, SEEK_SET);
	fread(&ler, sizeof(fifo), 1, arq_ent);
	ler.prox = pos;
	fseek(arq_ent, y, SEEK_SET);
	fwrite(&ler, sizeof(fifo), 1, arq_ent);
}


/**
void printa() {
	FILE *arq;
	short aux, aux2;
	fifo sai;
	char x;

	arq = fopen("teste.txt", "r+b");
	fread(&aux, sizeof(short), 1, arq);
	printf("\n%d\n", aux);
	while (aux != 0) {
		aux2 = aux;
		aux = calc_bs(aux);
		fseek(arq, aux, SEEK_SET);
		fread(&sai, sizeof(fifo), 1, arq);
		printf("%d # %s # %d\n", aux2, sai.package, sai.prox);
		aux = sai.prox;
		scanf("%c", &x);
	}
	fclose(arq);
}

int main(){
	int cont;
	char a[40];

	inicia_fifo("teste.txt");
	cont = 0;
	while (cont < 3) {
		sprintf(a, "kkkkkkkkkkk-%d", cont);
		adiciona("teste.txt", a, cont);
		cont++;
	}
	printa();
	remove_fifo("teste.txt");
	remove_fifo("teste.txt");
	remove_fifo("teste.txt");

		adiciona("teste.txt", "xxxxxxxxx-77", 1);
		adiciona("teste.txt", "xxxxxxxxx-79", 1);
		adiciona("teste.txt", "xxxxxxxxx-78", 1);
	printa();
}**/
