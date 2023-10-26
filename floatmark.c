#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <quadmath.h>

// Function to perform a simple matrix multiplication using quad precision
void matrix_multiplication(__float128* A, __float128* B, __float128* C, int n) {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            __float128 sum = 0;
            for(int k = 0; k < n; k++) {
                sum += A[i*n + k] * B[k*n + j];
            }
            C[i*n + j] = sum;
        }
    }
}

// Function to perform LU decomposition using quad precision
void lu_decomposition(__float128* A, __float128* L, __float128* U, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            __float128 sum = 0;
            for (int k = 0; k < i; k++)
                sum += (L[i*n + k] * U[k*n + j]);
            U[i*n + j] = A[i*n + j] - sum;
        }
        for (int j = i; j < n; j++) {
            if (i == j)
                L[i*n + j] = 1;
            else {
                __float128 sum = 0;
                for (int k = 0; k < i; k++)
                    sum += (L[j*n + k] * U[k*n + i]);
                L[j*n + i] = (A[j*n + i] - sum) / U[i*n + i];
            }
        }
    }
}

// Function to get matrix dimension from user
int get_matrix_dimension() {
    int n;
    printf("Enter matrix dimension (n x n): ");
    scanf("%d", &n);
    return n;
}

// Function to fill matrices with random data
void fill_matrix(__float128* matrix, int n) {
    for(int i = 0; i < n*n; i++) {
        matrix[i] = rand() / (double)RAND_MAX;
    }
}

// Function to display menu and get user choice
int get_user_choice() {
    int choice;
    printf("Choose a benchmarking task:\n");
    printf("1. Matrix Multiplication\n");
    printf("2. LU Decomposition\n");
    printf("0. Exit\n");
    printf("Choice: ");
    scanf("%d", &choice);
    return choice;
}

// Function to calculate Frobenius norm
__float128 frobenius_norm(__float128* matrix, int n) {
    __float128 sum = 0;
    for(int i = 0; i < n*n; i++) {
        sum += matrix[i] * matrix[i];
    }
    return sqrtq(sum);
}

// Function to print the benchmark report
void print_report(char* task_name, double elapsed_time, __float128 error) {
    printf("Benchmark Report:\n");
    printf("Task: %s\n", task_name);
    printf("Elapsed time: %f seconds\n", elapsed_time);
    printf("Error: %e\n", (double)error);
}

// Function to perform benchmarking for matrix multiplication
void benchmark_matrix_multiplication(int n) {
    __float128* A = (__float128*)malloc(n * n * sizeof(__float128));
    __float128* B = (__float128*)malloc(n * n * sizeof(__float128));
    __float128* C = (__float128*)malloc(n * n * sizeof(__float128));
    
    // ... Check for allocation failure ...

    fill_matrix(A, n);
    fill_matrix(B, n);

    clock_t start = clock();
    matrix_multiplication(A, B, C, n);
    clock_t end = clock();
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    __float128 error = frobenius_norm(C, n);

    print_report("Matrix Multiplication", elapsed_time, error);

    free(A);
    free(B);
    free(C);
}

// Function to perform benchmarking for LU decomposition
void benchmark_lu_decomposition(int n) {
    __float128* A = (__float128*)malloc(n * n * sizeof(__float128));
    __float128* L = (__float128*)malloc(n * n * sizeof(__float128));
    __float128* U = (__float128*)malloc(n * n * sizeof(__float128));
    
    // ... Check for allocation failure ...

    fill_matrix(A, n);

    clock_t start = clock();
    lu_decomposition(A, L, U, n);
    clock_t end = clock();
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    __float128 error = frobenius_norm(U, n);

    print_report("LU Decomposition", elapsed_time, error);

    free(A);
    free(L);
    free(U);
}

int main() {
    while (1) {
        int choice = get_user_choice();
        if (choice == 0) {
            break;
        }
        int n = get_matrix_dimension();
        switch (choice) {
            case 1:
                benchmark_matrix_multiplication(n);
                break;
            case 2:
                benchmark_lu_decomposition(n);
                break;
            default:
                fprintf(stderr, "Invalid choice\n");
        }
    }
    return 0;
}
