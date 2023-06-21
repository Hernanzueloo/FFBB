WITH numdir
     AS (SELECT employees.employeenumber
         FROM   employees
         WHERE  employees.reportsto IS NULL),
     subdir
     AS (SELECT employees.employeenumber
         FROM   employees
                JOIN numdir
                  ON employees.reportsto = numdir.employeenumber)
SELECT employees.employeenumber,
       employees.lastname
FROM   employees
       JOIN subdir
         ON employees.reportsto = subdir.employeenumber;  