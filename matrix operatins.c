/*
 * matrix_operations.c
 *
 * Implements:
 *  - Matrix Addition
 *  - Matrix Multiplication
 *  - Transpose
 *
 * Uses functions and dynamic 2D arrays (contiguous allocation) for modularity.
 *
 * Compile:
 *   gcc -std=c11 -O2 -o matrix_operations matrix_operations.c
 *
 * Example:
 *   ./matrix_operations
 */

#include <stdio.h>
#include <stdlib.h>

int *allocate_matrix(int r, int c) {
    int *m = malloc(sizeof(int) * r * c);
    if (!m) {
        fprintf(stderr, "Allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return m;
}

void free_matrix(int *m) {
    free(m);
}

void read_matrix(int *m, int r, int c) {
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j) {
            printf("Element [%d][%d]: ", i, j);
            if (scanf("%d", &m[i * c + j]) != 1) {
                fprintf(stderr, "Invalid input. Exiting.\n");
                exit(EXIT_FAILURE);
            }
        }
}

void print_matrix(const int *m, int r, int c) {
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j)
            printf("%6d", m[i * c + j]);
        putchar('\n');
    }
}

void add_matrices(const int *A, const int *B, int *C, int r, int c) {
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j)
            C[i * c + j] = A[i * c + j] + B[i * c + j];
}

int multiply_matrices(const int *A, int r1, int c1, const int *B, int r2, int c2, int *C) {
    if (c1 != r2) return 0; // incompatible
    // initialize result
    for (int i = 0; i < r1 * c2; ++i) C[i] = 0;
    for (int i = 0; i < r1; ++i) {
        for (int k = 0; k < c1; ++k) {
            int a = A[i * c1 + k];
            for (int j = 0; j < c2; ++j) {
                C[i * c2 + j] += a * B[k * c2 + j];
            }
        }
    }
    return 1;
}

void transpose_matrix(const int *A, int r, int c, int *T) {
    // T will be of size c x r
    for (int i = 0; i < r; ++i)
        for (int j = 0; j < c; ++j)
            T[j * r + i] = A[i * c + j];
}

void flush_stdin(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) ;
}

int main(void) {
    int choice;
    while (1) {
        printf("\nMatrix Operations\n");
        printf("1. Matrix Addition\n");
        printf("2. Matrix Multiplication\n");
        printf("3. Transpose\n");
        printf("4. Exit\n");
        printf("Choose an option (1-4): ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Invalid input. Exiting.\n");
            return EXIT_FAILURE;
        }

        if (choice == 1) {
            int r, c;
            printf("Matrix Addition selected.\n");
            printf("Enter rows and columns: ");
            if (scanf("%d %d", &r, &c) != 2 || r <= 0 || c <= 0) {
                fprintf(stderr, "Invalid dimensions.\n");
                continue;
            }
            int *A = allocate_matrix(r, c);
            int *B = allocate_matrix(r, c);
            int *C = allocate_matrix(r, c);

            printf("Enter elements of matrix A:\n");
            read_matrix(A, r, c);
            printf("Enter elements of matrix B:\n");
            read_matrix(B, r, c);

            add_matrices(A, B, C, r, c);

            printf("Result (A + B):\n");
            print_matrix(C, r, c);

            free_matrix(A);
            free_matrix(B);
            free_matrix(C);
        } else if (choice == 2) {
            int r1, c1, r2, c2;
            printf("Matrix Multiplication selected.\n");
            printf("Enter rows and columns of matrix A: ");
            if (scanf("%d %d", &r1, &c1) != 2 || r1 <= 0 || c1 <= 0) {
                fprintf(stderr, "Invalid dimensions.\n");
                continue;
            }
            printf("Enter rows and columns of matrix B: ");
            if (scanf("%d %d", &r2, &c2) != 2 || r2 <= 0 || c2 <= 0) {
                fprintf(stderr, "Invalid dimensions.\n");
                continue;
            }
            int *A = allocate_matrix(r1, c1);
            int *B = allocate_matrix(r2, c2);

            printf("Enter elements of matrix A:\n");
            read_matrix(A, r1, c1);
            printf("Enter elements of matrix B:\n");
            read_matrix(B, r2, c2);

            if (c1 != r2) {
                printf("Incompatible dimensions for multiplication (A.columns must equal B.rows).\n");
            } else {
                int *C = allocate_matrix(r1, c2);
                multiply_matrices(A, r1, c1, B, r2, c2, C);
                printf("Result (A x B):\n");
                print_matrix(C, r1, c2);
                free_matrix(C);
            }
            free_matrix(A);
            free_matrix(B);
        } else if (choice == 3) {
            int r, c;
            printf("Transpose selected.\n");
            printf("Enter rows and columns: ");
            if (scanf("%d %d", &r, &c) != 2 || r <= 0 || c <= 0) {
                fprintf(stderr, "Invalid dimensions.\n");
                continue;
            }
            int *A = allocate_matrix(r, c);
            printf("Enter elements of matrix:\n");
            read_matrix(A, r, c);

            int *T = allocate_matrix(c, r);
            transpose_matrix(A, r, c, T);

            printf("Original matrix:\n");
            print_matrix(A, r, c);
            printf("Transpose (T):\n");
            print_matrix(T, c, r);

            free_matrix(A);
            free_matrix(T);
        } else if (choice == 4) {
            printf("Exiting.\n");
            break;
        } else {
            printf("Invalid option. Please choose 1-4.\n");
        }

        flush_stdin();
    }

    return 0;
}
