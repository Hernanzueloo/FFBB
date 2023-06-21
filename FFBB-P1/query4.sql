 WITH objpercust AS
(
         SELECT   orders.customernumber,
                  Sum(orderdetails.quantityordered) AS qpercust
         FROM     orders
         JOIN     orderdetails
         ON       orders.ordernumber=orderdetails.ordernumber
         GROUP BY customernumber ), objperempnum AS
(
         SELECT   customers.salesrepemployeenumber,
                  Sum(qpercust) AS qpere
         FROM     customers
         JOIN     objpercust
         ON       customers.customernumber=objpercust.customernumber
         GROUP BY customers.salesrepemployeenumber )
SELECT   employees.officecode ,
         Sum(qpere) AS qperof
FROM     employees
JOIN     objperempnum
ON       employees.employeenumber=objperempnum.salesrepemployeenumber
GROUP BY officecode
ORDER BY qperof DESC limit 1; 

/* WITH qwerty AS ( WITH quancn AS
(
       SELECT orderdetails.quantityordered,
              orders.customernumber
       FROM   orderdetails,
              orders
       WHERE  orderdetails.ordernumber=orders.ordernumber )
SELECT   customers.salesrepemployeenumber,
         Sum(quancn.quantityordered)
FROM     customers,
         quancn
WHERE    quancn.customernumber = customers.customernumber
GROUP BY customers.salesrepemployeenumber )
SELECT   employees.officecode,
         Sum (qwerty.sum) AS s
FROM     employees
JOIN     qwerty
ON       qwerty.salesrepemployeenumber=employees.employeenumber
GROUP BY employees.officecode
ORDER BY s DESC limit 1; */