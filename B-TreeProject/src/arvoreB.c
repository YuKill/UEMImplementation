#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/arvoreB.h"

#define REDIST 1
#define CONCAT -1
#define REM_LVL 1
#define NO_ACTION 0

#define FOUND 1
#define NOT_FOUND 0

#define THE_END -1
#define KEYLESS -1

#define PROMOTION 1
#define NO_PROMOTION 0
#define ERRO -1

#define MAX_KEY 4
#define MIN_KEY 2

FILE *file_arvore;
FILE *saida;

int getRoot() {
	int root;
	fseek(file_arvore, 0, SEEK_SET);
	if(!(fread(&root, sizeof(int), 1, file_arvore))) {
		root = THE_END;
		fwrite(&root, sizeof(int), 1, file_arvore);
		fwrite(&root, sizeof(int), 1, file_arvore);
	}
	return root;
}

tipo_Pagina getPage(int rrn){
	int offset = rrn*TAM;
	tipo_Pagina page;

	offset += 8;
	fseek(file_arvore, offset, SEEK_SET);
	fread(&page, sizeof(tipo_Pagina), 1, file_arvore);

	return page;
}

void writePage(int rrn, tipo_Pagina page){
	int offset = rrn*TAM;
	
	offset += 8;
	fseek(file_arvore, offset, SEEK_SET);
	fwrite(&page, sizeof(tipo_Pagina), 1, file_arvore);
}

int reutilizar() {
	tipo_Pagina page, pageAux;
	int reut, reutAux, offset;

	fseek(file_arvore, 4, SEEK_SET);
	fread(&reut, sizeof(int), 1, file_arvore);
	if(reut == THE_END) {
		return geraRRN();
	} else {
		page = getPage(reut);
		reutAux = page.key_count;

		inicializaPage(&page);
		writePage(reut, page);

		fseek(file_arvore, 4, SEEK_SET);
		fwrite(&reutAux, sizeof(int), 1, file_arvore);
		return reut;
	}
}

int search(int rrn, tipo_Key key, int *found_rrn, int *found_pos) {
	int offset, i;
	tipo_Pagina page;
	if(rrn == THE_END) {
		return NOT_FOUND;
	} else {
		page = getPage(rrn);
		*found_rrn = rrn;
		for(i=0; i<MAX_KEY; i++){
			if((page.keys[i].id > key.id) || (page.keys[i].id == KEYLESS)) {
				*found_pos = i;
				break;
			} else if(page.keys[i].id == key.id) {
				*found_pos = i;
				return FOUND;
			}
			*found_pos = i+1;
		}
		return search(page.filhos[*found_pos], key, found_rrn, found_pos);
	}
}

int searchKey(int id) {
	int root, found_rrn, offset, found_pos, state;
	tipo_Key key;
	tipo_Pagina page;
	key.id = id;

	if(!(file_arvore = fopen("arquivos/arvoreB.tree", "rb+"))) {
		printf("\n----ERRO: arquivo nao existente(talvez)----\n");
		return NOT_FOUND;
	} else {
		root = getRoot();

		if((state = search(root, key, &found_rrn, &found_pos)) == FOUND) {
			page = getPage(found_rrn);
			state = page.keys[found_pos].offset;
		} else {
			state = -1;
		}

		fclose(file_arvore);
	}
	return state;
}



int geraRRN() {
	int bytes = 0;
	fseek(file_arvore, 0, SEEK_SET);
	while(fgetc(file_arvore) != EOF) {
		bytes++;
	}
	bytes = bytes-8;
	bytes = bytes/TAM;
	return bytes;
}

void inicializaPage(tipo_Pagina *page) {
	int i;
	for(i=0; i<MAX_KEY; i++) {
		page->active = 1;
		page->key_count = 0;
		page->keys[i].id = KEYLESS;
		page->filhos[i] = THE_END;
		page->filhos[i+1] = THE_END;
	}
}

void inicializaPageAux(tipo_PaginaAux *page) {
	int i;
	for(i=0; i<MAX_KEY+1; i++) {
		page->keys[i].id = KEYLESS;
		page->filhos[i] = THE_END;
		page->filhos[i+1] = THE_END;
	}
}

void insertKeyPage(tipo_Pagina *page, tipo_Key key, int promo_child, int mod) {
	int i, aux_, s_;
	tipo_Key aux, s;

	page->key_count += 1;

	aux.id = KEYLESS;
	s.id = KEYLESS;

	for(i=0; i<MAX_KEY; i++){
		if(aux.id != KEYLESS) {
			s = page->keys[i];
			page->keys[i] = aux;
			aux = s;
			s_ = page->filhos[i+mod];
			page->filhos[i+mod] = aux_;
			aux_ = s_;
			if(aux.id == KEYLESS){
				break;
			}
		} else {
			if(page->keys[i].id == KEYLESS) {
				page->keys[i] = key;
				page->filhos[i+mod] = promo_child;
				break;	
			}
			if(page->keys[i].id > key.id) {
				aux = page->keys[i];
				page->keys[i] = key;
				aux_ = page->filhos[i+mod];
				page->filhos[i+mod] = promo_child;
			}
		}
	} 
}

void insertKeyPageAux(tipo_PaginaAux *page, tipo_Key key, int promo_child) {
	int i, aux_, s_;
	tipo_Key aux;
	aux.id = KEYLESS;
	tipo_Key s;
	s.id = KEYLESS;
	for(i=0; i<MAX_KEY+1; i++){
		if(aux.id != KEYLESS) {
			s = page->keys[i];
			page->keys[i] = aux;
			aux = s;
			s_ = page->filhos[i+1];
			page->filhos[i+1] = aux_;
			aux_ = s_;
			if(aux.id == KEYLESS){
				break;
			}
		} else {
			if(page->keys[i].id == KEYLESS) {
				page->keys[i] = key;
				page->filhos[i+1] = promo_child;
				break;	
			}
			if(page->keys[i].id > key.id) {
				aux = page->keys[i];
				page->keys[i] = key;
				aux_ = page->filhos[i+1];
				page->filhos[i+1] = promo_child;
			}
		}
	} 
}

void split(tipo_Pagina *page, tipo_Pagina *newPage, tipo_Key *promo_key, int *promo_child) {
	tipo_PaginaAux pageAux;
	inicializaPageAux(&pageAux);	

	pageAux.filhos[0] = page->filhos[0];
	int i, med = (MAX_KEY+1)/2;
	for(i=0; i<MAX_KEY; i++) {
		insertKeyPageAux(&pageAux, page->keys[i], page->filhos[i+1]);
	}
	insertKeyPageAux(&pageAux, *promo_key, *promo_child);
	
	inicializaPage(page);
	inicializaPage(newPage);
	*promo_key = pageAux.keys[med];
	*promo_child = reutilizar();

	for(i=0; i<MAX_KEY+1; i++) {
		if(pageAux.keys[i].id < promo_key->id) {
			insertKeyPage(page, pageAux.keys[i], pageAux.filhos[i+1], 1);
		} else if(pageAux.keys[i].id > promo_key->id) {
			insertKeyPage(newPage, pageAux.keys[i], pageAux.filhos[i+1], 1);
		}
	}

	page->filhos[0] = pageAux.filhos[0];
	newPage->filhos[0] = pageAux.filhos[med+1];
}

int insert(int rrn, tipo_Key key, int *promo_child, tipo_Key *promo_key) {
	int offset, pos, ret_value, i;
	tipo_Pagina page;
	if(rrn == THE_END) {
		*promo_child = THE_END;
		*promo_key = key;
		return PROMOTION;
	} else {
		page = getPage(rrn);
		for(i=0; i<MAX_KEY; i++){
			if((page.keys[i].id > key.id) || (page.keys[i].id == KEYLESS)) {
				pos = i;
				break;
			} else if(page.keys[i].id == key.id) {
				return ERRO;
			}
			pos = i+1;
		}
	}
	ret_value = insert(page.filhos[pos], key, promo_child, promo_key); 

	if((ret_value == NO_PROMOTION) || (ret_value == ERRO)) {
		return ret_value;
	} else {
		if(page.key_count < MAX_KEY) {
			insertKeyPage(&page, *promo_key, *promo_child, 1);
			writePage(rrn, page);
			return NO_PROMOTION;
		} else {
			tipo_Pagina newPage;
			split(&page, &newPage, promo_key, promo_child);
			printf("\n---------OVERFLOW!! RRN: %d----------\n", rrn);
			writePage(rrn, page);
			writePage(*promo_child, newPage);
			return PROMOTION;
		}
	}
}

int insertKey(tipo_Key key) {
	int root, promo_child, offset, newRRN, state, a;
	tipo_Key promo_key;

	if(!(file_arvore = fopen("arquivos/arvoreB.tree", "rb+"))){
		file_arvore = fopen("arquivos/arvoreB.tree", "wb+");
	}
	root = getRoot();

	if((state = insert(root, key, &promo_child, &promo_key)) == PROMOTION) {
		tipo_Pagina new;
		inicializaPage(&new);
		insertKeyPage(&new, promo_key, promo_child, 1);
		new.filhos[0] = root;
		newRRN = reutilizar();

		fseek(file_arvore, 0, SEEK_SET);
		fwrite(&newRRN, sizeof(int), 1, file_arvore);

		writePage(newRRN, new);
	} else if(state == ERRO){
		printf("\n-------ID já inserido! =/ -----------\n");
		sleep(5);
	}
	fclose(file_arvore);
	return state;
}

void removeKeyPage(tipo_Pagina *page, int pos, int mod) {
	page->keys[pos].id = KEYLESS;
	page->key_count--;
	tipo_Key key;

	int i;
	for(i=pos+1; i<MAX_KEY+1; i++) {
		if(i<MAX_KEY) {
			page->keys[i-1] = page->keys[i];
			page->filhos[i-1+mod] = page->filhos[i+mod];
			page->keys[i].id = KEYLESS;	
		} else {
			if(!mod) {
				page->filhos[i-1] = page->filhos[i];
			} 
			page->filhos[i] = THE_END;
			
		}
	}
}

int findPai(int root, int rrn, int first, int *pos) {
	int offset, i;
	tipo_Pagina pageRoot;

	pageRoot = getPage(root);
	for(i=0; i<MAX_KEY; i++){
		*pos = i;
		if(pageRoot.filhos[i] == rrn) {
			return root;
		} else if(pageRoot.filhos[i+1] == rrn) {
			*pos = i+1;
			return root;
		} else if((first < pageRoot.keys[i].id) || (pageRoot.keys[i].id == KEYLESS)) {
			break;
		}
		*pos = i + 1;
	}
	return findPai(pageRoot.filhos[*pos], rrn, first, pos);
}

int removerFromFolha(int root, tipo_Key key, int *found_rrn){
	int offset, offsetFolha, found_rrnFolha, rrn, found_posFolha, found_pos;
	tipo_Pagina page, pageFolha;

	if(search(root, key, found_rrn, &found_pos) == FOUND) {
		page = getPage(*found_rrn);
		rrn = *found_rrn;

		if(page.filhos[0] != THE_END) {
			search(page.filhos[found_pos+1], key, &found_rrnFolha, &found_posFolha);
			
			pageFolha = getPage(found_rrnFolha);

			key = pageFolha.keys[found_posFolha];
			pageFolha.keys[found_posFolha] = page.keys[found_pos];
			page.keys[found_pos] = key;

			removeKeyPage(&pageFolha, found_posFolha, 0);

			writePage(found_rrnFolha, pageFolha);
	
			*found_rrn = found_rrnFolha;
		} else {
			removeKeyPage(&page, found_pos, 0);
		}

		writePage(rrn, page);
		return FOUND;
	}
	return NOT_FOUND;
}

int verificaAcao(int rrnFolha, int *rrnPai, int *rrnFilho, int *pos, int *lado) {
	int offset, i, ini, fim;
	tipo_Pagina page, pageFilho, pageFolha;
	
	pageFolha = getPage(rrnFolha);
	if(pageFolha.key_count < MIN_KEY) {
		*rrnPai = findPai(getRoot(), rrnFolha, pageFolha.keys[0].id, pos); 
		page = getPage(*rrnPai);

		if((ini = *pos-1) < 0) {
			ini = 0;
		}
		if((fim = *pos+2) > page.key_count+1) {
			fim = page.key_count+1;
		}

		for(i=ini; i<fim; i++) {
			if(page.filhos[i] != rrnFolha) {
				pageFilho = getPage(page.filhos[i]);
				*rrnFilho = page.filhos[i];
				if(i==ini){
					*lado = 0;
				} else {
					*lado = 1;
				}
				if(pageFilho.key_count > MIN_KEY) {
					return REDIST;
				}
			}
		}
		return CONCAT;
	}
	return NO_ACTION;
}

void redistribuir(int rrnPai, int rrnIrmao, int rrnFolha, int lado, int pos) {
	tipo_Pagina pai, irmao, folha;
	tipo_Key key;
	int filho;

	printf("\n----REDISTRIBUICAO-RRN: %d----\n", rrnFolha);

	pai = getPage(rrnPai);
	irmao = getPage(rrnIrmao);
	folha = getPage(rrnFolha);

	if(lado) {
		//direita
		key = pai.keys[pos];
		filho = irmao.filhos[0];
		pai.keys[pos] = irmao.keys[0];
		
		insertKeyPage(&folha, key, filho, 1);
		removeKeyPage(&irmao, 0, 0);
	} else {
		//esquerda
		key = pai.keys[pos-1];
		filho = irmao.filhos[irmao.key_count];
		pai.keys[pos-1] = irmao.keys[irmao.key_count-1];

		insertKeyPage(&folha, key, filho, 0);
		removeKeyPage(&irmao, irmao.key_count-1, 1);
	}
	writePage(rrnPai, pai);
	writePage(rrnIrmao, irmao);
	writePage(rrnFolha, folha);
	folha = getPage(rrnFolha);
} 	

int concatenar(int rrnPai, int rrnFilho, int rrnFolha, int lado, int pos){
	int ret = 1, i, pdd, *rrn_pdd, *rrn_root;
	tipo_Pagina pai, filho, folha;
	tipo_Pagina *page, *pageR;
	tipo_Key key;

	printf("\n----CONCATENACAO-RRN: %d----\n", rrnFolha);
	
	pai = getPage(rrnPai);
	filho = getPage(rrnFilho);
	folha = getPage(rrnFolha);
	
	rrn_root = &rrnFolha;
	rrn_pdd = &rrnFilho;
	page = &folha;
	pageR = &filho;
	if(!lado) {
		rrn_root = &rrnFilho;
		rrn_pdd = &rrnFolha;
		page = &filho;
		pageR = &folha;
		pos--;
	}
	
	key = pai.keys[pos];		
	insertKeyPage(page, key, -1, 1);
	removeKeyPage(&pai, pos, 1);
	for(i=0; i<MAX_KEY; i++){
		if(pageR->keys[0].id != KEYLESS) {
			insertKeyPage(page, pageR->keys[0], pageR->filhos[0], 0);
			removeKeyPage(pageR, 0, 0);
		} else {
			page->filhos[page->key_count] = pageR->filhos[0];
			break;
		}
	}

	if((rrnPai == getRoot()) && (pai.key_count < 1)) {
		printf("\n----DIMINUI ALTURA------\n");
		fseek(file_arvore, 0, SEEK_SET);
		fwrite(rrn_root, sizeof(int), 1, file_arvore);
		ret = 0;
	}

	//Adiciona PDD
	fseek(file_arvore, 4, SEEK_SET);
	fread(&pdd, sizeof(int), 1, file_arvore);
	pageR->key_count = pdd;
	pageR->active = 0;
	fseek(file_arvore, 4, SEEK_SET);
	fwrite(rrn_pdd, sizeof(int), 1, file_arvore);

	writePage(rrnPai, pai);
	writePage(rrnFilho, filho);
	writePage(rrnFolha, folha);
	return ret;
}

int verificaKeyCount(int rrnFolha, int *rrnPai, int *rrnFilho, int *pos, int *lado){
	if(rrnFolha != getRoot()) {
		switch(verificaAcao(rrnFolha, rrnPai, rrnFilho, pos, lado)){
			case REDIST:
				redistribuir(*rrnPai, *rrnFilho, rrnFolha, *lado, *pos);
				break;

			case CONCAT:
				if(concatenar(*rrnPai, *rrnFilho, rrnFolha, *lado, *pos)){
					return verificaKeyCount(*rrnPai, rrnPai, rrnFilho, pos, lado);
				} else {
					return REM_LVL;
				} 
				break;
	
			case NO_ACTION:
				break;
		}	
	}
	return NO_ACTION;
}

int removeKey(int id) {
	int root, i, rrnFilho, rrn, rrnFolha, posFolha, pos, lado, offset, offsetFolha, ret, pdd;
	tipo_Key key;
	tipo_Pagina pageRoot;

	if(!(file_arvore = fopen("arquivos/arvoreB.tree", "rb+"))){
		printf("\n----ERRO: arquivo nao existente(talvez)----\n");
	} else {

		key.id = id;
		root = getRoot();
		
		if(removerFromFolha(root, key, &rrn) == FOUND) {
			rrnFolha = rrn;
			if((ret = verificaKeyCount(rrnFolha, &rrn, &rrnFilho, &pos, &lado)) == REM_LVL) {
				pageRoot = getPage(root);
				pageRoot.active = 0;
				fseek(file_arvore, 4, SEEK_SET);
				fread(&pdd, sizeof(int), 1, file_arvore);
				pageRoot.key_count = pdd;
				fseek(file_arvore, 4, SEEK_SET);
				fwrite(&root, sizeof(int), 1, file_arvore);
				writePage(root, pageRoot);
			}
			return FOUND;
		}
		return NOT_FOUND;
	}
}

void print(tipo_Pagina page, int rrn){
	int i;
	fprintf(saida ,"RRN: %d\n", rrn);
	fprintf(saida ,"Chaves: ");
	for(i=0; i<MAX_KEY; i++){
		if(page.keys[i].id != KEYLESS) {
			fprintf(saida ,"%d", page.keys[i].id);
			if((i+1 < MAX_KEY) && (page.keys[i+1].id != KEYLESS)) {
				fprintf(saida ," | ");
			} 
		}
	}
	fprintf(saida ,"\n");

	fprintf(saida ,"Offsets: ");
	for(i=0; i<MAX_KEY; i++){
		if(page.keys[i].id != KEYLESS) {
			fprintf(saida ,"%d", page.keys[i].offset);
			if((i+1 < MAX_KEY) && (page.keys[i].id != KEYLESS)) {
				fprintf(saida ," | ");
			} 
		}
	}

	fprintf(saida ,"\n");
	fprintf(saida ,"Filhos: ");
	for(i=0; i<MAX_KEY+1; i++){
		if(page.filhos[i] != THE_END) {
			fprintf(saida ,"%d", page.filhos[i]);
			if((i+1 < MAX_KEY+1) && (page.filhos[i+1] != THE_END)) {
				fprintf(saida ," | ");
			} 
		}
	}
	fprintf(saida ,"\n");
}

void printTree(){
	int root, count_page = 0;
	tipo_Pagina page;
	
	if(!(file_arvore = fopen("arquivos/arvoreB.tree", "rb+"))) {
		printf("\n----ERRO: arquivo nao existente(talvez)----\n");
	} else {
		saida = fopen("output/pages.txt", "w+");
		root = getRoot();
		fseek(file_arvore, 4, SEEK_CUR);
		while(fread(&page, sizeof(tipo_Pagina), 1, file_arvore)){
			if((page.active) && (page.keys[0].id != KEYLESS)) {
				fprintf(saida ,"\n");
				if(root == count_page) {
					fprintf(saida ,"---------ROOT---------\n");
					print(page, count_page);
					fprintf(saida ,"----------------------\n");
				} else {
					print(page, count_page);
				}
				fprintf(saida ,"\n");
			}
			count_page++;
		}
		fclose(saida);
		fclose(file_arvore);
	}
}

void reescrever() {
	file_arvore = fopen("arquivos/arvoreB.tree", "w+");
	fclose(file_arvore);
}

