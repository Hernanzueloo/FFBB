WITH ordernum
     AS (SELECT orderdetails. ordernumber
         FROM   orderdetails
                JOIN products
                  ON products.productcode = orderdetails.productcode
         WHERE  products.productname = '1940 Ford Pickup Truck'),
     custnum
     AS (SELECT orders.customernumber
         FROM   orders
                INNER JOIN ordernum
                        ON orders.ordernumber = ordernum.ordernumber)
SELECT customers.customernumber,
       customers.customername,
       Sum(payments.amount)
FROM   customers
       JOIN custnum
         ON customers.customernumber = custnum.customernumber
       JOIN payments
         ON payments.customernumber = custnum.customernumber
GROUP  BY customers.customernumber
ORDER  BY sum DESC;  