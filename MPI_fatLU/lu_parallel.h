#ifndef LU_H
#define LU_H

#include <stdio.h>
#include <math.h>
#include <sys/time.h>

#define PRECISION 0.01

#define MASTER 0
#define SLAVE 42

#define DATA 0
#define LOCAL 1
#define ROOT 2

void print_(int n, double *a, int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (i % n == 0) printf("\n");
    printf("%.2lf ", a[i]);
  }
  printf("\n");
}

void fprint_vector(FILE *f, int n, double *a) {
  int i;
  for (i = 0; i < n; i++)
    fprintf(f, "%.2lf ", a[i]);
  fprintf(f, "\n");
}

void print_vector(int n, double *a) {
  int i;
  for (i = 0; i < n; i++)
    printf("%.2lf ", a[i]);
  printf("\n");
}

void print_matrix(int n, double *a) {
  int i;
  for (i = 0; i < n*n; i++) {
    if (i % n == 0) printf("\n");
    printf("%.2lf ", a[i]);
  }
  printf("\n");
}

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double) (tv.tv_sec) + 0.000001 * tv.tv_usec;
}

void mult_matrix(int n, int m, int o, double *a, double *b, double *r) {
  int i, j, k;
  for (i = 0; i < n; i++)
    for (j = 0; j < o; j++) {
      double sum = 0;
      for (k = 0; k < m; k++)
        sum += (a[k*m + i] * b[j*o + k]);
      r[j*o + i] = sum;
    }
}

void mult_square_matrix(int n, double *a, double *b, double *r) {
  mult_matrix(n, n, n, a, b, r);
}

int is_equal(double a, double b) {
  return (a - b < PRECISION);
}

int verify_lu(int n, double *a, double *l, double *u) {
  int i, no_error = 1;
  double *v = (double*) malloc(sizeof(double)*(n*n));
  mult_square_matrix(n, l, u, v);
  for (i = 0; i < n*n; i++) {
    if (!is_equal(a[i], v[i])) {
      no_error = 0;

    }
  }

  free(v);
  return no_error;
}

int verify_ls(int n, double *a, double *var, double *r) {
  int i;
  double res[n];
  mult_matrix(n, n, 1, a, var, res); 
  for (i = 0; i < n; i++)
    if (!is_equal(res[i], r[i])) return 0;
  return 1;
}

void solve_lx(int n, double *l, double *var, double *r) {
  int i, j;
  for (i = 0; i < n; i++) {
    var[i] = r[i];
    for (j = i-1; j >= 0; j--)
      var[i] -= (l[j*n + i] * var[j]);
    var[i] /= l[i*n + i];
  }
}

void solve_uy(int n, double *u, double *var, double *r) {
  int i, j;
  for (i = n-1; i >= 0; i--) {
    var[i] = r[i];
    for (j = i+1; j < n; j++)
      var[i] -= (u[j*n + i] * var[j]);
    var[i] /= u[i*n + i];
  }
}

void solve(int n, double *l, double *u, double *var, double *r) {
  double r1[n];
  solve_lx(n, l, r1, r);
  solve_uy(n, u, var, r1);
}

void lu_parallel(int n, int rank, int nof_slaves) {
  int i, j, k, cols;
  double *u, *l;

  u = (double*) malloc(sizeof(double) * n * n);
  l = (double*) malloc(sizeof(double) * n);

  for (j = (rank-1); j < n; j += nof_slaves)
    MPI_Recv(&u[j*n], n, MPI_DOUBLE, MASTER, DATA,
        MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  for (j = 0; j < n-1; j++) {
    int owner = (j % nof_slaves) + 1;
    if (owner == rank) {
      for (i = j+1; i < n; i++) {
        l[i] = u[j*n + i] / u[j*n + j];
      }
    }

    MPI_Bcast(&l[j+1], n-(j+1), MPI_DOUBLE, owner, MPI_COMM_WORLD);

    int offset = (rank-1) - ((j+1) % nof_slaves);
    if (offset < 0)
      offset = (rank-1) + nof_slaves - ((j+1) % nof_slaves);

    for (k = (j+1) + offset; k < n; k += nof_slaves)
      for (i = j+1; i < n; i++) 
        u[k*n + i] -= (l[i] * u[k*n + j]);
  }


  for (i = (rank-1); i < n; i += nof_slaves) 
    MPI_Send(&u[i*n], n, MPI_DOUBLE, MASTER, DATA, MPI_COMM_WORLD);

  free(u);
  free(l);
}

#endif
