#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/especie.h"
#include "headers/arvoreB.h"

int inserirEspecie(FILE* arquivo, tipo_Especie especie, int mod){
	short tamanho, size = 1;
	char buffer[610];
	tipo_Key key;
	int state = 0, offset = 0, id_def = 0, maior = 0;

	fseek(arquivo, 0, SEEK_SET);
	while (fread(&size, sizeof(short), 1, arquivo)){
		offset = offset + size + sizeof(size);
		fseek(arquivo, offset, SEEK_SET);
	}
	key.id = especie.id;
	key.offset = offset;
	if(mod) {
		state = insertKey(key);
	}

	if(state != -1){

		sprintf(buffer, "%d|%s|%s|%s|", especie.id, especie.nome_cient, especie.nome_pop, especie.desc);
		tamanho = strlen(buffer);	
		fwrite(&tamanho, sizeof(tamanho), 1, arquivo);
		fputs(buffer, arquivo);
	}
}

void rewriteArvore(FILE *arquivo) {
	int offset = 0, i, id_;
	short size;
	char id[20];
	tipo_Key key;
	fseek(arquivo, 0, SEEK_SET);
	while (fread(&size, sizeof(short), 1, arquivo)){
	
		for(i=0; i<size; i++){
			if((id[i] = fgetc(arquivo)) == '|') {
				break;
			}
		}
		id[i] = '\0';
		id_ = atoi(id);
		key.id = id_;
		key.offset = offset;
		insertKey(key);
		offset = offset + size + sizeof(size);
		fseek(arquivo, offset, SEEK_SET);
	}
	
}

int buscaIdEspecie(FILE* arquivo, int id){
	char leitura[10];
	short size;
	int id_busca;
	int offset = 0;

	if((offset = searchKey(id)) != -1){
		return offset;
	}
	return -1;
/*
	while(fread(&size, sizeof(size), 1, arquivo)) {
		int i;
		for (i=0;i<size;i++){
			if (((leitura[i] = fgetc(arquivo)) == '|') || (leitura[i] == '$' )) {
				break;
			}
		}
		leitura[i] = '\0';
		if(leitura[0] == '$'){
			id_busca = 0;
		} else {
			id_busca = atoi(leitura);
		}
		if (id_busca == id){
			return offset;
		} else {
			offset = offset + size + sizeof(size);
			fseek(arquivo, offset, SEEK_SET);
		}
		
	}

	return -1;
*/
}

int removeRegEspecie(FILE* arquivo, int id){
	int offset = buscaIdEspecie(arquivo, id);
	if(offset != -1) {
		fseek(arquivo, offset, SEEK_SET);
		char c[2];
		c[0] = '$';
		c[1] = " ";
		fputs(c, arquivo);
		removeKey(id);
		return 1;
	} else {
		return 0;
	}
}

tipo_Especie* recuperaCamposEspecie(char reg[]){
	int i, j = 0, flag = 1;
	char id[11],nome_cient[51], nome_pop[51], desc[501];
	tipo_Especie *esp = malloc(sizeof(tipo_Especie)+1);
	
	for(i = 0; i < strlen(reg); i++){
		if(reg[i] == '|'){
			if(flag == 1) {
				id[j] = '\0';
			} else if(flag == 2){
				nome_cient[j] = '\0';
			} else if(flag == 3){
				nome_pop[j] = '\0';
			} else if(flag == 4){
				break;
			}
			flag++;
			j = 0;
		} else if(flag == 1){
			id[j] = reg[i];
			j++;
		} else if(flag == 2){
			nome_cient[j] = reg[i];
			j++;
		} else if(flag == 3){
			nome_pop[j] = reg[i];
			j++;
		} else if(flag == 4){
			desc[j] = reg[i];
			j++;
		}
	}
	desc[j] = '\0';
	esp->id = atoi(id);
	strcpy(esp->nome_cient, nome_cient);
	strcpy(esp->nome_pop, nome_pop);
	strcpy(esp->desc, desc);
	
	return esp;	
}

tipo_Especie* recuperaRegEspecie(FILE* arquivo, int id){
	int offset = buscaIdEspecie(arquivo, id);
	char reg[610];
	short tamanho;
	if(offset == -1){
		return NULL;
	} else {
		fseek(arquivo, offset, SEEK_SET);
		fread(&tamanho, sizeof(tamanho), 1, arquivo);
		int i;

		for(i=0; i<tamanho; i++){
			reg[i] = fgetc(arquivo);
		}
		reg[i] = '\0';

		return recuperaCamposEspecie(reg);
	}
}

void imprimeRegistroEspecie(tipo_Especie *especie){
	printf("\n--------------\n");
	printf("Id:              %d\n", especie->id);
	printf("Nome Cientifico: %s\n", especie->nome_cient);
	printf("Nome Popular:    %s\n", especie->nome_pop);
	printf("Descricao:       %s\n", especie->desc);
	printf("----------------------\n");
}

void modificaArquivoEspecie(FILE* arquivo, FILE* sub, int mod){
	fclose(arquivo);
	char reg[610];
	int offset = 0, flag = 1, i = 0;
	short size;
	char c;
	tipo_Especie* e;

	if(mod){
		arquivo = fopen("arquivos/especie.txt", "w+");
	} else {
		arquivo = fopen("arquivos/especie.txt", "r+");
	}

	fseek(sub, 0, SEEK_SET);

	while((c = fgetc(sub)) != EOF){
		if(c == '#'){
			if(i){
				reg[i] = '\0';
				e = recuperaCamposEspecie(reg);	
				inserirEspecie(arquivo, *e, 0);					
			}
			flag = 1;
			i = 0;
			c = fgetc(sub);
		} else if(c == '\n'){
			reg[i] = '|';
			i++;
		} else if(flag ==1){
			reg[i] = c;
			i++;
		} else if(flag ==2){
			reg[i] = c;
			i++;
		} else if(flag ==3){
			reg[i] = c;
			i++;
		} else if(flag ==4){
			reg[i] = c;
			i++;
		}
	}
	reg[i] = '\0';
	e = recuperaCamposEspecie(reg);
	inserirEspecie(arquivo, *e, 0);

	fclose(arquivo);
	arquivo = fopen("arquivos/especie.txt", "r+");
}
