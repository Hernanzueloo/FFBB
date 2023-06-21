#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include "products.h"
#include "orders.h"
#include "customers.h"
    int readQuery(int numQ, char *querr);
    SQLHSTMT initQuery(SQLHDBC dbc, char *querr);
    int endQuery(SQLHSTMT stmt);
    char*** reservar(int col);
    void paginacionyLiberacion(char***tabla,int c,int f,int fact);
    void Menu(SQLHDBC dbc);
    char *** AumentarReserva(char***ptabla, int col, int fact);
#endif

