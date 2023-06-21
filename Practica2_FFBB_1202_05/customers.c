#include "customers.h"
#include "funciones.h"
#define nombre "queries.sql"
#define OK 0
#define ERROR 1
#define TAM 1000
#define BufferLength 512
#define pagTAM 10
#define QueryFindCustomer 8
#define QueryListProductCustomer 9
#define QueryBalanceCustomer 10






void CustomersFindQuery(SQLHDBC dbc, char *customername) {
    /* odbc.c */
    char customername2[BufferLength]="\0",customerfirstname[BufferLength]="\0",customerlastname[BufferLength]="\0",customernumber[BufferLength]="\0";
    char *querry=NULL;
    int ret=-1, i,fact=1;
    SQLHSTMT stmt = NULL;
    char ***tablac = NULL;
    

    /*Función hacer query extraer el número de unidades de el productcode*/
    querry=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryFindCustomer, querry)==ERROR){ /*Control de errores*/
        printf("Error readind the query");
        free(querry);
        return;
    }
;
    
    /*Inicilizar*/
    stmt=initQuery(dbc, querry);
    if(stmt==NULL){
        free(querry);
        printf("Error saving the query CustomerFind");
        return;
    }   
    ret=SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, customername, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameter 1 in CustomersFindQuery");
        free(querry); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        return;
    } 
    ret=SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, customername, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameter in CustomersListProductsQuery");
        free(querry); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        return;
    } 
    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in CustomersFindQuery");
            free(querry); /*Liberamos la memoria empleada*/   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) customername2, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 1 in CustomersFindQuery");
            free(querry); /*Liberamos la memoria empleada*/   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret=SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) customerfirstname, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in CustomersFindQuery");
            free(querry); /*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret= SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*) customerlastname, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 3 in CustomersFindQuery");
            free(querry);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret=SQLBindCol(stmt, 4, SQL_C_CHAR, (SQLCHAR*) customernumber, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 4 in CustomersFindQuery");
            free(querry);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    tablac=reservar(4);
    if(tablac==NULL){
        printf("Error in the allocation of the results table in CustomersFindQuery");
            free(querry);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
        return;
    }
     /* Loop through the rows in the result-set */
    i=0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(i>= fact*TAM){
            fact++;
            tablac=AumentarReserva(tablac,4,fact);
            if(tablac==NULL){
                printf("Error in the realloc");
                free(querry);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        strcpy(tablac[0][i], customername2);
        strcpy(tablac[1][i], customerfirstname);
        strcpy(tablac[2][i], customerlastname);
        strcpy(tablac[3][i], customernumber);
        i++;
    }

    (void)getchar();
    paginacionyLiberacion(tablac,4, i,fact);
    
    
    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    free(querry);/*Liberamos la memoria empleada*/
    printf("\n");
   
    if(-1==endQuery(stmt)) printf("Error freeing stmt");
    
    /*Función hacer query extraer la lista de clientes de con todos 
    los clientes cuyo nombre contenga la cadena suministrada. Cada
    línea de la salida contendrña dos cadenas productcode y productname
    La salida estará ordenada segun productcode*/
}

void CustomersListProductsQuery(SQLHDBC dbc,int customernumber) {
    /* odbc.c */
    char productname[BufferLength]="\0", quantityordered[BufferLength]="\0";
    char *querr=NULL;
    int ret=-1, len,fact=1;
    SQLHSTMT stmt = NULL;
    char ***tablac = NULL;


    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryListProductCustomer, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query CustomersList");
         (void)getchar();
        return;
    }
    /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error al almacenar la query CustomersListProductsQuery");
        (void)getchar();
        return;
    }

   ret=SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_SLONG, SQL_INTEGER,
                            0, 0, &customernumber, 0, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameter in CustomersListProductsQuery");
        free(querr); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        (void)getchar();
        return;
    }

    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in CustomersListProductsQuery");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
            (void)getchar();
            return;
    }

    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) productname, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 1 in CustomersListProductsQuery");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
                        (void)getchar();
            return;
    }

    ret= SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) quantityordered, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in CustomersListProductsQuery");
            free(querr); /*Liberamos la memoria empleada*/
    
   
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        (void)getchar();
            return;
    }
    tablac=reservar(2);
    if(tablac==NULL){
        printf("Error in the allocation of the results table in CustomersListProductQuery");
            free(querr);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            (void)getchar();
        return;
    }

    /* Loop through the rows in the result-set */
    len=0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(len>= fact*TAM){
            fact++;
            tablac=AumentarReserva(tablac,2,fact);
            if(tablac==NULL){
                printf("Error in the realloc");
                free(querr);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        strcpy(tablac[0][len], productname);
        strcpy(tablac[1][len], quantityordered);
        len++;
    }

    (void)getchar();
    paginacionyLiberacion(tablac,2, len,fact);
    
    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
    /* Se solicita al usuario el nombre de contacto del cliente y se devuelve un listado con todos los clientes cuyo nombre (contactfirstname)
    o apellido (contactlastname) de contacto contenga la cadena suministrada. El listado debe incluir el nombre del cliente (customername) y el nombre
    (contactfirstname) y el apellido (contactlastname) de contacto asi como el
    identificador (customernumber) del cliente. Ordena la salida por customernumber.*/
}

void CustomersBalanceQuery(SQLHDBC dbc, int customernumber) {
    double balance=0;
    char *querr=NULL;
    int ret=-1;
    SQLHSTMT stmt = NULL;

    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryBalanceCustomer, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query CustomerBalance");
        return;
    }
    /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query CustomersBalanceQuery");
            (void)getchar();
        return;
    }
    ret=SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_SLONG, SQL_INTEGER,
                            0, 0, &customernumber, 0, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameters 1 in CustomersBalanceQuery");
        free(querr); /*Liberamos la memoria empleada*/
    
               (void)getchar();
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
            return;
    }
    ret= SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                            SQL_C_SLONG, SQL_INTEGER,
                            0, 0, &customernumber, 0, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameters 2 in CustomersBalanceQuery");
        free(querr); /*Liberamos la memoria empleada*/
              (void)getchar();  
   
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
            return;
    }
     ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
         printf("Error in SQLExecute in CustomersBalanceQuery\n");
         free(querr); /*Liberamos la memoria empleada*/
            (void)getchar();
   
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
            return;
    }
    ret=SQLBindCol(stmt, 1, SQL_C_DOUBLE, (SQLDOUBLE*) &balance, BufferLength, NULL);/*-1040950*/
    if(!SQL_SUCCEEDED(ret)){
         printf("Error in SQLBindCol 1 in CustomersBalanceQuery");
         free(querr); /*Liberamos la memoria empleada*/
    
        (void)getchar();
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        return;
    }
    /* Loop through the rows in the result-set */
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("Balance = %.2f\n\n", balance);
    }else{
        printf("No results");
    }

    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    (void)getchar();
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
    /* Se solicita el identificador de un cliente (“customer”) y se devuelve el
    saldo del mismo, esto es la suma de los pagos realizados menos el valor de la
    suma de todos los productos comprados.*/
}
