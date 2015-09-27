#include "../../data/funcionario.h"
#ifndef DB_H
#define DB_H

void init_db(char *nome);
int cadastrar_db(char *nome, funcionario func);
int remover_db(char *nome, funcionario func);
void consulta_db(char *nome, char *at, char *value, int idade);

#endif
