#ifndef INDIVIDUO_H
#define INDIVIDUO_H

typedef struct Individuo{
	int id;
	int id_especie;
	char sexo;
} tipo_Individuo;

int inserirIndividuo(FILE* arquivo, tipo_Individuo individuo, int mod);

int buscaIdIndividuo(FILE* arquivo, int id);

tipo_Individuo* recuperaRegIndividuo(FILE* arquivo, int id);

int removeRegIndividuo(FILE* arquivo, int id);

void imprimeRegistroIndividuo(tipo_Individuo* ind);

void modificaArquivoIndividuo(FILE* arquivo, FILE* sub, int mod);
#endif
