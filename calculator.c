#include <stdio.h>

int main(void) {
    double a, b, result;
    int choice;

    printf("Basic Calculator\n");
    printf("----------------\n");

    printf("Enter first number: ");
    if (scanf("%lf", &a) != 1) {
        fprintf(stderr, "Invalid input for first number.\n");
        return 1;
    }

    printf("Enter second number: ");
    if (scanf("%lf", &b) != 1) {
        fprintf(stderr, "Invalid input for second number.\n");
        return 1;
    }

    printf("\nChoose operation:\n");
    printf(" 1. Addition (+)\n");
    printf(" 2. Subtraction (-)\n");
    printf(" 3. Multiplication (*)\n");
    printf(" 4. Division (/)\n");
    printf("Enter choice (1-4): ");
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            result = a + b;
            printf("Result: %g\n", result);
            break;
        case 2:
            result = a - b;
            printf("Result: %g\n", result);
            break;
        case 3:
            result = a * b;
            printf("Result: %g\n", result);
            break;
        case 4:
            if (b == 0.0) {
                printf("Error: Division by zero is not allowed.\n");
            } else {
                result = a / b;
                printf("Result: %g\n", result);
            }
            break;
        default:
            printf("Invalid choice. Please run the program again and select 1-4.\n");
    }

    return 0;
}
