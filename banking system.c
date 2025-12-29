#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DATA_FILE "accounts.dat"
typedef struct {
    int acc_no;
    char name[50];
    double balance;
} Account;
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len == 0) return;
    if (s[len-1] == '\n') s[len-1] = '\0';
}
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
int next_acc_no() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 1001; // start from 1001
    Account a;
    int max = 1000;
    while (fread(&a, sizeof(Account), 1, fp) == 1) {
        if (a.acc_no > max) max = a.acc_no;
    }
    fclose(fp);
    return max + 1;
}
void create_account() {
    Account a;
    a.acc_no = next_acc_no();
    printf("Enter account holder name: ");
    if (fgets(a.name, sizeof(a.name), stdin) == NULL) return;
    trim_newline(a.name);
    if (strlen(a.name) == 0) {
        printf("Name cannot be empty.\n");
        return;
    }

    printf("Enter initial deposit amount: ");
    if (scanf("%lf", &a.balance) != 1) { clear_stdin(); printf("Invalid amount.\n"); return; }
    clear_stdin();
    if (a.balance < 0.0) {
        printf("Initial deposit cannot be negative.\n");
        return;
    }

    FILE *fp = fopen(DATA_FILE, "ab");
    if (!fp) {
        perror("Unable to open data file for appending");
        return;
    }
    fwrite(&a, sizeof(Account), 1, fp);
    fclose(fp);

    printf("Account created successfully!\nAccount Number: %d\nName: %s\nBalance: %.2f\n",
           a.acc_no, a.name, a.balance);
}
int find_account(int acc_no, Account *result, long *pos) {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) return 0;
    Account a;
    while (1) {
        long offset = ftell(fp);
        if (fread(&a, sizeof(Account), 1, fp) != 1) break;
        if (a.acc_no == acc_no) {
            if (result) *result = a;
            if (pos) *pos = offset;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}
int update_account_at_offset(long offset, const Account *a) {
    FILE *fp = fopen(DATA_FILE, "rb+");
    if (!fp) return 0;
    if (fseek(fp, offset, SEEK_SET) != 0) { fclose(fp); return 0; }
    if (fwrite(a, sizeof(Account), 1, fp) != 1) { fclose(fp); return 0; }
    fclose(fp);
    return 1;
}
void transact(int type) {
    int acc_no;
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { clear_stdin(); printf("Invalid account number.\n"); return; }
    clear_stdin();

    Account a;
    long pos;
    if (!find_account(acc_no, &a, &pos)) {
        printf("Account number %d not found.\n", acc_no);
        return;
    }

    double amt;
    if (type == 1) printf("Enter deposit amount: ");
    else printf("Enter withdrawal amount: ");
    if (scanf("%lf", &amt) != 1) { clear_stdin(); printf("Invalid amount.\n"); return; }
    clear_stdin();

    if (amt <= 0.0) {
        printf("Amount must be positive.\n");
        return;
    }

    if (type == 1) {
        a.balance += amt;
        if (!update_account_at_offset(pos, &a)) {
            printf("Failed to update account.\n");
            return;
        }
        printf("Deposit successful. New balance for account %d: %.2f\n", a.acc_no, a.balance);
    } else {
        if (amt > a.balance) {
            printf("Insufficient balance. Current balance: %.2f\n", a.balance);
            return;
        }
        a.balance -= amt;
        if (!update_account_at_offset(pos, &a)) {
            printf("Failed to update account.\n");
            return;
        }
        printf("Withdrawal successful. New balance for account %d: %.2f\n", a.acc_no, a.balance);
    }
}
void balance_enquiry() {
    int acc_no;
    printf("Enter account number: ");
    if (scanf("%d", &acc_no) != 1) { clear_stdin(); printf("Invalid account number.\n"); return; }
    clear_stdin();

    Account a;
    if (find_account(acc_no, &a, NULL)) {
        printf("Account Number: %d\nName: %s\nBalance: %.2f\n", a.acc_no, a.name, a.balance);
    } else {
        printf("Account number %d not found.\n", acc_no);
    }
}
void display_all() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (!fp) {
        printf("No accounts found.\n");
        return;
    }
    Account a;
    int count = 0;
    printf("--------------------------------------------------\n");
    printf("| %-8s | %-25s | %-10s |\n", "Acc No", "Name", "Balance");
    printf("--------------------------------------------------\n");
    while (fread(&a, sizeof(Account), 1, fp) == 1) {
        printf("| %-8d | %-25s | %-10.2f |\n", a.acc_no, a.name, a.balance);
        count++;
    }
    printf("--------------------------------------------------\n");
    if (count == 0) printf("No accounts to display.\n");
    fclose(fp);
}
void menu() {
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
        if (scanf("%d", &choice) != 1) { clear_stdin(); printf("Invalid input.\n"); continue; }
        clear_stdin();

        switch (choice) {
            case 1: create_account(); break;
            case 2: transact(1); break;
            case 3: transact(2); break;
            case 4: balance_enquiry(); break;
            case 5: display_all(); break;
            case 6: printf("Exiting. Goodbye!\n"); return;
            default: printf("Invalid choice. Please select 1-6.\n");
        }
    }
}

int main(void) {
    printf("Bank Account Management System (file: %s)\n", DATA_FILE);
    menu();
    return 0;
}
