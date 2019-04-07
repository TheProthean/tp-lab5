#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlfunctions.h"

void main() {
    login_menu()
}

void login_menu() {
    ACCOUNT_TYPE account = ERROR_TYPE;
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
                break;
        }
        free(login);
        free(password);
    }
}

void makler_menu(char* makler_name) {
    printf("Possible options:\n\t1 - Add new deal\n\t2 - Show all your deals
            \n\t3 - Show all deals on date\n\t4 - Show the most needed product\n\t5 - Log out");
    int answer = -1;
    scanf("%d", &answer);
    switch (answer) {
        case 1:
        
        case 2:

        case 3:
        
        case 4:

        case 5:
            printf("Logging out...");
            break;
    }
}