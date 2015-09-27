#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include "headers/capturado.h"
#include "headers/individuo.h"

int inserirCapturado(FILE* arquivo, tipo_Capturado capturado){
	char buffer[200], leitura[200];
	short size, size_r;
	int bytes, offset = 0;

	sprintf(buffer, "%d|%d|%d|%d|%s|%s", capturado.id_ind, capturado.comprimento, capturado.largura, capturado.peso, capturado.data_captura, capturado.local_captura);

	size = strlen(buffer);	
	

	fseek(arquivo, 0, SEEK_SET);
	while((bytes = fread(&size_r, 1, sizeof(size_r), arquivo)) != 0) {
		fread(leitura, 1, size_r, arquivo);
		leitura[size_r] = '\0';

		offset = offset + size_r + sizeof(size_r);
		fseek(arquivo, offset, SEEK_SET);
	}
	
	fwrite(&size, 1, sizeof(size), arquivo);
	fwrite(buffer, 1, size, arquivo);
}

int buscaIdCapturado(FILE* arquivo, int id_ind, char data_captura[], int off, int offset_inicial){
	char leitura[200];
	short size;
	int bytes, i, offset = 0, data;
	// off == -1 -> nao ira comparar data. Ira pegar o primeiro registro a partir de offset_inicial, que seja igual a id_ind
	if(off == -1){
		offset = offset_inicial;
	}
	fseek(arquivo, offset, SEEK_SET);
	while((bytes = fread(&size, 1, sizeof(size), arquivo)) != 0) {
		fread(leitura, 1, size, arquivo);
		if(leitura[0] != '$'){
			leitura[size] = '\0';	
			if(comparaID(leitura, id_ind)){
				if(off != -1){
					if(!(comparaData(leitura, data_captura))){
						return offset;
					}
				} else {
					return offset;
				}
			}
		}
		offset = offset + size + sizeof(size);
		fseek(arquivo, offset, SEEK_SET);
	} 			
	return -1;
}

int comparaID(char registro[], int id_ind){
	int i, id_reg;
	char id_st[10];
	for(i = 0; i < strlen(registro); i++){
		if(registro[i] != '|'){
			id_st[i] = registro[i];
		} else {
			break;
		}
	}
	id_st[i] = '\0';
	id_reg = atoi(id_st);
	if(id_reg == id_ind){
		return 1;
	} else {
		return 0;
	}
}

int dtoi(char data[]){
	int i, off = 0;
	char datas[11];
	for(i = 0; i < strlen(data); i++){
		if(data[i] != '-'){
			datas[off] = data[i];
			off++;
		}
	}
	datas[off] = '\0';
	return atoi(datas);
}

void pegaDiaMesAno(int data, int *dma){
	int *pont = dma;
	int dia, mes, ano;
	//dia
	dia = data/100000;
	//mes
	mes = data/10000;
	mes = data%100;
	//ano
	ano = data%10000;
	*pont = dia;
	pont++;
	*pont = mes;
	pont++;
	*pont = ano;
}

int comparaData(char registro[], char data_captura[]){
	int datai, i, flag = 0, off = 0, datac;
	int dma[3], dma2[3];
	char data[11];
	for(i = 0; i < strlen(registro); i++){
		if(registro[i] == '|'){
			if(flag == 4){
				break;
			}
			flag++;
		} else if(flag == 4){
			data[off] = registro[i];
			off++;
		}
	}	
	data[off] = '\0';
	datai = dtoi(data);
	datac = dtoi(data_captura);


	if(datai == datac){
		return 0;
	}

	pegaDiaMesAno(datac, &dma);
	pegaDiaMesAno(datai, &dma2);

	if(dma[2] > dma2[2]){
		return -1; //a data que foi passada é maior do que a data do registro
	} else if (dma[2] < dma2[2]){
		return 1; //a data que foi passada é menor do que a data do registro
	} else {
		if(dma[1] > dma2[1]){
			return -1;
		} else if(dma[1] < dma2[1]){
			return 1;
		} else {
			if(dma[0] > dma2[0]){
				return -1;
			} else if(dma[0] < dma2[0]){
				return 1;
			}
		}
	}

}

tipo_Capturado* recuperaCamposCapturado(char registro[]){
	tipo_Capturado* c = malloc(sizeof(tipo_Capturado)+1);
	int  i, flag = 1, j = 0;
	char field_id[11], field_comprimento[11], field_largura[11], field_peso[11], field_data[11], field_local[101];
	for(i = 0; i < strlen(registro); i++){
		if(registro[i] == '|'){
			if(flag == 1) {
				field_id[j] = '\0';
			} else if(flag == 2) {
				field_comprimento[j] = '\0';
			} else if(flag == 3) {
				field_largura[j] = '\0';
			} else if(flag == 4) {
				field_peso[j] = '\0';
			} else if(flag == 5) {
				field_data[j] = '\0';
			} else if(flag == 6) {
				break;
			}	
			flag++;
			j = 0;
		} else if(flag == 1) {
			field_id[j] = registro[i];
			j++;
		} else if(flag == 2) {
			field_comprimento[j] = registro[i];
			j++;
		} else if(flag == 3) {
			field_largura[j] = registro[i];
			j++;			
		} else if(flag == 4) {
			field_peso[j] = registro[i];
			j++;
		} else if(flag == 5) {
			field_data[j] = registro[i];
			j++;
		} else if(flag == 6) {
			field_local[j] = registro[i];
			j++;
		}	
	}
	field_local[j-1] = '\0';
	c->id_ind = atoi(field_id);
	c->comprimento = atoi(field_comprimento);
	c->largura = atoi(field_largura);
	c->peso = atoi(field_peso);
	strcpy(c->data_captura, field_data);
	strcpy(c->local_captura, field_local);	
	return c;
}

tipo_Capturado* recuperaRegCapturado(FILE* arquivo, int id_ind, char data[], int off, int offset_inicial){
	int offset = buscaIdCapturado(arquivo, id_ind, data, off, offset_inicial);
	char reg[201];
	short size;
	if(offset == -1){
		return NULL;
	} else {
		fseek(arquivo, offset, SEEK_SET);
		fread(&size, 1, sizeof(size), arquivo);
		fread(reg, 1, size, arquivo);
		reg[size] = '\0';
		return recuperaCamposCapturado(reg);
	}
}

int removeRegCapturado(FILE* arquivo, int id_ind, char data[], int off, int offset_inicial){
	int offset = buscaIdCapturado(arquivo, id_ind, data, off, offset_inicial);
	char pipe[] = "$";
	if(offset == -1){
		return 0;
	} else {
		offset += 2;
		fseek(arquivo, offset, SEEK_SET);
		fwrite(pipe, 1, strlen(pipe), arquivo);
		return 1;
	}
}

void imprimeRegistroCapturado(tipo_Capturado* capturado){
	printf("-----------\n");
	printf("Comprimento:       %d\n", capturado->comprimento);
	printf("Largura:           %d\n", capturado->largura);
	printf("Data de Captura:   %s\n", capturado->data_captura);
	printf("Local de Captura:  %s\n", capturado->local_captura);
	printf("-----------\n");

}

tipo_Capturado* retornaUltimaCaptura(FILE* arquivo, int id_ind){
	int offset = 0;
	char data[] = "aa";
	char date[201];
	char maior[] = "00-00-0000a";
	maior[10] = '\0';
	tipo_Capturado *c, *m;
	m = NULL;
	short size;
	int result;
	while((offset = buscaIdCapturado(arquivo, id_ind, data, -1, offset)) != -1) {
		c = recuperaRegCapturado(arquivo, id_ind, data, -1, offset);
		
			
		fseek(arquivo, offset, SEEK_SET);
		fread(&size, 1, sizeof(size), arquivo);
		sprintf(date, "%d|%d|%d|%d|%s|%s|", c->id_ind, c->comprimento, c->largura, c->peso, c->data_captura, c->local_captura);
	
		if((result = comparaData(date, maior)) == 1){
			strcpy(maior, c->data_captura);
			m = c;
		}
 
		offset = offset + size + sizeof(size);
	}
	return m;
}

void recuperaListaCapturas(FILE* arquivo, int id_ind){
	int offset = 0;
	char data[] = "aa";
	tipo_Capturado *cap;
	short size;
	
	printf("\n\n------Lista do Individuo de ID = %d -----------\n", id_ind);
	while((offset = buscaIdCapturado(arquivo, id_ind, data, -1, offset)) != -1) {
		cap = recuperaRegCapturado(arquivo, id_ind, data, -1, offset);
		fseek(arquivo, offset, SEEK_SET);
		fread(&size, 1, sizeof(size), arquivo);
		imprimeRegistroCapturado(cap);

		offset = offset + size + sizeof(size);
	}
	printf("\n==================================================\n\n");	
}

void recuperaListaCapturasEspecie(FILE* file_cap, FILE* file_ind, int id){ //testar!!!!!!!!!!!!!!
	int offset = 0;
	char data[] = "aa";
	char reg[201];
	tipo_Capturado *cap;
	tipo_Individuo *ind;
	short size;
	int bytes;

	fseek(file_cap, 0, SEEK_SET);
	printf("\n\n--------Lista de Capturas da Espécie: %d -----------\n", id);
	while((bytes = fread(&size, 1, sizeof(size), file_cap)) != 0){
		fread(reg, 1, size, file_cap);
		reg[size] = '\0';
		cap = recuperaCamposCapturado(reg);
		ind = recuperaRegIndividuo(file_ind, cap->id_ind);

		if(ind != NULL){
			if(ind->id_especie == id){
				printf("\n  -=Individuo %d=-\n", cap->id_ind);
				imprimeRegistroCapturado(cap);
			}
		}
	} 
}

void modificaArquivoCapturado(FILE* arquivo, FILE* sub, int mod){
	fclose(arquivo);
	char reg[201];
	int bytes, offset = 0, flag = 1, i = 0;
	short size;
	char c;
	tipo_Capturado* e;

	if(mod){
		arquivo = fopen("arquivos/capturado.data", "w+b");
	} else {
		arquivo = fopen("arquivos/capturado.data", "r+b");
	}

	fseek(sub, 0, SEEK_SET);

	while((c = fgetc(sub)) != EOF){
		if(c == '#'){
			if(i){
				reg[i] = '\0';
				e = recuperaCamposCapturado(reg);	
				inserirCapturado(arquivo, *e);					
			}
			flag = 1;
			i = 0;
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
		} else if(flag ==5){
			reg[i] = c;
			i++;
		} else if(flag ==6){
			reg[i] = c;
			i++;
		}
	}
	reg[i] = '\0';
	e = recuperaCamposCapturado(reg);
	inserirCapturado(arquivo, *e);

	fclose(arquivo);
	arquivo = fopen("arquivos/capturado.data", "r+b");
}
