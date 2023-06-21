WITH fechin2003
     AS (SELECT orders.customernumber
         FROM   orders
         WHERE  orders.orderdate <= '2003-12-31'
                AND orders.orderdate >= '2003-01-01'),
     empsalein2003
     AS (SELECT customers.salesrepemployeenumber
         FROM   customers
                JOIN fechin2003
                  ON customers.customernumber = fechin2003.customernumber),
     ofcnotin2003
     AS ((SELECT employees.officecode
          FROM   employees)
         EXCEPT
         (SELECT employees.officecode
          FROM   employees
                 JOIN empsalein2003
                   ON
         employees.employeenumber = empsalein2003.salesrepemployeenumber))
SELECT offices.country,
       Count(*)
FROM   offices
       JOIN ofcnotin2003
         ON offices.officecode = ofcnotin2003.officecode
GROUP  BY offices.country; 

