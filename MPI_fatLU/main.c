#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lu.h"

static int n = 0;
static char input_name[128];
static FILE *input = NULL;
static FILE *output = NULL;

void draw_help() {
  printf("\nPara executar esse programa, sao necessarios os seguintes parametros:\n");
  printf("\t-i: especifica o arquivo de entrada;\n");
  printf("\t-n: especifica a quantidade de equacoes.\n");
  printf("Parametros opcionais:\n");
  printf("\t-o: especifica o arquivo de entrada.\n");
  exit(1);
}

void get_params(int argc, char **argv) {
  int i;
  for (i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-n")) {
      i++;
      n = atoi(argv[i]);
    } else if (!strcmp(argv[i], "-i")) {
      i++;
      input = fopen(argv[i], "r");
      strcpy(input_name, argv[i]);
    } else if (!strcmp(argv[i], "-o")) {
      i++;
      output = fopen(argv[i], "w");
    }
  }
  if (!n || !input) draw_help();
  if (!output) {
    printf("Output file not specified. Redirecting to stdout.\n");
    output = stdout;
  }
}

void read_input(double *a, double *r) {
  int i;
  for (i = 0; i < n*n; i++) {
    fscanf(input, "%lf", &a[i]);
    if ((i % n == n-1) && (i))
      fscanf(input, "%lf", &r[i/n]);
  }
}

int main(int argc, char **argv) {
  double out_start, out_end;
  out_start = get_time();

  get_params(argc, argv);

  double *a = (double*)malloc(sizeof(double)*(n*n)), 
         r[n], var[n];
  read_input(a, r);

  double time_ = solve(n, a, var, r);
  out_end = get_time();

  printf("%s 1 %.4lf %.4lf 0 0\n", input_name, time_, out_end - out_start);
  fprint_vector(output, n, var);

  /*
  if (solved)
    print_vector(n, var);
    */
}
