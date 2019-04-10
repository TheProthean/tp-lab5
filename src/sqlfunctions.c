#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlfunctions.h"

sqlite3 *db = NULL;
char *buf;
char *err = NULL;
enum ACCOUNT_TYPE account = ERROR_TYPE;

int getAdminStatus(void *arg, int argc, char **argv, char **azColName);
int printInfoByProductType(void *arg, int argc, char **argv, char **azColName);
void updateProductsLastName(char* last_name);
int printMostSuccessfulMakler(void *arg, int argc, char **argv, char **azColName);

void initSqlDB() {
    sqlite3_open("bazaar.db", &db);
    buf = (char*)malloc(256);
}

void clearTmp() {
    err = NULL;
    account = ERROR_TYPE;
}


enum ACCOUNT_TYPE verifyLogin(char* login, char* password) {
    clearTmp();
    char *sql = (char*)malloc(128);
    printf("%s %s\n", login, password);
    sprintf(sql, "SELECT is_admin FROM _ACCOUNT login = \"%s\" AND password = \"%s\";", login, password);
    sqlite3_exec(db, sql, getAdminStatus, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    return account;
}

int getAdminStatus(void *arg, int argc, char **argv, char **azColName) {
    printf("kfkfkfk%s\n", argv[0]);
    if (!strcmp(argv[0], "false")) {
       account = MAKLER;
    }
    else {
        if (!strcmp(argv[0], "true")) {
            account = ADMIN;
        }
    }
    return 0;
}

void getMaklerDeals(char* last_name) {
    clearTmp();
    char *sql = (char*)malloc(128);
    sprintf(sql, "SELECT d.deal_id, d.transaction_date, p.name, d.count, d.wholesaler_buyer FROM _BROKER b ",
	             "JOIN _DEALS d ON d._BROKER_id = b.id ",
                 "JOIN _PRODUCT p ON d._PRODUCT_id = p.id ",
                 "WHERE b.last_name = ?;");
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, last_name, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("Deals of makler %s:\n", last_name);
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    printf("%4d", sqlite3_column_int(res, 0));
                    break;
                case 1:
                    printf("%10s", sqlite3_column_text(res, 1));
                    break;
                case 2:
                    printf("%10s", sqlite3_column_text(res, 2));
                    break;
                case 3:
                    printf("%4d", sqlite3_column_int(res, 3));
                    break;
                case 4:
                    printf("%15s", sqlite3_column_text(res, 4));
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

void getProductStats(char* product_name, char* date_from, char* date_until) {
    clearTmp();
    char *sql = (char*)malloc(128);
    sprintf(sql, "SELECT p.name, SUM(d.count), SUM(d.count) * p.cost FROM _PRODUCT p ",
                 "JOIN _DEALS d ON d._PRODUCT_id = p.id ",
                 "WHERE p.name = ? AND d.transaction_date >= ? AND d.transaction_date <= ?;");
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, product_name, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(res,  2, date_from, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(res,  3, date_until, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("Product stats %s:\n", product_name);
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    printf("%10s", sqlite3_column_text(res, 0));
                    break;
                case 1:
                    printf("%4d", sqlite3_column_int(res, 1));
                    break;
                case 2:
                    printf("%6f", sqlite3_column_double(res, 2));
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

/* MY PART */

void getListOfBuyerFirms(char* p_name) {
    clearTmp();
    char *sql = (char*)malloc(128);
    
    sprintf(sql, "SELECT p.name, d.wholesaler_buyer, SUM(d.count), SUM(d.count) * p.cost FROM _PRODUCT p ",
                 "JOIN _DEALS d ON p.id = d._PRODUCT_id ",
                 "WHERE p.name = ? ",
                 "GROUP BY p.name, d.wholesaler_buyer;");

    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, p_name, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("List of buyer firms %s:\n", p_name);
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    printf("%10s", sqlite3_column_text(res, 0));     // p.name
                    break;
                case 1:
                    printf("%10s", sqlite3_column_text(res, 1));    // d.wholesaler_buyer
                    break;
                case 2:
                    printf("%4d", sqlite3_column_int(res, 2));    // SUM(d.count)
                    break;
                case 3:
                    printf("%5d", sqlite3_column_int(res, 3));     // SUM(d.count) * p.cost
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

void getInfoByProductType() {
    clearTmp();
    char *sql = (char*)malloc(128);
    
    sprintf(sql, "SELECT p.type, d.wholesaler_buyer, SUM(d.count), SUM(d.count * p.cost) FROM _PRODUCT p ",
                 "JOIN _DEALS d ON p.id = d._PRODUCT_id ",
                 "WHERE p.type IN (SELECT p.type FROM _PRODUCT p ",
                 "JOIN _DEALS d ON p.id = d._PRODUCT_id ",
                 "GROUP BY p.type ORDER BY SUM(d.count) DESC LIMIT 1) ",
                 "GROUP BY p.type, d.wholesaler_buyer;");
    sqlite3_exec(db, sql, printInfoByProductType, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

int printInfoByProductType(void *arg, int argc, char **argv, char **azColName) {
    printf("The most popular product info:\n");
    for (int i = 0; i < argc; i += 1) {
        switch(i) {
            case 0:
                printf("%10s", argv[i]);    // p.type
                break;
            case 1:
                printf("%10s", argv[i]);    // d.wholesaler_buyer
                break;
            case 2:
                printf("%4s", argv[i]);     // SUM(d.count)
                break;
            case 3:
                printf("%5s", argv[i]);     // SUM(d.count * p.cost)
                break;
        }
    }
    return 0;
}

void getBuyersFirmInfo(char* p_wholesaler_supplier) {
    clearTmp();
    char *sql = (char*)malloc(128);
    sprintf(sql, "SELECT p.wholesale_supplier, b.last_name, SUM(d.count), SUM(d.count * p.cost) FROM _PRODUCT p ",
                 "JOIN _DEALS d ON p.id = d._PRODUCT_id ",
                 "JOIN _BROKER b ON b.id = d._BROKER_id ",
	             "WHERE p.wholesale_supplier = ? ",
                 "GROUP BY p.wholesale_supplier, b.last_name;");

    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, p_wholesaler_supplier, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("List of buyer firms %s:\n", p_wholesaler_supplier);
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    printf("%10s", sqlite3_column_text(res, 0));     // wholesaler_supplier
                    break;
                case 1:
                    printf("%10s", sqlite3_column_text(res, 1));    // b.last_name
                    break;
                case 2:
                    printf("%4d", sqlite3_column_int(res, 2));    // SUM(d.count)
                    break;
                case 3:
                    printf("%5d", sqlite3_column_int(res, 3));     // SUM(d.count) * p.cost
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

void getDealInfoByDate(char* d_date) {
    clearTmp();
    char *sql = (char*)malloc(128);
    sprintf(sql, "SELECT d.deal_id, d.transaction_date, d.count, d.wholesaler_buyer FROM _DEALS d ",
                 "WHERE d.transaction_date = ?;");

    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, d_date, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return;
    }
    printf("Deal info by date %s:\n", d_date);
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    printf("%4d", sqlite3_column_int(res, 0));     // d.id
                    break;
                case 1:
                    printf("%10s", sqlite3_column_text(res, 1));    // d.transaction_date
                    break;
                case 2:
                    printf("%4d", sqlite3_column_int(res, 2));    // d.count
                    break;
                case 3:
                    printf("%10s", sqlite3_column_text(res, 3));     // d.wholesaler_byuer
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

void getMostSuccessfulMakler() {
    clearTmp();
    char *sql = (char*)malloc(128);
    sprintf(sql, "SELECT b.last_name, b.address, b.DOB, p.wholesale_supplier FROM _BROKER b ",
                 "JOIN _DEALS d ON d._BROKER_id = b.id ",
                 "JOIN _PRODUCT p ON p.id = d._PRODUCT_id ",
                 "WHERE b.last_name = (SELECT b.last_name FROM _BROKER b ",
                 "JOIN _DEALS d ON d._BROKER_id = b.id ",
                 "GROUP BY b.last_name ",
                 "ORDER BY COUNT(d.deal_id) DESC LIMIT 1);");
    sqlite3_exec(db, sql, printMostSuccessfulMakler, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
    printf("\n");
}

int printMostSuccessfulMakler(void *arg, int argc, char **argv, char **azColName) {
    printf("The most successful makler:\n");
    for (int i = 0; i < argc; i += 1) {
        switch(i) {
            case 0:
                printf("%10s", argv[i]);    // b.last_name
                break;
            case 1:
                printf("%10s", argv[i]);    // b.address
                break;
            case 2:
                printf("%10s", argv[i]);     // b.DOB
                break;
            case 3:
                printf("%10s", argv[i]);     //p.wholesaler_supplier
                break;
        }
    }
    return 0;
}

void addNewDeal(char* last_name, char* p_name, char* transaction_date, int count, char* wholesaler_buyer) {
    clearTmp();
    char *sql = (char*)malloc(256);
    sprintf(sql, "BEGIN;",
                 "INSERT INTO _DEALS(_BROKER_id, _PRODUCT_id, transaction_date, count, wholesaler_buyer) ",
	             "VALUES ((SELECT b.id FROM _BROKER b WHERE b.last_name = %s), ",
                 "(SELECT p.id FROM _PRODUCT p WHERE p.name = %s), %s, %d, %s); ",
                 "UPDATE _STATISTICS SET count = (SELECT s1.count FROM _STATISTICS s1 ",
                 "WHERE s1._BROKER_id = (SELECT b1.id FROM _BROKER b1 ",
                 "WHERE b1.last_name = %s)) + %d, ",
                 "total_amount = (SELECT s2.total_amount FROM _STATISTICS s2 ",
                 "WHERE s2._BROKER_id = (SELECT b2.id FROM _BROKER b2 ",
                 "WHERE b2.last_name = %s)) + %d * (SELECT p1.cost FROM _PRODUCT p1 ",
                 "WHERE p1.name = %s);"
                 "COMMIT;", last_name, p_name, transaction_date, count, wholesaler_buyer, last_name, count, last_name, count, p_name);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
}

void addNewMakler(char* login, char* password, char* last_name, char* address, char* DOB) {
    clearTmp();
    char* sql = (char*)malloc(256);
    sprintf(sql, "BEGIN;",
                 "INSERT INTO _ACCOUNT(login, password, is_admin) ",
                 "VALUES(%s, %s, false); ",
                 "INSERT INTO _BROKER(last_name, address, DOB, _ACCOUNT_id) ",
                 "VALUES(%s, %s, %s, (SELECT a.id FROM _ACCOUNT a WHERE a.login = %s)); ",
                 "INSERT INTO _STATISTICS(_BROKER_id, count, total_amount) ",
                 "VALUES((SELECT b.id FROM _BROKER b WHERE b.last_name = %s), 0, 0); ",
                 "COMMIT;",
                 login, password, last_name, address, DOB, login, last_name);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
}

void deleteMakler(char* last_name) {
    clearTmp();
    char* sql = (char*)malloc(256);
    updateProductsLastName(last_name);
    sprintf(sql, "BEGIN;",
                 "DELETE FROM _STATISTICS",
                 "WHERE _BROKER_id = (SELECT id FROM _BROKER WHERE last_name = %s);",
                 "DELETE FROM _ACCOUNT WHERE id = (SELECT _ACCOUNT_id FROM _BROKER WHERE last_name = %s);",
                 "DELETE FROM _BROKER WHERE last_name = %s;",
                 "COMMIT;", last_name, last_name, last_name);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql); 
}

void updateProducts(char* date) {
    clearTmp();
    char* sql = (char*)malloc(256);
    sprintf(sql, "BEGIN;",
                 "DELETE FROM _DEALS WHERE transaction_date < %s;",
                 "COMMIT;", date);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);     
}

void updateProductsLastName(char* last_name) {
    clearTmp();
    char* sql = (char*)malloc(256);
    sprintf(sql, "BEGIN;",
                 "DELETE FROM _DEALS WHERE _BROKER_id = (SELECT id FROM _BROKER WHERE last_name = %s);",
                 "COMMIT;", last_name);
    sqlite3_exec(db, sql, NULL, NULL, &err);
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);     
}

char* getMaklerName(char *login) {
    clearTmp();
    char* sql = (char*)malloc(256);
    sprintf(sql, "SELECT b.last_name FROM _BROKER b \
                  JOIN _ACCOUNT a ON b._ACCOUNT_id = a.id \
                  WHERE a.login = ?;");
                  sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(res,  1, login, -1, SQLITE_TRANSIENT);
    } 
    else {
        fprintf(stderr, "Failed to execute query: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    if (sqlite3_step(res) == SQLITE_ROW) {
        int columnCnt = sqlite3_column_count(res);
        for (int i = 0; i < columnCnt; i += 1) {
            switch(i) {
                case 0:
                    return sqlite3_column_text(res, 0);     // d.id
                    break;
            }
        }
    }
    if (err != NULL) {
        fprintf(stderr, "%s\n", err);
    }
    free(sql);
}