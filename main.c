#include <stdio.h>
#include <stdlib.h>

typedef struct Account {
  char name[50];
  int accountNumber;
  float balance;
} Account;


void menuSelection(int *menuOption){
    printf("Account Management System\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    printf("\n");
    scanf("%d", menuOption);
}

void createAccount(account*){
    printf("Enter your name: ");
    fgets(account_obj.name, 50, stdin);
    printf("\n");
    printf("Enter your account number: ");
    scanf("%d", accountNumber);
    printf("\n");
    printf("Enter your initial deposit: ");
    scanf("%f", balance);
    printf("\n");
    retun account;
}

int main(){
    int numberOfAccounts = 1;
    int menuOption = 1;
    int accountInt = 0;
    Account *accounts = (Account *) malloc(numberOfAccounts * sizeof(Account));  
    while (menuOption != 0){
        menuSelection(&menuOption);
        if (menuOption == 1){ 
            createAccount()
        }   
        accountInt++;
    }
    return 0;
}