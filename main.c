/*
 * Copyright (c) 2023 Daniel J. Thompson.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 or later.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Account {
char name[50];
int accountNumber;
float balance;
} Account;

void menuSelection(int *optionSelect){
    printf("Account Management System\n");
    printf("1. Create Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Check Balance\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", optionSelect);
    printf("\n");
}

void accountCheck(int *accountNumber, Account *account, int *numberOfAccounts, int *accountPosition, bool *doesAccountExist){
    for (int i = 0; i < (*numberOfAccounts); i++){
        if(account[i].accountNumber == (*accountNumber)){
            *accountPosition = i;
            *doesAccountExist = true;
            break;
        }
    }
}

void createAccount(Account *account, int *currentAccount){
    printf("Enter your name: ");
    scanf("%50s", account[(*currentAccount)].name);
    printf("\n");
    printf("Enter your account number: ");
    scanf("%d", &account[(*currentAccount)].accountNumber);
    printf("\n");
    printf("Enter your initial deposit: ");
    scanf("%f", &account[(*currentAccount)].balance);
    printf("\n");
}

void depositFunds(Account *account, bool *doesAccountExist, int *accountNumber, int *numberOfAccounts){
    printf("Enter your account number: ");
    scanf("%d", accountNumber);
    printf("\n");
    int accountPosition;
    float depositAmount;
    accountCheck(accountNumber, account, numberOfAccounts, &accountPosition, doesAccountExist);
    if ((*doesAccountExist) == true){
        printf("Enter the amount you want to deposit: ");
        scanf("%f", &depositAmount);
        printf("\n");
        account[accountPosition].balance += depositAmount;
        printf("Your account balance is now: $%.2f\n", account[accountPosition].balance);
        doesAccountExist = false;
    } else{
        printf("Account number %d not found\n", *accountNumber);
    }
    printf("\n");
}

void withdrawFunds(Account *account, bool *doesAccountExist, int *accountNumber, int *numberOfAccounts){
    printf("Enter your account number: ");
    scanf("%d", accountNumber);
    printf("\n");
    int accountPosition;
    float widthdrawAmount;
    accountCheck(accountNumber, account, numberOfAccounts, &accountPosition, doesAccountExist);
    if ((*doesAccountExist) == true){
        printf("Enter the amount you want to withdraw: ");
        scanf("%f", &widthdrawAmount);
        printf("\n");
        account[accountPosition].balance -= widthdrawAmount;
        printf("Your account balance is now: $%.2f\n", account[accountPosition].balance);
        *doesAccountExist = false;
    } else{
        printf("Account number %d not found\n", *accountNumber);
    }
    printf("\n");
}


void checkBalance(Account *account, bool *doesAccountExist, int *accountNumber, int *numberOfAccounts){
    printf("Enter your account number: ");
    scanf("%d", accountNumber);
    printf("\n");
    int accountPosition;
    accountCheck(accountNumber, account, numberOfAccounts, &accountPosition, doesAccountExist);
    if ((*doesAccountExist) == true){
        printf("Your account balance is $%.2f\n", account[accountPosition].balance);
        *doesAccountExist = false;
    } else{
        printf("Account number %d not found\n", *accountNumber);
    }
    printf("\n");
}

const int accountExpandAmount = 3;

int main(){
    int numOfAccounts = 3;
    Account *account = (Account *) malloc(numOfAccounts * sizeof(Account));
    if(account == NULL){
        fprintf(stderr, "Unable to allocate memory. Now terminating the process\n");
        exit(0);
    }
    int optionSelect = 1;
    int currentAccount = 0;
    int accountNumberTmp = 0;
    bool doesAccountExist = false;
    while(optionSelect != 0){
        menuSelection(&optionSelect);
        accountNumberTmp = 0;
        if(currentAccount == numOfAccounts){
            numOfAccounts += accountExpandAmount;
            account = (Account *) realloc(account, numOfAccounts * sizeof(Account));
            if(account == NULL){
                fprintf(stderr, "Unable to allocate additional memory. Now terminating the process\n");
                exit(0);
            }
        }
        if(optionSelect == 1){
            createAccount(account, &currentAccount);
        } else if(optionSelect == 2){
            depositFunds(account, &doesAccountExist, &accountNumberTmp, &numOfAccounts);
        } else if (optionSelect == 3){
            withdrawFunds(account, &doesAccountExist, &accountNumberTmp, &numOfAccounts);
        } else if (optionSelect == 4){
            checkBalance(account, &doesAccountExist, &accountNumberTmp, &numOfAccounts);
        }
        currentAccount++;
    }
    free(account);
    printf("Goodbye!\n");
    return 0;
}