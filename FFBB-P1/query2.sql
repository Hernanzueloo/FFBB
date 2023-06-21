WITH orderandtime
     AS (SELECT orders.ordernumber,
                orders.shippeddate - orders.orderdate AS DiffDates
         FROM   orders where orders.status='Shipped')
SELECT products.productline,
       Avg(orderandtime.diffdates) AS PrCodAvg
FROM   orderdetails
       JOIN orderandtime
         ON orderdetails.ordernumber = orderandtime.ordernumber
       JOIN products
         ON orderdetails.productcode = products.productcode
GROUP  BY products.productline;  

/*WITH fechaslinea
     AS (SELECT orders.shippeddate,
                orders.orderdate,
                products.productline
         FROM   products,
                orderdetails,
                orders
         WHERE  products.productcode = orderdetails.productcode
                AND orderdetails.ordernumber = orders.ordernumber
         ORDER  BY products.productline DESC)
SELECT fechaslinea.productline,
       Avg(fechaslinea.shippeddate - fechaslinea.orderdate)
FROM   fechaslinea
GROUP  BY fechaslinea.productline;  
    */
