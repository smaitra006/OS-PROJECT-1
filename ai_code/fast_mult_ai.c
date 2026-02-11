#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <time.h>
#include <pwd.h>

// CORRECTED: Helper to get Total RAM in GB
float get_total_ram() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) return 0;
    
    char label[32];
    long total_mem_kb;
    // Safely parse the label and the value
    if (fscanf(fp, "%s %ld", label, &total_mem_kb) != 2) {
        fclose(fp);
        return 0;
    }
    fclose(fp);
    
    // FIX: Using 1024.0 * 1024.0 forces correct floating point math
    // KB -> MB -> GB
    return (float)total_mem_kb / (1024.0 * 1024.0);
}

void populate_matrix(double *mat, int n) {
    for (int i = 0; i < n * n; i++) mat[i] = (double)rand() / RAND_MAX;
}

void transpose(double *src, double *dst, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dst[j * n + i] = src[i * n + j];
}

void multiply_sequential(double *A, double *B, double *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
}

int main(int argc, char *argv[]) {
    char *user = getpwuid(getuid())->pw_name;
    int cores = sysconf(_SC_NPROCESSORS_ONLN);
    float total_ram = get_total_ram();
    
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    
    printf("\n--- OS Project: Resource-Aware FASTmult ---\n");
    printf("System: %s | Cores: %d | RAM: %.2f GB\n", user, cores, total_ram);
    printf("Matrix Size: %d x %d\n", n, n);

    size_t size = n * n * sizeof(double);
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    double *C_par = (double *)shmat(shmid, NULL, 0);
    double *C_seq = (double *)malloc(size);
    double *A = (double *)malloc(size);
    double *B = (double *)malloc(size);
    double *B_T = (double *)malloc(size); 

    srand(time(NULL));
    populate_matrix(A, n);
    populate_matrix(B, n);
    transpose(B, B_T, n);

    struct timespec start, end;

    printf("Executing Sequential version...\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_sequential(A, B, C_seq, n);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_seq = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Executing Parallel version (%d child processes)...\n", cores);
    clock_gettime(CLOCK_MONOTONIC, &start);
    int rows_per_proc = n / cores;
    for (int i = 0; i < cores; i++) {
        if (fork() == 0) {
            int start_r = i * rows_per_proc;
            int end_r = (i == cores - 1) ? n : (i + 1) * rows_per_proc;
            for (int r = start_r; r < end_r; r++) {
                for (int c = 0; c < n; c++) {
                    double sum = 0;
                    for (int k = 0; k < n; k++) {
                        sum += A[r * n + k] * B_T[c * n + k];
                    }
                    C_par[r * n + c] = sum;
                }
            }
            shmdt(C_par);
            exit(0);
        }
    }
    for (int i = 0; i < cores; i++) wait(NULL);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_par = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    FILE *fp = fopen("benchmarks.txt", "a");
    if (fp) {
        fprintf(fp, "------------------------------------------------\n");
        fprintf(fp, "USER: %-10s | RAM: %.2f GB | CORES: %d\n", user, total_ram, cores);
        fprintf(fp, "MATRIX SIZE: %d x %d\n", n, n);
        fprintf(fp, "SEQUENTIAL TIME: %.4f seconds\n", time_seq);
        fprintf(fp, "PARALLEL TIME  : %.4f seconds\n", time_par);
        fprintf(fp, "SPEEDUP        : %.2fx faster\n", time_seq / time_par);
        fprintf(fp, "------------------------------------------------\n\n");
        fclose(fp);
        printf("Results logged to benchmarks.txt\n");
    }

    shmctl(shmid, IPC_RMID, NULL);
    free(A); free(B); free(B_T); free(C_seq);
    return 0;
}