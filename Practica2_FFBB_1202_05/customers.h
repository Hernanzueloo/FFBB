#ifndef CUSTOMERS_H
#define CUSTOMERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

    void CustomersFindQuery(SQLHDBC dbc,char*c);
    void CustomersListProductsQuery(SQLHDBC dbc,int c);
    void CustomersBalanceQuery(SQLHDBC dbc,int a);
#endif