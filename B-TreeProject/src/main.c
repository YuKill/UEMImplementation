#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "headers/especie.h"
#include "headers/individuo.h"
#include "headers/capturado.h"
/*
int main(){
	char a[] = "aaaaaaaa";
	tipo_Especie e;
	strcpy(e.nome_cient, a);
	strcpy(e.nome_pop, a);
	strcpy(e.desc, a);

	FILE* arquivo;
	arquivo = fopen("especie.txt", "r+");
	inserirEspecie(arquivo, e, 1);

	return 0;
}*/

FILE* file_especie;
FILE* file_individuo;
FILE* file_capturado;

void desenhaMenu(){
	printf("\n\n	=========================================================================\n");
	printf("	==================MONITORAMENTO DE ANIMAIS SILVESTRES====================\n");
	printf("	=========================================================================\n");
	printf("	=		1. Adicionar		                                =\n");
	printf("	=		2. Consultar (ID)					=\n");
	printf("	=		3. Remover  (ID)					=\n");
	printf("	=		4. Consultas Pré-Definidas				=\n");
	printf("	=		5. Adicionar Dados ao Banco				=\n");
	printf("	=		6. Refazer ARVORE B					=\n");
	printf("	=		7. Exportar Arvore B					=\n");
	printf("	=-----------------------------------------------------------------------=\n");
	printf("	=		0. Sair							=\n");
	printf("	=========================================================================\n\n");
}

void desenhaOpcoes(){	
	printf("\n\n	=========================================================================\n");
	printf("	=		1. Especie		                                =\n");
	printf("	=		2. Individuo						=\n");
	printf("	=		3. Capturas						=\n");
	printf("	=-----------------------------------------------------------------------=\n");
	printf("	=		0. Voltar ao Menu					=\n");
	printf("	=========================================================================\n");
}

void desenhaConsultas(){	
	printf("\n\n	=========================================================================\n");
	printf("	=		1. Historico de um Individuo                            =\n");
	printf("	=		2. Historico de uma Especie				=\n");
	printf("	=		3. Ultima Captura de um Individuo			=\n");
	printf("	=-----------------------------------------------------------------------=\n");
	printf("	=		0. Voltar ao Menu					=\n");
	printf("	=========================================================================\n");

}

void modoImportacao(){	
	printf("\n\n	=========================================================================\n");
	printf("	=		1. Juncao		                                =\n");
	printf("	=		2. Sobrescrever						=\n");
	printf("	=-----------------------------------------------------------------------=\n");
	printf("	=		0. Voltar ao Menu					=\n");
	printf("	=========================================================================\n");

}

int pedeOpcao(){
	int opcao;
	printf("	Escolha uma opcao!\n");
	printf("		-> ");
	scanf("%d", &opcao);
	return opcao;
}

void adicionarEspecie(){
	tipo_Especie e;
	
	printf("\n> Digite o ID desejado:");
	printf("\n> ");
	scanf("%d", &e.id);

	printf("\n> Digite o NOME CIENTIFICO:");
	printf("\n> ");
	fgetc(stdin);
	fgets(e.nome_cient, 50, stdin);
	e.nome_cient[strlen(e.nome_cient)-1] = '\0';	
	
	printf("\n> Digite o NOME POPULAR:");
	printf("\n> ");
	fgets(e.nome_pop, 50, stdin);
	e.nome_pop[strlen(e.nome_pop)-1] = '\0';
	
	printf("\n> Digite uma DESCRICAO:");
	printf("\n> ");
	fgets(e.desc, 400, stdin);
	e.desc[strlen(e.desc)] = '\0';

	inserirEspecie(file_especie, e, 1);

	
}

void adicionarIndividuo(){
	tipo_Individuo i;

	printf("\n> Digite o ID da espécie pertencente:");
	printf("\n> ");
	scanf("%d", &i.id_especie);

	int check;
	while ((check = buscaIdEspecie(file_especie, i.id_especie)) == -1){
		printf("> Especie não existente!");
		printf("\n> ");
		scanf("%d", &i.id_especie);
	}
	
	fgetc(stdin);
	printf("> Digite o SEXO do individuo:");
	printf("\n> ");
	i.sexo = fgetc(stdin);

	while ((i.sexo != 'm') && (i.sexo != 'f')){
		printf("Digito invalido.");
		printf("\n> ");
		i.sexo = fgetc(stdin);
	}

	inserirIndividuo(file_individuo, i, 1);
}

void adicionarCaptura(){
	tipo_Capturado c;

	printf("\n> Digite o ID do individuo:");
	printf("\n> ");
	scanf("%d", &c.id_ind);

	int check;
	while ((check = buscaIdIndividuo(file_individuo, c.id_ind)) == -1){
		printf("> Individuo não existente!");
		printf("\n> ");
		scanf("%d", &c.id_ind);
	}
	
	printf("> Digite o COMPRIMENTO do individuo:");
	printf("\n> ");
	scanf("%d", &c.comprimento);

	printf("> Digite o LARGURA do individuo:");
	printf("\n> ");
	scanf("%d", &c.largura);

	printf("> Digite o PESO do individuo:");
	printf("\n> ");
	scanf("%d", &c.peso);

	fgetc(stdin);
	printf("> Digite a DATA da captura (dd-mm-aaaa):");
	printf("\n> ");
	fgets(c.data_captura, 11, stdin);

	fgetc(stdin);
	printf("> Digite o LOCAL da captura:");
	printf("\n> ");
	fgets(c.local_captura, 100, stdin);
		
	inserirCapturado(file_capturado, c);

}

void adicionar(){
	int menu = 0;
	int opcao;
	opcao = 9;

	while(menu == 0){
		desenhaOpcoes();
		while((opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=0)){
			opcao = pedeOpcao();
		}
		switch(opcao){
			case 1:
				adicionarEspecie();
				menu = 1;
				break;
			case 2:
				adicionarIndividuo();
				menu = 1;
				break;
			case 3:
				adicionarCaptura();
				menu = 1;
				break;
			case 0:
				menu = 1;
				break;
		}
	}
}

void consultar(){
	int menu = 0;
	int opcao;
	opcao = 9;
	int id;
	char data[11];

	while(menu == 0){
		desenhaOpcoes();
		while((opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=0)){
			opcao = pedeOpcao();
		}
		switch(opcao){
			case 1:
				printf("\n> Digite o ID da especie desejada:");
				printf("\n> ");
				scanf("%d", &id);
				tipo_Especie* e;
				e = recuperaRegEspecie(file_especie, id);

				if(e == NULL){
					printf("> Não existe esta espécie!!!");
				} else {
					imprimeRegistroEspecie(e);
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 2:
				printf("\n> Digite o ID do individuo desejado:");
				printf("\n> ");
				scanf("%d", &id);
				tipo_Individuo* i;
				i = recuperaRegIndividuo(file_individuo, id);

				if(i == NULL){
					printf("> Não existe este individuo!!!");
				} else {
					imprimeRegistroIndividuo(i);
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 3:
				printf("\n> Digite o ID do individuo da captura desejada:");
				printf("\n> ");
				scanf("%d", &id);

				fgetc(stdin);
				printf("> Digite a DATA da captura desejada (dd-mm-aaaa):");
				printf("\n> ");
				fgets(data, 11, stdin);

				tipo_Capturado* c;
				c = recuperaRegCapturado(file_capturado, id, data, 0, 0);

				if(c == NULL){
					printf("> Não existe tal captura!!");
				} else {
					imprimeRegistroCapturado(c);
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 0:
				menu = 1;
				break;
		}
	}
}

void remover(){
	int menu = 0;
	int opcao;
	opcao = 9;
	int id, result;
	char data[11];

	while(menu == 0){
		desenhaOpcoes();
		while((opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=0)){
			opcao = pedeOpcao();
		}
		switch(opcao){
			case 1:
				printf("\n> Digite o ID da especie desejada:");
				printf("\n> ");
				scanf("%d", &id);
					
				result = removeRegEspecie(file_especie, id);
				
				if(!(result)){
					printf("> Não existe esta espécie!!!");
				} else {
					printf("> REGISTRO REMOVIDO");
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 2:
				printf("\n> Digite o ID do individuo desejado:");
				printf("\n> ");
				scanf("%d", &id);

				result = removeRegIndividuo(file_individuo, id);

				if(!(result)){
					printf("> Não existe este individuo!!!");
				} else {
					printf("> REGISTRO REMOVIDO");
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 3:
				printf("\n> Digite o ID do individuo da captura desejada:");
				printf("\n> ");
				scanf("%d", &id);

				fgetc(stdin);
				printf("> Digite a DATA da captura desejada (dd-mm-aaaa):");
				printf("\n> ");
				fgets(data, 11, stdin);

				result = removeRegCapturado(file_capturado, id, data, 0, 0);

				if(!(result)){
					printf("> Não existe tal captura!!");
				} else {
					printf("> REGISTRO REMOVIDO");
				}
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 0:
				menu = 1;
				break;
		}
	}

}

void consultaPre(){
	int menu = 0;
	int opcao;
	opcao = 9;
	int id, result;
	char data[11];

	while(menu == 0){
		desenhaConsultas();
		while((opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=0)){
			opcao = pedeOpcao();
		}
		switch(opcao){
			case 1:
				printf("\n> Digite o ID do individuo desejado:");
				printf("\n> ");
				scanf("%d", &id);
					
				while((result = buscaIdIndividuo(file_individuo, id)) == -1){
					printf("> Individuo não existe.");
					printf("\n> ");
					scanf("%d", &id);
				}
								
				recuperaListaCapturas(file_capturado, id);
		
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 2:
				printf("\n> Digite o ID da especie desejada:");
				printf("\n> ");
				scanf("%d", &id);
					
				while((result = buscaIdEspecie(file_especie, id)) == -1){
					printf("> Especie não existe.");
					printf("\n> ");
					scanf("%d", &id);
				}
								
				recuperaListaCapturasEspecie(file_capturado, file_individuo, id);
		
				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 3:
				printf("\n> Digite o ID do individuo da captura desejada:");
				printf("\n> ");
				scanf("%d", &id);
					
				while((result = buscaIdIndividuo(file_individuo, id)) == -1){
					printf("> Individuo não existe.");
					printf("\n> ");
					scanf("%d", &id);
				}
								
				tipo_Capturado* cap;
				cap = retornaUltimaCaptura(file_capturado, id);
				if(cap == NULL){
					printf("> Não existem registros de captura para esse individuo!");
				} else {
					printf("\n=====ULTIMA CAPTURA======");
					imprimeRegistroCapturado(cap);
				}		

				fgetc(stdin);
				fgetc(stdin);
				menu = 1;
				break;
			case 0:
				menu = 1;
				break;
		}
	}

}

void importarArquivo(char nome[], char arq){
	int menu = 0;
	int opcao;
	opcao = 9;
	FILE* import;
	int id, result;
	char data[11];
	
	if(import = fopen(nome, "r+")){

		while(menu == 0){
			modoImportacao();
			while((opcao!=1) &&(opcao!=2) &&(opcao!=0)){
				opcao = pedeOpcao();
			}
			switch(opcao){
				case 1:
					if(arq == 'e'){
						modificaArquivoEspecie(file_especie, import, 0);
					} else if(arq == 'i'){
						modificaArquivoIndividuo(file_individuo, import, 0);
					} else if(arq == 'c'){
						modificaArquivoCapturado(file_capturado, import, 0);
					}

					printf("\n\n		DONE!\n\n");
					fgetc(stdin);
					fgetc(stdin);
					menu = 1;
					break;
				case 2:
					if(arq == 'e'){
						modificaArquivoEspecie(file_especie, import, 1);
					} else if(arq == 'i'){
						modificaArquivoIndividuo(file_individuo, import, 1);
					} else if(arq == 'c'){
						modificaArquivoCapturado(file_capturado, import, 1);
					}

					printf("\n\n		DONE!\n\n");
					fgetc(stdin);
					fgetc(stdin);
					menu = 1;
					break;
				case 0:
					menu = 1;
					break;
			}
		}
	} else {
		printf("\n\n====================PROBLEMA AO ABRIR O ARQUIVO================================\n\n");
	}


}

void importar(){
	int menu = 0;
	int opcao;
	opcao = 9;
	int id, result;
	char data[11], nome[50];


	printf("\n> Informe o CAMINHO do arquivo:");
	printf("\n> ");
	scanf("%s", nome);

	while(menu == 0){
		desenhaOpcoes();
		while((opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=0)){
			opcao = pedeOpcao();
		}
		switch(opcao){
			case 1:
				importarArquivo(nome, 'e');
				menu = 1;
				break;
			case 2:
				importarArquivo(nome, 'i');
				menu = 1;
				break;
			case 3:
				importarArquivo(nome, 'c');
				menu = 1;
				break;
			case 0:
				menu = 1;
				break;
		}
	}


}

int main() {
	short size;
	int sair = 0;
	int opcao;
	opcao = 10;

	if(!(file_especie = fopen("arquivos/especie.txt", "r+"))) {
		file_especie = fopen("arquivos/especie.txt", "w+");
	}
	if(!(file_individuo = fopen("arquivos/individuo.txt", "r+"))) {
		file_individuo = fopen("arquivos/individuo.txt", "w+");
	}
	if(!(file_capturado = fopen("arquivos/capturado.data", "rb+"))) {
		file_capturado = fopen("arquivos/capturado.data", "wb+");
	}
	while(sair == 0){
		desenhaMenu();
	
		while((opcao!=0) &&(opcao!=1) &&(opcao!=2) &&(opcao!=3) &&(opcao!=4) &&(opcao!=5) &&(opcao!=6) &&(opcao!=7)){
			opcao = pedeOpcao();		
		}
	
		switch(opcao){
			case 0:
				sair = 1;
				break;
	
			case 1:
				adicionar();
				opcao = 10;
				break;
	
			case 2:
				consultar();
				opcao = 10;
				break;
				
			case 3:
				remover();
				opcao = 10;
				break;
			
			case 4:
				consultaPre();
				opcao = 10;
				break;
	
			case 5:
				importar();
				opcao = 10;
				break;

			case 6:
				reescrever();
				rewriteArvore(file_especie);	
				opcao = 10;
				break;

			case 7:
				printTree();
				printf("\n> Arvore B exportada para: output/pages.txt\n");
				sleep(1);
				opcao = 10;
				break;	
		}
	}
	fclose(file_especie);
	fclose(file_individuo);
	fclose(file_capturado);
	return 0;
}

