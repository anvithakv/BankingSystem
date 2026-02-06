
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Account {
    int acc_no;
    char name[100];
    float balance;
};

void create_account();
void view_account();
void deposit_money();
void withdraw_money();
void delete_account();
void menu();

int main() {
    menu();
    return 0;
}

void menu() {
    int choice;
    while(1) {
        printf("\n--- BANK MANAGEMENT SYSTEM ---\n");
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: create_account(); break;
            case 2: view_account(); break;
            case 3: deposit_money(); break;
            case 4: withdraw_money(); break;
            case 5: delete_account(); break;
            case 6: exit(0);
            default: printf("Invalid choice. Try again.\n");
        }
    }
}

void create_account() {
    FILE *fp;
    struct Account acc;

    fp = fopen("bank.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%d", &acc.acc_no);
    printf("Enter name: ");
    getchar(); // clear newline
    fgets(acc.name, 100, stdin);
    acc.name[strcspn(acc.name, "\n")] = 0;
    printf("Enter initial balance: ");
    scanf("%f", &acc.balance);

    fwrite(&acc, sizeof(acc), 1, fp);
    fclose(fp);
    printf("Account created successfully!\n");
}

void view_account() {
    FILE *fp;
    struct Account acc;
    int acc_no, found = 0;

    fp = fopen("bank.dat", "rb");
    if (fp == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter account number to view: ");
    scanf("%d", &acc_no);

    while(fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("Account Number: %d\n", acc.acc_no);
            printf("Name: %s\n", acc.name);
            printf("Balance: %.2f\n", acc.balance);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("Account not found.\n");
    }
}

void deposit_money() {
    FILE *fp, *temp;
    struct Account acc;
    int acc_no, found = 0;
    float amount;

    fp = fopen("bank.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter account number to deposit: ");
    scanf("%d", &acc_no);

    while(fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("Enter amount to deposit: ");
            scanf("%f", &amount);
            acc.balance += amount;
            found = 1;
        }
        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        printf("Amount deposited successfully.\n");
    else
        printf("Account not found.\n");
}

void withdraw_money() {
    FILE *fp, *temp;
    struct Account acc;
    int acc_no, found = 0;
    float amount;

    fp = fopen("bank.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter account number to withdraw: ");
    scanf("%d", &acc_no);

    while(fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            printf("Enter amount to withdraw: ");
            scanf("%f", &amount);
            if (acc.balance >= amount) {
                acc.balance -= amount;
                printf("Withdrawal successful.\n");
            } else {
                printf("Insufficient balance.\n");
            }
            found = 1;
        }
        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (!found)
        printf("Account not found.\n");
}

void delete_account() {
    FILE *fp, *temp;
    struct Account acc;
    int acc_no, found = 0;

    fp = fopen("bank.dat", "rb");
    temp = fopen("temp.dat", "wb");

    if (fp == NULL || temp == NULL) {
        printf("File error.\n");
        return;
    }

    printf("Enter account number to delete: ");
    scanf("%d", &acc_no);

    while(fread(&acc, sizeof(acc), 1, fp)) {
        if (acc.acc_no == acc_no) {
            found = 1;
            continue; 
        }
        fwrite(&acc, sizeof(acc), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        printf("Account deleted successfully.\n");
    else
        printf("Account not found.\n");
}