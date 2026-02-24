USE Tour

GO

DELETE FROM sale WHERE quantity_ticket = 0;

DELETE FROM marshrut 
WHERE id_route NOT IN (
    SELECT DISTINCT id_route 
    FROM sale 
    WHERE date_sale >= DATEADD(YEAR, -2, GETDATE())
);

DELETE FROM country 
WHERE id_country NOT IN (
    SELECT DISTINCT id_country 
    FROM marshrut
);

DELETE FROM sale 
WHERE date_sale > GETDATE();

DELETE FROM marshrut 
WHERE duration_days <= 0;