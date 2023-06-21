/*Archivo con las consultas ya escritas para evitar copiarlas tal cual en la función:*/
/*EN el submenu Products Consulta STOCK */
SELECT products.quantityinstock FROM products WHERE products.productcode=?
/*EN el submenu Products Consulta FIND */
SELECT products.productcode, products.productname from products where products.productname like ?
/*EN el submenu Orders Consulta OPEN */
SELECT orders.ordernumber FROM orders WHERE orders.shippeddate IS NULL
/*EN el submenu Orders Consulta RANGE */
SELECT orders.ordernumber, orders.orderdate, orders.shippeddate FROM orders WHERE ? < orders.orderdate and  orders.orderdate < ?
/*EN el submenu Orders Consulta DETAILS */
SELECT orders.orderdate,orders.status FROM orders WHERE orders.ordernumber= ?
/*Segunda Query de Details*/
SELECT SUM(orderdetails.priceeach*orderdetails.quantityordered) FROM orderdetails WHERE orderdetails.ordernumber= ?
/*Tercera Query de Details*/
SELECT orderdetails.productcode, orderdetails.quantityordered, orderdetails.priceeach FROM orderdetails WHERE orderdetails.ordernumber = ?
/*EN el submenu Customers Consulta FIND */
SELECT customers.customername, customers.contactfirstname, customers.contactlastname, customers.customernumber FROM customers WHERE (customers.contactfirstname like ? OR customers.contactlastname like ?)
/*EN el submenu Customers Consulta LIST PRODUCTS */
WITH codProductos AS (SELECT orderdetails.productcode, orderdetails.quantityordered FROM orderdetails WHERE orderdetails.ordernumber IN (SELECT orders.ordernumber FROM orders where orders.customernumber=?))SELECT products.productname, SUM(codProductos.quantityordered) FROM codProductos, products WHERE products.productcode = codProductos.productcode GROUP BY products.productcode ORDER BY products.productcode
/*EN el submenu Customers Consulta BALANCE */
WITH pagado AS (SELECT SUM(orderdetails.priceeach*orderdetails.quantityordered) AS suma FROM orderdetails WHERE orderdetails.ordernumber IN (SELECT orders.ordernumber FROM orders WHERE orders.customernumber= ? )), comprado AS (SELECT SUM(payments.amount) AS suma FROM payments WHERE payments.customernumber = ? ) SELECT comprado.suma-pagado.suma from comprado, pagado
