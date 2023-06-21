#include "products.h"
#include "funciones.h"
#define nombre "queries.sql"
#define OK 0
#define ERROR 1
#define TAM 1000
#define BufferLength 512
#define pagTAM 10
#define QueryStockProducts 1
#define QueryFindProducts 2



void ProductsStockQuery(SQLHDBC dbc,char * productcode) {
    char cantidad[BufferLength]="\0"; /* odbc.c */
    char *querr=NULL;
    int ret=-1;
    SQLHSTMT stmt = NULL;

    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryStockProducts, querr)==ERROR){ /*Control de errores*/
        printf("Query FindProduct could not be read\n\n");
        return ;
    }
    
    /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query Stock");
        return;
    }
    ret= SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, productcode, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameter in ProductStockQuery");
        if(-1==endQuery(stmt)) printf("Error freeing stmt");
        free(querr);
        return;
    }

    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLExecute in ProductStockQuery");
        if(-1==endQuery(stmt)) printf("Error freeing stmt");
        free(querr);
        return;
    }
    
    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) cantidad, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindCol in ProductStockQuery");
        if(-1==endQuery(stmt)) printf("Error freeing stmt");
        free(querr);
        return;
    }
    /* Loop through the rows in the result-set */
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        
        printf("%s\n\n", cantidad);
    }
    else{
        printf("\nNo results\n");
    }
        
        
    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    (void)getchar();
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing stmt");
}

void ProductsFindQuery(SQLHDBC dbc, char*c) {
    char productos[BufferLength]="\0", productosN[BufferLength]="\0"; /* odbc.c */
    char *querr=NULL,***tablap=NULL;
    int ret=-1,i=0,fact=1;
    SQLHSTMT stmt = NULL;

    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryFindProducts, querr)==ERROR){ /*Control de errores*/
        printf("Query FindProduct could not be read\n\n");
        return;
    }
    /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query FindStock");
        return;
    }
    ret=SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, c, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindParameter in ProductsFindQuery");
        free(querr); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) printf("Error freeing handle stmt");
        return;
    } 
    ret= SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLExecute in ProductFindQuery");
        free(querr); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) 
            printf("Error freeing stmt");
        return;
        }
    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) productos, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
        printf("Error in SQLBindCol 1 in ProductFindQuery");
        free(querr); /*Liberamos la memoria empleada*/
        if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
        }
    ret=SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) productosN, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in ProductStockQuery");
            free(querr); /*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
        }
    tablap=reservar(2);
    if(tablap==NULL){
        printf("Error in the allocation of the results table in ProductFindQuery");
            free(querr);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
        return;
    }
    /* Loop through the rows in the result-set */
    i=0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(i>= fact*TAM){
            fact++;
            tablap=AumentarReserva(tablap,2,fact);
            if(tablap==NULL){
                printf("Error in the realloc");
                free(querr);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        strcpy(tablap[0][i], productos);
        strcpy(tablap[1][i], productosN);
        i++;
    }
    if(i==0){
        printf("\nNo results\n");
    }

    (void)getchar();
    paginacionyLiberacion(tablap,2,i,fact);

    /*Cerrar esto*/
    (void) SQLCloseCursor(stmt);
    free(querr); /*Liberamos la memoria empleada*/
   if(-1==endQuery(stmt)) printf("Error freeing stmt");
}
