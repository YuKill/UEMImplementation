#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mpi.h>

#include "lu_parallel.h"

static int n = 0;
static int nof_processes = 0;
static char input_name[128];
static FILE *input = NULL;
static FILE *output = NULL;

void draw_help() {
  printf("\nPara executar esse programa, sao necessarios os seguintes parametros:\n");
  printf("\t-i: especifica o arquivo de entrada;\n");
  printf("\t-n: especifica a quantidade de equacoes.\n");
  printf("Parametros opcionais:\n");
  printf("\t-o: especifica o arquivo de saida.\n");
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
    printf("File not specified. Redirecting to stdout.\n");
    output = stdout;
  }
}

void read_input(double *a, double *l, double *u, double *r) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      // Reading A
      fscanf(input, "%lf", &a[j*n + i]);
      // Setting U
      u[j*n + i] = a[j*n + i];
      // Setting L
      l[j*n + i] = 0;
      if (i == j) l[j*n + i] = 1;
    }
    // Setting R
    fscanf(input, "%lf", &r[i]);
  }
}

int main(int argc, char **argv) {
  int rank, nof_slaves;
  double out_start, out_end;

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nof_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  nof_slaves = nof_processes -1;
  if (rank == MASTER) {
    out_start = get_time();

    int i, j, k;
    double start, end, stime, rtime; 

    get_params(argc, argv);
    double *a = (double*) malloc(sizeof(double)*(n*n)), 
           *l = (double*) malloc(sizeof(double)*(n*n)),
           *u = (double*) malloc(sizeof(double)*(n*n)),
           r[n], var[n], *pkg = NULL;
    read_input(a, l, u, r);

    start = get_time();

    MPI_Bcast(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    stime = get_time();    
    for (j = 0; j < n; j++) 
      MPI_Send(&u[j*n], n, MPI_DOUBLE, (j % nof_slaves) + 1, DATA, MPI_COMM_WORLD);
    stime = get_time()-stime;

    for (i = 0; i < n-1; i++) 
      MPI_Bcast(&l[i*n + i+1], n-(i+1), MPI_DOUBLE, (i % nof_slaves) + 1, MPI_COMM_WORLD); 

    double *current = u;
    rtime = get_time();
    for (i = 0; i < n; i++) {
      MPI_Recv(current, n, MPI_DOUBLE, (i % nof_slaves) + 1, DATA, 
          MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
      current += n;
    }
    rtime = get_time()-rtime;

    end = get_time();

    //printf("Master: decomposition took %.4lfs.\n", end-start);

    /*
    if (!verify_lu(n, a, l, u)) {
      printf("Master: error in lu.\n");
    } else printf("Master: lu decomposed successfully.\n");
    */

    solve(n, l, u, var, r);
    if (!verify_ls(n, a, var, r)) {
      printf("Master: error in solving system.\n");
    } //else printf("Master: linnear system solved successfully.\n");

    fprint_vector(output, n, var);

    free(a);
    free(l);
    free(u);

    out_end = get_time();

    printf("%s %d %.4lf %.4lf %.4lf %.4lf\n", input_name, nof_slaves, 
        end-start, out_end-out_start, stime, rtime);
  } else {
    MPI_Bcast(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    lu_parallel(n, rank, nof_slaves);
  }

  /*
     if (solved)
     print_vector(n, var);
     */

  MPI_Finalize();
}
