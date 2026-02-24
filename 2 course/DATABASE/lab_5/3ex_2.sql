USE Tour
GO

CREATE VIEW vw_route_popularity
AS
SELECT 
    m.name_route AS 'Маршрут',
    cnt.name_country AS 'Страна',
    COUNT(s.id_sale) AS 'Количество продаж',
    SUM(s.quantity_ticket) AS 'Продано путёвок',
    SUM(s.ticket_price * s.quantity_ticket) AS 'Выручка',
    ROUND(AVG(s.ticket_price), 2) AS 'Средняя цена'
FROM marshrut m
INNER JOIN country cnt ON m.id_country = cnt.id_country
LEFT JOIN sale s ON m.id_route = s.id_route
GROUP BY m.name_route, cnt.name_country;
GO
