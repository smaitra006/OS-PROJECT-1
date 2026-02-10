#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

double get_curr_time() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec + t.tv_usec / 1000000.0;
}

void serial_mult(int n, int *x, int *y, int *z) {
  int i, j, k;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      long temp = 0;
      for (k = 0; k < n; k++) {
        temp += x[i * n + k] * y[k * n + j];
      }
      z[i * n + j] = temp;
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    return 1;

  int n = atoi(argv[1]);
  int p = 4;

  if (argc > 2) {
    p = atoi(argv[2]);
  }

  int *a = (int *)malloc(n * n * sizeof(int));
  int *b = (int *)malloc(n * n * sizeof(int));
  int *c = (int *)malloc(n * n * sizeof(int));

  srand(time(NULL));

  int i;
  for (i = 0; i < n * n; i++) {
    a[i] = rand() % 10;
    b[i] = rand() % 10;
  }

  if (n <= 2000) {
    double start = get_curr_time();
    serial_mult(n, a, b, c);
    double end = get_curr_time();
    printf("Sequential: %f s\n", end - start);
  } else {
    printf("Sequential: Skipped (N too big)\n");
  }

  fflush(stdout);

  double start_par = get_curr_time();
  int rows = n / p;

  for (int id = 0; id < p; id++) {
    int s = id * rows;
    int e = (id + 1) * rows;
    if (id == p - 1)
      e = n;

    if (fork() == 0) {
      int r, c_idx, k;
      for (r = s; r < e; r++) {
        for (c_idx = 0; c_idx < n; c_idx++) {
          long val = 0;
          for (k = 0; k < n; k++) {
            val += a[r * n + k] * b[k * n + c_idx];
          }
          c[r * n + c_idx] = val;
        }
      }
      free(a);
      free(b);
      free(c);
      exit(0);
    }
  }

  for (i = 0; i < p; i++) {
    wait(NULL);
  }

  double end_par = get_curr_time();
  printf("Parallel:   %f s\n", end_par - start_par);

  free(a);
  free(b);
  free(c);

  return 0;
}
