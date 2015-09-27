#ifndef CAPTURADO_H
#define CAPTURADO_H

typedef struct Capturado{
	int id_ind;
	int comprimento;
	int largura;
	int peso;
	char data_captura[11];
	char local_captura[101];
} tipo_Capturado;

int inserirCapturado(FILE* arquivo, tipo_Capturado capturado);

int buscaIdCapturado(FILE* arquivo, int id_ind, char data_captura[], int off, int offset_inicial);

tipo_Capturado* recuperaRegCapturado(FILE* arquivo, int id_ind, char data[], int off, int offset_inicial);

int removeRegCapturado(FILE* arquivo, int id_ind, char data[], int off, int offset_inicial);

void imprimeRegistroCapturado(tipo_Capturado* capturado);

tipo_Capturado* retornaUltimaCaptura(FILE* arquivo, int id_ind);

void recuperaListaCapturas(FILE* arquivo, int id_ind);

void recuperaListaCapturasEspecie(FILE* file_cap, FILE* file_ind, int id);

void modificaArquivoCapturado(FILE* arquivo, FILE* sub, int mod);
#endif
