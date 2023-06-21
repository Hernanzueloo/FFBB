#ifndef PRODUCTS_H
#define PRODUCTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
    void ProductsStockQuery(SQLHDBC dbc,char * productcode);
    void ProductsFindQuery(SQLHDBC dbc,char*c);
#endif