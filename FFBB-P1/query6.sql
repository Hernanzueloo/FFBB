with resultado AS
(
         SELECT   o1.productcode,
                  o2.productcode,
                  count(DISTINCT o1.ordernumber) AS cont
         FROM     orderdetails                   AS o1
         JOIN     orderdetails                   AS o2
         ON       o1.ordernumber = o2.ordernumber
         WHERE    o1.productcode < o2.productcode
         GROUP BY o1.productcode,
                  o2.productcode)
SELECT   *
FROM     resultado
WHERE    cont >1
ORDER BY cont;


/*WITH parejas
     AS (WITH tablaaux
              AS (SELECT orderdetails.ordernumber,
                         orderdetails.productcode
                  FROM   orderdetails)
         SELECT orderdetails.ordernumber,
                orderdetails.productcode AS pc1,
                tablaaux.productcode     AS pc2
          FROM   tablaaux,
                 orderdetails
          WHERE  tablaaux.ordernumber = orderdetails.ordernumber
                 AND (tablaaux.productcode > orderdetails.productcode)),
     parejasaux
     AS (SELECT *
         FROM   parejas)
SELECT parejas.pc1,
       parejas.pc2,
       parejas.ordernumber,
       parejasaux.ordernumber
FROM   parejas,
       parejasaux
WHERE  parejas.pc1 = parejasaux.S
       AND parejas.pc2 = parejasaux.pc2
       AND parejas.ordernumber != parejasaux.ordernumber; */ 