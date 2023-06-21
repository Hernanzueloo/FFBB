#ifndef ORDERS_H
#define ORDERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

    void OrdersOpenQuery(SQLHDBC dbc);
    void OrdersRangeQuery(SQLHDBC dbc, char* fecha1, char* fecha2);
    void OrdersDetailQuery(SQLHDBC dbc,char*c);

#endif