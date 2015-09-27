#include <stdlib.h>
#include "../data/funcionario.h"



int main() {
	int sair = 0;

	while(sair == 0) {
		mostra_menu();
		switch(recebe_op()) {
			case 1:
				cadastrar();
				break;
			case 2:
				consultar();
				break;
			case 3:
				remover();
				break;
			case 4:
				sair = 1;
				break;
			default:
				printf("Opcao nao existente.\n");
				break;
		}
		printf("\n");
	}

	return 0;
}

void mostra_menu() {
	printf("    ------------------\n");
	printf("    - 1. Cadastrar   -\n");
	printf("    - 2. Consultar   -\n");
	printf("    - 3. Remover     -\n");
	printf("    ------------------\n");
	printf("    - 4. Sair        -\n");
	printf("    ------------------\n");
}

int recebe_op() {
	int op;
	char lin;

	printf("> Digite uma opcao: ");
	scanf("%d", &op);
	scanf("%c", &lin);

	return op;
}

void cadastrar() {
	funcionario cad;
	char lin;
	printf("\n+++++++++++++++++++++++++++++++");
	printf("\n++++++++++ CADASTRAR ++++++++++");
	printf("\n+++++++++++++++++++++++++++++++\n");
	printf("\n> Nome: ");
	gets(cad.nome);
	printf("\n> Sobrenome: ");
	gets(cad.sobrenome);
	printf("\n> Cidade: ");
	gets(cad.cidade);
	printf("\n> Estado: ");
	gets(cad.estado);
	printf("\n> Idade: ");
	scanf("%d", &cad.idade);
	scanf("%c", &lin);

	if(!send_funcionario("add", cad)) {
		printf("> Falha no cadastramento!");
	}
}

void consultar() {
	int op;
	funcionario busca;
	char opc[10], lin;
	printf("\n+++++++++++++++++++++++++++++++");
	printf("\n++++++++++ CONSULTAR ++++++++++");
	printf("\n+++++++++++++++++++++++++++++++\n");
	printf("\n>Consultar por:");
	printf("\n     Opção ( 1 )> NOME:");
	printf("\n     Opção ( 2 )> SOBRENOME:");
	printf("\n     Opção ( 3 )> CIDADE:");
	printf("\n     Opção ( 4 )> ESTADO:");
	printf("\n     Opção ( 5 )> IDADE:\n");

	op = 0;
	strcpy(busca.cidade, "$");
	strcpy(busca.estado, "$");
	busca.idade = 0;
	while ((op > 5) || (op < 1) )  {
		op = recebe_op();
		switch(op) {
			case 1:
				printf("> Digite NOME:");
				gets(busca.sobrenome);
				strcpy(busca.nome, "nome");
				printf("---------------------------------------------\n");
				send_funcionario("bus", busca);
				break;
			case 2:
				printf("> Digite SOBRENOME:");
				gets(busca.sobrenome);
				strcpy(busca.nome, "sobrenome");
				printf("---------------------------------------------\n");
				send_funcionario("bus", busca);
				break;
			case 3:
				printf("> Digite CIDADE:");
				gets(busca.sobrenome);
				strcpy(busca.nome, "cidade");
				printf("---------------------------------------------\n");
				send_funcionario("bus", busca);
				break;
			case 4:
				printf("> Digite ESTADO:");
				gets(busca.sobrenome);
				strcpy(busca.nome, "estado");
				printf("---------------------------------------------\n");
				send_funcionario("bus", busca);
				break;			
			case 5:
				printf("> Digite IDADE:");
				scanf("%d", &busca.idade);
				scanf("%c", &lin);
				strcpy(busca.nome, "idade");
				printf("---------------------------------------------\n");
				send_funcionario("bus", busca);
				break;
			default:
				op = recebe_op();
				break;
		}
	}

	printf("---------------------------------------------\n");
}

void remover() {
	funcionario rem;
	char lin;

	printf("\n+++++++++++++++++++++++++++++++");
	printf("\n+++++++++++ REMOVER +++++++++++");
	printf("\n+++++++++++++++++++++++++++++++\n");

	printf("\n> Nome: ");
	gets(rem.nome);
	printf("\n> Sobrenome: ");
	gets(rem.sobrenome);
	printf("\n> Cidade: ");
	gets(rem.cidade);
	printf("\n> Estado: ");
	gets(rem.estado);
	printf("\n> Idade: ");
	scanf("%d", &rem.idade);
	scanf("%c", &lin);

	send_funcionario("rem", rem);


}
