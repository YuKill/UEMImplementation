#ifndef ESPECIE_H
#define ESPECIE_H

typedef struct Especie{
	int id;
	char nome_cient[50];
	char nome_pop[50];
	char desc[500];
} tipo_Especie;

void rewriteArvore(FILE *arquivo);

int inserirEspecie(FILE* arquivo, tipo_Especie especie, int mod);

int buscaIdEspecie(FILE* arquivo, int id);

int removeRegEspecie(FILE* arquivo, int id);

tipo_Especie* recuperaRegEspecie(FILE* arquivo, int id);

void imprimeRegistroEspecie(tipo_Especie* especie);

void modificaArquivoEspecie(FILE* arquivo, FILE* sub, int mod);
#endif

