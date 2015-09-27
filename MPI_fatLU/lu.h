#ifndef LU_H
#define LU_H

#include <sys/time.h>
#include <stdio.h>
#include <math.h>

#define PRECISION 2

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
        sum += (a[i*m + k] * b[k*o + j]);
      r[i*o + j] = sum;
    }
}

void mult_square_matrix(int n, double *a, double *b, double *r) {
  mult_matrix(n, n, n, a, b, r);
}

int is_equal(double a, double b) {
  return (a - b <= pow(10, -PRECISION));
}

int verify_lu(int n, double *a, double *l, double *u) {
  int i, no_error = 1;
  double *v = (double*) malloc(sizeof(double)*(n*n));
  mult_square_matrix(n, l, u, v);
  for (i = 0; i < n*n; i++) {
    if (!is_equal(a[i], v[i])) {
      no_error = 0;
      break;
    }
  }

  free(v);
  return no_error;
}

int verify_ls(int n, double *a, double *var, double *r) {
  int i, no_error = 1;
  double res[n];
  mult_matrix(n, n, 1, a, var, res); 
  for (i = 0; i < n; i++)
    if (!is_equal(res[i], r[i])) {
      printf("Error: %.2lf != %.2lf\n", res[i], r[i]);
      no_error = 0;
    }
  return no_error;
}

void solve_lx(int n, double *l, double *var, double *r) {
  int i, j;
  for (i = 0; i < n; i++) {
    var[i] = r[i];
    for (j = i-1; j >= 0; j--)
      var[i] -= (l[i*n + j] * var[j]);
    var[i] /= l[i*n + i];
  }
}

void solve_uy(int n, double *u, double *var, double *r) {
  int i, j;
  for (i = n-1; i >= 0; i--) {
    var[i] = r[i];
    for (j = i+1; j < n; j++)
      var[i] -= (u[i*n + j] * var[j]);
    var[i] /= u[i*n + i];
  }
}

void lu(int n, double *a, double *l, double *u) {
  int i, j, k;

  for (i = 0; i < n*n; i++) {
    u[i] = a[i];
    if ((i / n) == (i % n)) l[i] = 1;
    else l[i] = 0;
  }

  for (j = 0; j < n-1; j++) {
    for (i = j+1; i < n; i++) {
      double m = - u[i*n + j] / u[j*n + j];
      for (k = j; k < n; k++) 
        u[i*n + k] = (m * u[j*n + k]) + u[i*n + k];
      l[i*n + j] = -m;
    }
  }
}

double solve(int n, double *a, double *var, double *r) {
  int no_error = 1;
  double *l = (double*) malloc(sizeof(double)*(n*n)), 
         *u = (double*) malloc(sizeof(double)*(n*n)), 
         r1[n];
  double start, end;

  start = get_time();
  lu(n, a, l, u);
  end = get_time();

  /*
  if (!verify_lu(n, a, l, u)) {
    printf("Error: lu decomposition not done right.\n");
    no_error = 0;
  } else printf("LU executed successfully!\n");
  */

  if (no_error) {
    //printf("Solving the linnear system.\n");

    solve_lx(n, l, r1, r);
    if (!verify_ls(n, l, r1, r))
      printf("Error: lx yield not expected value.\n");

    solve_uy(n, u, var, r1);
    if (!verify_ls(n, u, var, r1))
      printf("Error: uy yield not expected value.\n");

    if (!verify_ls(n, a, var, r)) {
      printf("Error: linnear system not solved right.\n");
      no_error = 0;
    } //else printf("Linnear system solved successfully!\n");
  }

  free(l);
  free(u);
  return end-start;
}

#endif
