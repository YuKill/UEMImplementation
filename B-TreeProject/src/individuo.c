#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "headers/individuo.h"

int inserirIndividuo(FILE* arquivo, tipo_Individuo ind, int mod){
	char id[5];
	char buffer[12];
	char buf[4], sexo[2];
	int offset = 0, i, offsetI = 0, maior = 0, num_id;

	buffer[0] = '\0';	
	sexo[1] = '\0';

	fseek(arquivo, 0, SEEK_SET);
	while((id[0] = fgetc(arquivo)) != EOF){
		if(id[0] == '$'){
			offsetI = offset;
		} else {
			for(i = 1; i < 4; i++){
				id[i] = fgetc(arquivo);
			}
			id[5] = '\0';
			num_id = atoi(id);
			if(num_id > maior) {
				maior = num_id;
			}
		}
		offset = offset + 12;
		fseek(arquivo, offset, SEEK_SET);
	}
	if(mod){
		ind.id = maior + 1;
	}

	if(!offsetI){
		offsetI = offset;
	}


	if(!(ind.id/10)){
		sprintf(buf, "000%d", ind.id);
	} else if (!(ind.id/100)){
		sprintf(buf, "00%d", ind.id);
	} else if (!(ind.id/1000)){
		sprintf(buf, "0%d", ind.id);
	} else {
		sprintf(buf, "%d", ind.id);
	}

	strcat(buffer, buf);
	strcat(buffer, "|");


	if(!(ind.id_especie/10)){
		sprintf(buf, "000%d", ind.id_especie);
	} else if (!(ind.id_especie/100)){
		sprintf(buf, "00%d", ind.id_especie);
	} else if (!(ind.id_especie/1000)){
		sprintf(buf, "0%d", ind.id_especie);
	} else {
		sprintf(buf, "%d", ind.id_especie);
	}
	
	sexo[0] = ind.sexo;

	strcat(buffer, buf);
	strcat(buffer, "|");

	strcat(buffer, sexo);
	strcat(buffer, "|");	

	fseek(arquivo, offsetI, SEEK_SET);
	fputs(buffer, arquivo);
}

int buscaIdIndividuo(FILE* arquivo, int id){
	int id_num, i, offset = 0;
	char id_st[4];
	
	fseek(arquivo, 0, SEEK_SET);
	while((id_st[0] = fgetc(arquivo)) != EOF) {
		if(id_st[0] != '$') {
			for(i=1; i<4; i++){
				id_st[i] = fgetc(arquivo);	
			}

			id_num = atoi(id_st);
			
			if(id_num == id){
				return offset;
			}
		}
		
		offset = offset + 12;
		fseek(arquivo, offset, SEEK_SET);
	}
	return -1;
}

tipo_Individuo* recuperaCamposIndividuo(char registro[]){
	tipo_Individuo* ind = malloc(sizeof(tipo_Individuo)+1);
	char field_id[4], field_idesp[4], field_sexo;
	int i, flag = 1, j = 0;

	field_id[4] = '\0';
	for(i = 0; i < 12 ; i++){
		if(registro[i] == '|'){
			j = 0;
			flag++;
		} else if(flag == 1){
			field_id[j] = registro[i];
			j++;
		} else if(flag == 2){
			field_idesp[j] = registro[i];
			j++;
		} else if(flag == 3){
			field_sexo = registro[i];
			break;
		}
	}
	
	ind->id = atoi(field_idesp);
	ind->id_especie = atoi(field_idesp);
	ind->sexo = field_sexo;

	return ind;
}

tipo_Individuo* recuperaRegIndividuo(FILE* arquivo, int id){
	int i,offset = buscaIdIndividuo(arquivo, id);
	char reg[13];

	if(offset == -1){
		return NULL;
	} else {
		fseek(arquivo, offset, SEEK_SET);
		for(i = 0; i < 12; i++){
			reg[i] = fgetc(arquivo);
		}
		reg[12] = '\0';
		return recuperaCamposIndividuo(reg);
	}
}

int removeRegIndividuo(FILE* arquivo, int id){

	int offset = buscaIdIndividuo(arquivo, id);
	char sif[2];
	sif[1] = '\0';
	sif[0] = '$';

	if(offset == -1){
		return 0;
	} else {
		fseek(arquivo, offset, SEEK_SET);
		fputs(sif, arquivo);
		return 1;
	}
}

void imprimeRegistroIndividuo(tipo_Individuo *ind){
	printf("\n--------------\n");
	printf("Id:          %d\n", ind->id);
	printf("Id-Especie:  %d\n", ind->id_especie);
	printf("Sexo:        %c\n", ind->sexo);
	printf("-------------\n");
}

void modificaArquivoIndividuo(FILE* arquivo, FILE* sub, int mod){
	fclose(arquivo);
	char tam[4], reg[13];
	int offset = 0, flag = 1, i = 0;
	short size;
	char c;
	tipo_Individuo* e;

	if(mod){
		arquivo = fopen("../arquivos/individuo.txt", "w+");
	} else {
		arquivo = fopen("../arquivos/individuo.txt", "r+");
	}

	fseek(sub, 0, SEEK_SET);

	while((c = fgetc(sub)) != EOF){
		if(c == '#'){
			if(i){
				reg[i] = '\0';
				e = recuperaCamposIndividuo(reg);	
				inserirIndividuo(arquivo, *e, 1);					
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
		}
	}
	reg[i] = '\0';
	e = recuperaCamposIndividuo(reg);
	inserirIndividuo(arquivo, *e, 1);

	fclose(arquivo);
	arquivo = fopen("../arquivos/individuo.txt", "r+");
}
