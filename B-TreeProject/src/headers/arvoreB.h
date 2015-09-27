#ifndef ARVOREB_H
#define ARVOREB_H

typedef struct Key{
	int id;
	int offset;
}tipo_Key;

typedef struct PaginaAux{
	tipo_Key keys[5];
	short filhos[6];
}tipo_PaginaAux;

typedef struct Pagina{
	short active;
	short key_count;
	tipo_Key keys[4];
	short  filhos[5];
}tipo_Pagina;

#define TAM sizeof(tipo_Pagina);

int searchKey(int id);

int removeKey(int id);

int insertKey(tipo_Key key);

void reescrever();

void printTree();

#endif
