USE Tour
GO


SELECT * FROM (
    SELECT 
        ROW_NUMBER() OVER (ORDER BY name_country) AS '№',
        name_country AS 'Страна',
        capital AS 'Столица',
        cost_visa AS 'Визовый сбор',
        languagee AS 'Язык',
        currency AS 'Валюта'
    FROM country
) AS t
ORDER BY [№];


SELECT * FROM (
    SELECT 
        ROW_NUMBER() OVER (ORDER BY name_route) AS '№',
        name_route AS 'Название тура',
        duration_days AS 'Дней',
        organizer AS 'Организатор'
    FROM marshrut
) AS t
ORDER BY [№];

SELECT * FROM (
    SELECT 
        ROW_NUMBER() OVER (ORDER BY full_name) AS '№',
        full_name AS 'Клиент',
        phone_number AS 'Телефон',
        email AS 'Email',
        FORMAT(birth_day, 'dd.MM.yyyy') AS 'Дата рождения'
    FROM client
) AS t
ORDER BY [№];

SELECT * FROM (
    SELECT 
        ROW_NUMBER() OVER (ORDER BY date_sale DESC) AS '№',
        number_contract AS 'Договор',
        FORMAT(date_sale, 'dd.MM.yyyy') AS 'Дата',
        full_name_client AS 'Клиент',
        name_route AS 'Тур',
        quantity_ticket AS 'Кол-во',
        FORMAT(ticket_price, 'N2') AS 'Цена',
        sales_manager AS 'Менеджер'
    FROM sale
) AS t
ORDER BY [№];