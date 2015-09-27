#include <stdlib.h>
#include <stdio.h>
#include "../header/db.h"
#include "../../data/funcionario.h"
#include <string.h>

void init_db(char *nome) {
	FILE *f = fopen(nome, "r+b");
	short m = 0;
	if(!f) {
		f = fopen(nome, "w+b");
	}
	if(!feof(f)) {
		fwrite(&m, sizeof(short), 1, f);
	}
	fclose(f);
}

int cadastrar_db(char *nome, funcionario func) {
	short garb;
	FILE *f = fopen(nome, "r+b");
	fseek(f, 0, SEEK_SET);
	fread(&garb, sizeof(short), 1, f);

	if(garb == 0) {
		add_no_garb(f, func);
		fclose(f);
		return 1;
	}	
	add_garb(f, func, garb);
	fclose(f);
	return 1;
}

void add_no_garb(FILE *f, funcionario func) {
	fseek(f, 0, SEEK_END);
	fwrite(&func, sizeof(funcionario), 1, f);
}

void add_garb(FILE *f, funcionario func, int garb) {
	funcionario ler;
	int boff = 2+(garb-1)*sizeof(funcionario);
	//add over the garbage
	fseek(f, boff, SEEK_SET);
	fread(&ler, sizeof(funcionario), 1, f);
	fseek(f, boff, SEEK_SET);
	fwrite(&func, sizeof(funcionario), 1, f);
	//att garbage
	fseek(f, 0, SEEK_SET);
	fwrite(&ler.idade, sizeof(short), 1, f);
}

void consulta_db(char *nome, char *at, char *value, int idade) {
	funcionario ler;
	FILE *f = fopen(nome, "r+b");
	fseek(f, 2, SEEK_SET);
	while(fread(&ler, sizeof(funcionario), 1, f) > 0) {
		if(!strcmp(ler.nome, "$")) continue;
		if(!strcmp(at, "nome")) if(print_filter(ler, ler.nome, value, 0, 0)) continue;
		if(!strcmp(at, "sobrenome")) if(print_filter(ler, ler.sobrenome, value, 0, 0)) continue;
		if(!strcmp(at, "cidade")) if(print_filter(ler, ler.cidade, value, 0, 0)) continue;
		if(!strcmp(at, "estado")) if(print_filter(ler, ler.estado, value, 0, 0)) continue;
		if(!strcmp(at, "idade")) if(print_filter(ler, NULL, NULL, idade, ler.idade)) continue;
	}
	printf("end|");
}

int print_filter(funcionario func, char *value, char *value2, int idade, int idade2) {
	if(!idade) { //se at != idade
		if(!strcasecmp(value, value2)) printf("%s\n", ftochar(func));
	} else {
		if(idade == idade2) printf("%s\n", ftochar(func));
	}
	return 1;
}

int remover_db(char *nome, funcionario func) {
	long int head;
	int num;
	funcionario ler;
	FILE *f = fopen(nome, "r+b");
	fseek(f, 2, SEEK_SET);
	while((num = fread(&ler, sizeof(funcionario), 1, f)) > 0) {
		if(!strcmp(ler.nome, "$")) continue;
		if(strcasecmp(ftochar(func), ftochar(ler))) continue;
	
		head = ftell(f);
		head = head - sizeof(funcionario);
		printf("\n%ld\n", head);
		strcpy(ler.nome, "$");
		fseek(f, head, SEEK_SET);
		fwrite(&ler, sizeof(funcionario), 1, f);
		fclose(f);
		return 1;
	}
	fclose(f);
	return 0;
}

