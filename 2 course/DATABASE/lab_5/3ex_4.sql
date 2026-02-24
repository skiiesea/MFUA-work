USE Tour
GO

CREATE VIEW vw_customer_travel_profile
AS
SELECT 
    c.full_name AS 'ФИО клиента',
    c.phone_number AS 'Телефон',
    c.email AS 'Email',
    COUNT(DISTINCT s.id_sale) AS 'Количество поездок',
    SUM(s.quantity_ticket) AS 'Всего билетов',
    ROUND(AVG(s.ticket_price * s.quantity_ticket), 2) AS 'Средний бюджет',
    SUM(s.ticket_price * s.quantity_ticket) AS 'Общий бюджет',
    MAX(s.date_sale) AS 'Последняя поездка',
    'Отдых' AS 'Тип путешествий'
FROM client c
INNER JOIN sale s ON c.id_client = s.id_client
GROUP BY c.full_name, c.phone_number, c.email;
GO
