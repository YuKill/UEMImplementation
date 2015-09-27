#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

typedef struct {
	char nome[100];
	char sobrenome[100];
	char cidade[50];
	char estado[50];
	int idade;
} funcionario;

int send_funcionario(char cmd[], funcionario cad);
void print_func(funcionario f);
funcionario *chartof(char *buf);
char *ftochar(funcionario func);
char *get_ip_client(char *pack);
char *get_cmd(char *pack);
void take_out_ip(char *pack);

#endif
