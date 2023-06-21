#include "orders.h"
#include "funciones.h"
#define nombre "queries.sql"
#define OK 0
#define ERROR 1
#define TAM 1000
#define BufferLength 512
#define pagTAM 10
#define QueryOpenOrders 3
#define QueryRangeOrders 4
#define QueryDetailsOrders1 5
#define QueryDetailsOrders2 6
#define QueryDetailsOrders3 7




void OrdersOpenQuery(SQLHDBC dbc) {
    /*Devuelve un listado con todos los pedidos que no se hayan enviado
    (fecha no asignado a shippeddate).Se mostrará el valror del
    identificadoe ordernumber. Ordena la salida por ordernumber*/
    char y[BufferLength]="\0"; /* odbc.c */
    char *querr=NULL;
    int ret=-1, i,fact=1;
    SQLHSTMT stmt = NULL;
    char***tablao=NULL;

    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryOpenOrders, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query OrdersOpen\n\n");
        if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
        return;
    }

   /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query OrdersOpen");
        if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
        return;
    }
    
    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in OrdersOpenQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    /* Loop through the rows in the result-set */
    ret=SQLBindCol(stmt, 1, SQL_C_CHAR,(SQLCHAR *) y, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol in OrdersOpenQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    tablao=reservar(1);
    if(tablao==NULL){
        printf("Error in the allocation of the results table in OrdersOpenQuery");
            free(querr);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
        return;
    }
    i=0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(i>= fact*TAM){
            fact++;
            tablao=AumentarReserva(tablao,1,fact);
            if(tablao==NULL){
                printf("Error in the realloc");
                free(querr);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        strcpy(tablao[0][i], y);
        i++;
    }
    
    paginacionyLiberacion(tablao,1, i,fact);

    /*Cerrar stmt*/    
    (void) SQLCloseCursor(stmt);
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing stmt");
}

void OrdersRangeQuery(SQLHDBC dbc, char* fecha1, char* fecha2) {
    char ordernumber[BufferLength]="\0",buf[BufferLength]="\0", buf2[BufferLength]="\0";
    char *querr=NULL,***tablao=NULL;
    int ret=-1,i=0,fact=1;
    SQLHSTMT stmt = NULL;
    SQLLEN ordershippedInd=0;

    (void)fflush(stdout);
    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryRangeOrders, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query OrdersRange\n\n");
        return;
    }
    /*Inicilizar*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query FindStock");
        return;
    }

    ret= SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_DATE,
                            0, 0, fecha1, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindParameters1 in RangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    ret=SQLBindParameter(stmt, 2, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_DATE,
                            0, 0, fecha2, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindParameters2 in RangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in OredersRangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    
    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) ordernumber, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 1 in OredersRangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    ret=SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) buf, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in OredersRangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    ret= SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*) buf2, BufferLength, &ordershippedInd);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 3 in OredersRangeQuery");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
     tablao=reservar(3);
    if(tablao==NULL){
        printf("Error in the allocation of the results table in OrdersDetailQuery 3");
            free(querr);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
        return;
    }
    i=0;
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(i>= fact*TAM){
            fact++;
            tablao=AumentarReserva(tablao,3,fact);
            if(tablao==NULL){
                printf("Error in the realloc");
                free(querr);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        if(ordershippedInd != SQL_NULL_DATA){
            strcpy(tablao[0][i], ordernumber);
            strcpy(tablao[1][i], buf);
            strcpy(tablao[2][i], buf2); 
        }else{
            strcpy(tablao[0][i], ordernumber);
            strcpy(tablao[1][i], buf);
            strcpy(tablao[2][i], " "); 
        }
        
        i++;
    }
    (void)getchar();
    paginacionyLiberacion(tablao,3,i,fact);
    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing stmt");
    
    /*Función hacer query extraer obtiene un listado con todos los pedidos
    solicitados entre dichas fechas (orderdate). La salida debe contener 
    ordernumber, orderdate, shippeddate. Ordenada por ordernumber*/

}

void OrdersDetailQuery(SQLHDBC dbc,char * ordernumber){
    char fecha1[BufferLength]="\0",status[BufferLength]="\0", totalP[BufferLength]="\0", productcode[BufferLength]="\0",cOrdered[BufferLength]="\0",pEach[BufferLength]="\0";
    char *querr=NULL,***tablao=NULL;
    int ret=-1, i,fact=1;
    SQLHSTMT stmt = NULL;

    /*Función hacer query extraer el número de unidades de el productcode*/
    querr=(char*)calloc(TAM, sizeof(char));
    if(readQuery(QueryDetailsOrders1, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query OrdersDetail");
        return;
    }
    /*Inicilizar Query 1*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query");
        return;
    }

    ret= SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, ordernumber, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
       printf("Error in SQLBindParameter in OrdersDetailsQuery 1");
       if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
      return;
    }

    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in OredersDetailsQuery 1");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    
    ret=SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) fecha1, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 1 in OredersDetailsQuery 1");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    ret=SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) status, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in OredersDetailsQuery 1");
            if(-1==endQuery(stmt)) printf("Error freeing stmt 1");
            free(querr);
            return;
        }
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("\n%s %s\n", fecha1, status);
    }
    else{
        printf("Error executing the query OrdersDetails 1");
    }

    if(-1==endQuery(stmt)) printf("Error freeing stmt 1");

    /*Query 2*/
    if(readQuery(QueryDetailsOrders2, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query OrdeDetails 2");
        return;
    }

    /*Inicilizar Query 2*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query OrderDetails 2");
        return;
    }

    ret=SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, ordernumber, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindParameter in OrdersDetailsQuery 2");
            free(querr); /*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }

    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error executing OrdersDetailsQuery 2");
            free(querr); /*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret= SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) totalP, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol in OrdersDetailsQuery 2");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    /* Loop through the rows in the result-set */
    if (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        printf("%s\n", totalP);
    }
    else{
        printf("Error executing the query");
    }

    if(-1==endQuery(stmt)) printf("Error freeing stmt 3");

    /*Query 3*/
    if(readQuery(QueryDetailsOrders3, querr)==ERROR){ /*Control de errores*/
        printf("Error reading the query 3 OrdersDetail");
        return;
    }
    /*Inicilizar Query 3*/
    stmt=initQuery(dbc, querr);
    if(stmt==NULL){
        free(querr);
        printf("Error saving the query OrdersDetails 3");
        return;
    }

    ret= SQLBindParameter(stmt, 1, SQL_PARAM_INPUT,
                            SQL_C_CHAR, SQL_CHAR,
                            0, 0, ordernumber, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindParameter in OrdersDetailsQuery 3");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }

    ret=SQLExecute(stmt);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLExecute in OrdersDetailsQuery 3");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    
    ret=SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR*) productcode, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 1 in OrdersDetailsQuery 3");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret= SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR*) cOrdered, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 2 in OredersDetailsQuery 3");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    ret=SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR*) pEach, BufferLength, NULL);
    if(!SQL_SUCCEEDED(ret)){
            printf("Error in SQLBindCol 3 in OredersDetailsQuery 3");
            free(querr); /*Liberamos la memoria empleada*/
    
   
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
            return;
    }
    tablao=reservar(3);
    if(tablao==NULL){
        printf("Error in the allocation of the results table in OrdersDetailQuery 3");
            free(querr);/*Liberamos la memoria empleada*/
            if(-1==endQuery(stmt)) printf("Error freeing stmt");
        return;
    }
    /* Loop through the rows in the result-set */
    i=0;
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt))) {
        if(i>= fact*TAM){
            fact++;
            tablao=AumentarReserva(tablao,3,fact);
            if(tablao==NULL){
                printf("Error in the realloc");
                free(querr);/*Liberamos la memoria empleada*/
                if(-1==endQuery(stmt)) printf("Error freeing stmt");
                
                return;
            }
        }
        strcpy(tablao[0][i], productcode);
        strcpy(tablao[1][i], cOrdered);
        strcpy(tablao[2][i], pEach);
        i++;
    }
    
    paginacionyLiberacion(tablao,3,i,fact);


    /*Cerrar esto*/    
    (void) SQLCloseCursor(stmt);
    free(querr); /*Liberamos la memoria empleada*/
    
   
    if(-1==endQuery(stmt)) printf("Error freeing stmt");
    /*Función hacer query extraer obtiene un listado con todos los detalles 
    pedidos. La primera l ́ınea mostrar ́a la fecha en que se realiz ́o 
    el pedido (orderdate) as ́ı como si ya se ha enviado
    (status); la segunda l ́ınea mostrar ́a el coste total del pedido. 
    A continuaci ́on se mostrar ́a, en una l ́ınea por producto, el 
    c ́odigo del producto (productcode),la cantidad de unidades 
    solicitadas (quantityordered) y el precio por unidad (priceeach).
     La salida de las l ́ıneas donde se listen los productos se ordenar ́a
    por orderlinenumber.ordernumber, orderdate, shippeddate. Ordenada por ordernumber*/

}
