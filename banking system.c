#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100   // maximum number of accounts

typedef struct {
    int acc_no;
    char name[50];
    double balance;
} Account;

Account accounts[MAX];
int count = 0;
int next_acc_no = 1001; // auto-increment starting number

void create_account() {
    if (count >= MAX) {
        printf("Cannot create more accounts.\n");
        return;
    }
    Account a;
    a.acc_no = next_acc_no++;
    printf("Enter account holder name: ");
    getchar(); // clear newline
    fgets(a.name, sizeof(a.name), stdin);
    a.name[strcspn(a.name, "\n")] = '\0';

    printf("Enter initial deposit amount: ");
    scanf("%lf", &a.balance);

    accounts[count++] = a;
    printf("Account created successfully!\nAccount Number: %d\nName: %s\nBalance: %.2f\n",
           a.acc_no, a.name, a.balance);
}

int find_account(int acc_no) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].acc_no == acc_no) return i;
    }
    return -1;
}

void deposit() {
    int acc_no;
    double amt;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    printf("Enter deposit amount: ");
    scanf("%lf", &amt);
    if (amt <= 0) {
        printf("Invalid amount.\n");
        return;
    }
    accounts[idx].balance += amt;
    printf("Deposit successful. New balance: %.2f\n", accounts[idx].balance);
}

void withdraw() {
    int acc_no;
    double amt;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    printf("Enter withdrawal amount: ");
    scanf("%lf", &amt);
    if (amt <= 0 || amt > accounts[idx].balance) {
        printf("Invalid or insufficient balance.\n");
        return;
    }
    accounts[idx].balance -= amt;
    printf("Withdrawal successful. New balance: %.2f\n", accounts[idx].balance);
}

void balance_enquiry() {
    int acc_no;
    printf("Enter account number: ");
    scanf("%d", &acc_no);
    int idx = find_account(acc_no);
    if (idx == -1) {
        printf("Account not found.\n");
        return;
    }
    printf("Account Number: %d\nName: %s\nBalance: %.2f\n",
           accounts[idx].acc_no, accounts[idx].name, accounts[idx].balance);
}

void display_all() {
    if (count == 0) {
        printf("No accounts to display.\n");
        return;
    }
    printf("--------------------------------------------------\n");
    printf("| %-8s | %-20s | %-10s |\n", "Acc No", "Name", "Balance");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-8d | %-20s | %-10.2f |\n",
               accounts[i].acc_no, accounts[i].name, accounts[i].balance);
    }
    printf("--------------------------------------------------\n");
}

int main() {
    int choice;
    while (1) {
        printf("\n=== Bank Account Management System ===\n");
        printf("1. Create Account\n");
        printf("2. Deposit\n");
        printf("3. Withdraw\n");
        printf("4. Balance Enquiry\n");
        printf("5. Display All Accounts\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: deposit(); break;
            case 3: withdraw(); break;
            case 4: balance_enquiry(); break;
            case 5: display_all(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
