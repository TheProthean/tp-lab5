#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlfunctions.h"

void login_menu();
void makler_menu(char* makler_name);
void ui_getDealInfoByDate();
void ui_addNewDeal(char* makler_name);
void admin_menu();
void ui_addNewMakler();
void ui_getProductStats();

void main() {
    initSqlDB();
    login_menu();
}

void login_menu() {
    enum ACCOUNT_TYPE account = ERROR_TYPE;
    while (account == ERROR_TYPE) {
        char* login = (char*)malloc(64);
        char* password = (char*)malloc(64);
        printf("Login: ");
        scanf("%s", login);
        printf("Password: ");
        scanf("%s", password);
        account = verifyLogin(login, password);
        switch (account) {
            case ERROR_TYPE:
                printf("Wrong login or password. Try again.\n");
                break;
            case MAKLER:
                makler_menu(getMaklerName(login));
                break;
            case ADMIN:
                admin_menu();
                break;
        }
        free(login);
        free(password);
    }
}

void makler_menu(char* makler_name) {
    while (1) {
    printf("Possible options:\n\t1 - Add new deal\n\t2 - Show all your deals \
            \n\t3 - Show all deals on date\n\t4 - Show the most needed product type\n\t5 - Log out\n");
    int answer = -1;
    scanf("%d", &answer);
    switch (answer) {
        case 1:
            ui_addNewDeal(makler_name);
            break;
        case 2:
            getMaklerDeals(makler_name);
            break;
        case 3:
            ui_getDealInfoByDate();
            break;
        case 4:
            getInfoByProductType();
            break;
        case 5:
            printf("Logging out...");
            return;
    }
    }
}

void ui_getDealInfoByDate() {
    printf("Enter date: ");
    char *date = (char*)malloc(16);
    scanf("%s", date);
    getDealInfoByDate(date);
    free(date);
}

void ui_addNewDeal(char* makler_name) {
    printf("Enter product name: ");
    char *product = (char*)malloc(16);
    scanf("%s", product);

    printf("Enter date: ");
    char *date = (char*)malloc(16);
    scanf("%s", date);
    
    printf("Enter count: ");
    int count = 0;
    scanf("%d", &count);
    
    printf("Enter buyer: ");
    char *buyer = (char*)malloc(16);
    scanf("%s", buyer);
    
    addNewDeal(makler_name, product, date, count, buyer);
    free(product);
    free(date);
    free(buyer);
}

void admin_menu() {
    while (1) {
    printf("Possible options:\n\t1 - Add new makler\n\t2 - Show all deals of makler \
            \n\t3 - Show all deals on date\n\t4 - Show the most needed product type \
            \n\t5 - Show most successful makler\n\t6 - Show product stats by period \
            \n\t7 - Show list of buyer on product name\n\t8 - update product info by deals until date \
            \n\t9 - delete makler\n\t10 - Get info on supplier\n\t11 - Log out\n");
    int answer = -1;
    scanf("%d", &answer);
    switch (answer) {
        case 1:
            ui_addNewMakler();
            break;
        case 2:
            printf("Enter makler last name: ");
            char *makler_name = (char*)malloc(16);
            scanf("%s", makler_name);
            getMaklerDeals(makler_name);
            free(makler_name);
            break;
        case 3:
            ui_getDealInfoByDate();
            break;
        case 4:
            getInfoByProductType();
            break;
        case 5:
            getMostSuccessfulMakler();
            break;
        case 6:
            ui_getProductStats();
            break;
        case 7:
            printf("Enter product name: ");
            char *buyer = (char*)malloc(16);
            scanf("%s", buyer);
            getListOfBuyerFirms(buyer);
            free(buyer);
            break;
        case 8:
            printf("Enter date: ");
            char *date = (char*)malloc(16);
            scanf("%s", date);
            updateProducts(date);
            free(date);
            break;
        case 9:
            printf("Enter last name: ");
            char *last_name = (char*)malloc(16);
            scanf("%s", last_name);
            deleteMakler(last_name);
            free(last_name);
            break;
        case 10:
            printf("Enter supplier: ");
            char *sup_name = (char*)malloc(16);
            scanf("%s", sup_name);
            getBuyersFirmInfo(sup_name);
            free(sup_name);
            break;
        case 11:
            printf("Logging out...");
            return;
    }
    }
}

void ui_addNewMakler() {
    printf("Enter login: ");
    char *login = (char*)malloc(16);
    scanf("%s", login);

    printf("Enter password: ");
    char *password = (char*)malloc(16);
    scanf("%s", password);
    
    printf("Enter last name: ");
    char *last_name = (char*)malloc(16);
    scanf("%s", last_name);
    
    printf("Enter address: ");
    char *address = (char*)malloc(32);
    scanf("%s", address);

    printf("Enter date of birth: ");
    char *date = (char*)malloc(16);
    scanf("%s", date);
    
    addNewMakler(login, password, last_name, address, date);
    free(login);
    free(password);
    free(last_name);
    free(address);
    free(date);
}

void ui_getProductStats() {
    printf("Enter product: ");
    char *product = (char*)malloc(16);
    scanf("%s", product);

    printf("Enter starting date: ");
    char *date_s = (char*)malloc(16);
    scanf("%s", date_s);
    
    printf("Enter ending date: ");
    char *date_e = (char*)malloc(16);
    scanf("%s", date_e);

    getProductStats(product, date_s, date_e);
    free(product);
    free(date_s);
    free(date_e);
}