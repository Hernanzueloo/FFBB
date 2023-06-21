#include "funciones.h"
#define nombre "queries.sql"
#define OK 0
#define ERROR 1
#define TAM 1000
#define pagTAM 10
#define BufferLength 512

static int ShowMainMenu();
static void ShowProductsMenu(SQLHDBC dbc);
static void ShowOrdersMenu(SQLHDBC dbc);
static void ShowCustomersMenu(SQLHDBC dbc);

static int ShowProductsSubMenu();
static int ShowOrdersSubMenu();
static int ShowCustomersSubMenu();

static void ProductsStock(SQLHDBC dbc);
static void ProductsFind(SQLHDBC dbc);



static void OrdersOpen(SQLHDBC dbc);
static void OrdersRange(SQLHDBC dbc);
static void OrdersDetail(SQLHDBC dbc);

static void CustomersFind(SQLHDBC dbc);
static void CustomersListProducts(SQLHDBC dbc);
static void CustomersBalance(SQLHDBC dbc);
static void freeTabla(char***tabla, int col,int fact);

void paginacionyLiberacion(char***ptabla,int c, int f,int fact){
    int i, j, pag=0;
    char ***tabla=NULL;
    char letra[BufferLength];

    tabla=ptabla;
    printf("\nThere are %d results\n\n", f);
    if(f<=pagTAM){ /*Si hay pagTAM filas o menos*/
        for(i=0; i<f; i++){
            for(j=0; j<c; j++){
                printf("%s ", tabla[j][i]);
            }
            printf("\n");
            }
        printf("\n");
        freeTabla(ptabla,c,fact);
        return;
    }
    else{ /*Si hay mas de pagTAM filas*/
        do{
            for(i=pag; (i<f) && (i<pag+pagTAM); i++){
                for(j=0; j<c; j++){
                    printf("%s ", tabla[j][i]);
                }
                printf("\n");
            }
            
            printf("\n                                                                  [<] Move to the previous page\n");
            printf("\n                                                                  [>] Move to the next page\n");
            printf("\n                                                                  [x] Exit\n\n");
            printf("\n                                                                  Page %d out of %d\n", pag/pagTAM+1, (f-1)/pagTAM+1);
            printf("Introduce a character: ");


            
            if(!fgets(letra, BufferLength, stdin)) return;
            
            while ((letra[0]!='<')&&(letra[0]!='>')&&(letra[0]!='x')){
                printf("\nWrong character");
                printf("\n                                                                  [<] Move to the previous page\n");
                printf("\n                                                                  [>] Move to the next page\n");
                printf("\n                                                                  [x] Exit\n\n");
                printf("\n                                                                  Page %d out of %d\n", pag/pagTAM+1, (f-1)/pagTAM+1);
                printf("Introduce a character: ");
                if(!fgets(letra, BufferLength, stdin)) return;
            }
            if(letra[0]=='>'){
                pag+=pagTAM;
                if(pag>=f) pag=0;
            }
            else if(letra[0]=='<'){
                pag-=pagTAM;
                if(pag<0) pag=f-f%pagTAM;
                if(pag==f) pag=f-10;
            }
            
        }while(letra[0]!='x');
    }
    freeTabla(ptabla,c,fact);
}
static void freeTabla(char***ptabla, int col, int fact){
    int i,j;
    char ***tabla=NULL;
    tabla=ptabla;
    if(!tabla || col<=0){
        return;
    }
    for(i=0;i<col;i++){
        for(j=0;j<TAM*fact;j++){
            free(tabla[i][j]);
        }
        free(tabla[i]);
    }
    free(tabla);
}
char *** AumentarReserva(char***ptabla, int col, int fact){
    char ***tabla=ptabla;
    int i,j;
    if(ptabla==NULL || col<0 || fact<=0){
        return NULL;
    }
    for(i=0;i<col;i++){
        tabla[i]=(char**)realloc(tabla[i],fact*TAM*sizeof(char*));
        if(tabla[i]==NULL){
            for(i--;i>=0;i--){
                free(tabla[i]);
            }
            free(tabla);
            return NULL;
        }
        
        for(j=TAM*(fact-1);j<fact*TAM;j++){
            tabla[i][j]=(char*)calloc(BufferLength,sizeof(char));
            if(tabla[i][j]==NULL){
                for(j--;j>=0;j--){
                    free(tabla[i][j]);
                 }
                 for(i--;i>=0;i--){
                     for(j=0;j<TAM;j++){
                        free(tabla[i][j]);
                    }
                    free(tabla[i]);
                 }
                free(tabla);
                return NULL;
            }
        }
    }
    return tabla;
    
}
char*** reservar(int col){
    char ***tabla;
    int i,j;
    if(col<=0) return NULL;
    tabla=(char***)calloc(col,sizeof(char**));
    if(tabla==NULL)
        return NULL;
    for(i=0;i<col;i++){
        tabla[i]=(char**)calloc(TAM,sizeof(char*));
        if(tabla[i]==NULL){
            for(i--;i>=0;i--){
                free(tabla[i]);
            }
            free(tabla);
            return NULL;
        }
        
        for(j=0;j<TAM;j++){
            tabla[i][j]=(char*)calloc(BufferLength,sizeof(char));
            if(tabla[i][j]==NULL){
                for(j--;j>=0;j--){
                    free(tabla[i][j]);
                 }
                 for(i--;i>=0;i--){
                     for(j=0;j<TAM;j++){
                        free(tabla[i][j]);
                    }
                    free(tabla[i]);
                 }
                free(tabla);
                return NULL;
            }
        }
    }
    return tabla;
}

int readQuery(int numQ, char *querr){
    int i;
    FILE *f=NULL;

    if(numQ<1||!querr){
        printf("Error al leer la consulta");
        return ERROR;
    }

    f=fopen(nombre, "r");

    if(!f){ /*Control de errores*/
        printf("Error");
        return ERROR;
    }

    for(i=0; i<(1+2*numQ); i++) (void)fgets(querr, TAM, f); /*Almacena en query la query deseada*/

   (void) fclose(f);

    return OK;
}
SQLHSTMT initQuery(SQLHDBC dbc, char *querr){
    SQLHSTMT stmt = NULL;
    SQLINTEGER ret; /* odbc.c */
    /*Inicilizar*/
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt); /*Guarda memoria para almacenar la consulta en stmt*/
    if (!SQL_SUCCEEDED(ret)) { /*Control de errores*/
       odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return NULL;
    }
    ret = SQLPrepare(stmt, (SQLCHAR*) querr, SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) { /*Control de errores*/
       odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return NULL;
    }
    return stmt;
}

int endQuery(SQLHSTMT stmt){
    SQLRETURN ret2; /* ODBC API return status */

    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2)) { /*Control de Errores*/
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return -1;
    }
    return 0;
}

void Menu(SQLHDBC dbc) {   
    int nChoice=0;
    
    do {
        nChoice = ShowMainMenu(dbc);
        switch (nChoice) {
            case 1: {
                ShowProductsMenu(dbc);
            }
                break;

            case 2: {
                ShowOrdersMenu(dbc);
            }
                break;

            case 3: {
                ShowCustomersMenu(dbc);
            }
                break;

            case 4: {
                printf("Bye, see you soon\n\n");
                return;
            }
        }
    } while (1==1);

   
}

int ShowMainMenu() { /*Muestra las distintas opciones del Menu y devuelve el valor introducido por el usuario + control de errores*/
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
               " (4) Exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

void ShowProductsMenu(SQLHDBC dbc) {
    int nChoice = 0;
    do {
        nChoice = ShowProductsSubMenu(dbc);
        switch (nChoice) {

            case 1: {
                ProductsStock(dbc);
            }
                break;

            case 2: {
                ProductsFind(dbc);
            }
                break;
        }
    } while (nChoice != 3);

}

 int ShowProductsSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 3)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 3));

    return nSelected;
}


void ShowOrdersMenu(SQLHDBC dbc) {
    int nChoice = 0;
    do {
        nChoice = ShowOrdersSubMenu(dbc);
        switch (nChoice) {

            case 1: {
                OrdersOpen(dbc);
            }
                break;

            case 2: {
                OrdersRange(dbc);
            }
                break;

            case 3: {
                OrdersDetail(dbc);
            }
                break;
        }
    } while (nChoice != 4);

}

 int ShowOrdersSubMenu() {
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");


        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

void ShowCustomersMenu(SQLHDBC dbc){
    int nChoice = 0;

    do {
        nChoice = ShowCustomersSubMenu(dbc);
        switch (nChoice) {
            case 1: {
                CustomersFind(dbc);
            }
                break;

            case 2: {
                CustomersListProducts(dbc);
            }
                break;

            case 3: {
                CustomersBalance(dbc);
            }
                break;
        }
    } while (nChoice != 4);

}

int ShowCustomersSubMenu(){
    int nSelected = 0;
    char buf[16];

    do {

        printf(" (1) Find \n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n\n");

        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, 16, stdin))
            /* reading input failed, give up: */
            nSelected =0;
        else
            /* have some input, convert it to integer: */
            nSelected = atoi(buf);
        printf("\n");

        if ((nSelected < 1) || (nSelected > 4)) {
            printf("You have entered an invalid choice. Please try again\n\n\n");
        }
    } while ((nSelected < 1) || (nSelected > 4));

    return nSelected;
}

void ProductsStock(SQLHDBC dbc){
    char productcode[BufferLength];

    printf("Enter productcode > ");
    if(scanf("%s", productcode) < 1){
        printf("Productname could not be read\n\n");
        return;
    }
    ProductsStockQuery(dbc,productcode);
}
void ProductsFind(SQLHDBC dbc){
    char productname[BufferLength+2]="%";
    int len;
    printf("Enter productname > ");

    if (1!=scanf("%s",productname+1))
            printf("Error reading customername") ;
    len=(int)strlen(productname);
    if(productname[len-1]=='\n')
        productname[len-1]='\0';
    strcat(productname,"%");
    ProductsFindQuery(dbc,productname);
    
}
void OrdersOpen(SQLHDBC dbc){
    OrdersOpenQuery(dbc);
}
void OrdersRange(SQLHDBC dbc){
    char date1[BufferLength],date2[BufferLength];
    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");

    if (2!=scanf("%s - %s",date1,date2))
            printf("Error en la lectura") ; 

    OrdersRangeQuery(dbc,date1,date2);
}
void OrdersDetail(SQLHDBC dbc){
    char ordernumber[BufferLength];
    printf("Enter ordernumber > ");

    if (!fgets(ordernumber, BufferLength, stdin))
            printf("Error reading ordernumber\n\n") ; 
    OrdersDetailQuery(dbc,ordernumber);
}
void CustomersFind(SQLHDBC dbc){
    char customername[BufferLength+2]="%";
    int len;
    printf("Enter customer name > ");

    if (1!=scanf("%s",customername+1))
            printf("Error reading customername") ;
    len=(int)strlen(customername);
    if(customername[len-1]=='\n')
        customername[len-1]='\0';
    strcat(customername,"%");
    CustomersFindQuery(dbc,customername);
}
void CustomersListProducts(SQLHDBC dbc){
    int customernumber;
    printf("Enter customer number > ");

    if (1!=scanf("%d",&customernumber))
            printf("Error en la lectura ") ; 
    CustomersListProductsQuery(dbc,customernumber);
}
void CustomersBalance(SQLHDBC dbc){
    int customernumber;
    printf("Enter customer number > ");

    if (1!=scanf("%d",&customernumber))
            printf("Error en la lectura") ;
    CustomersBalanceQuery(dbc,customernumber);
}
