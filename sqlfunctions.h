enum ACCOUNT_TYPE {
    ERROR_TYPE = 0,
    MAKLER = 1,
    ADMIN = 2
};

void initSqlDB();

ACCOUNT_TYPE verifyLogin(char* login, char* password);

void getMaklerDeals(char* last_name); //

void getProductStats(char* product_name, char* date_from, char* date_until); //

void getListOfBuyerFirms(char* p_name); //

void getInfoByProductType(); //

void getBuyersFirmInfo(char* p_wholesaler_supplier);

void getDealInfoByDate(char* d_date); //

void getMostSuccessfulMakler(); //

void addNewDeal(char* last_name, char* p_name, char* transaction_date, int count, char* wholesaler_buyer); //

void addNewMakler(char* login, char* password, char* last_name, char* address, char* DOB); //

void deleteMakler(char* last_name);

void updateProducts(char* date);

int getMaklerName(char *login);