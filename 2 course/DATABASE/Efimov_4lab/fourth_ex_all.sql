-- 1 задание
USE Tour
GO

SELECT 
    s.number_contract AS 'Номер договора',
    s.date_sale AS 'Дата продажи',
    c.full_name AS 'Клиент',
    m.name_route AS 'Маршрут',
    m.duration_days AS 'Длительность (дни)',
    s.ticket_price AS 'Цена билета ($)',
    s.quantity_ticket AS 'Количество билетов',
    (s.ticket_price * s.quantity_ticket) AS 'Общая стоимость ($)',
    s.sales_manager AS 'Менеджер'
FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country
WHERE cnt.name_country = 'Франция'

-- 2 задание
SELECT 
    s.id_sale AS 'ID продажи',
    s.number_contract AS 'Номер договора',
    s.date_sale AS 'Дата продажи',
    c.full_name AS 'Клиент',
    m.name_route AS 'Маршрут',
    cnt.name_country AS 'Страна',
    s.ticket_price AS 'Цена билета ($)',
    s.quantity_ticket AS 'Количество билетов',
    (s.ticket_price * s.quantity_ticket) AS 'Общая стоимость ($)',
    s.sales_manager AS 'Менеджер'
FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country
WHERE YEAR(s.date_sale) = 2024
ORDER BY s.date_sale DESC;

-- 3 задание
SELECT name_country, cost_visa
FROM country
ORDER BY cost_visa ASC;

-- 4 задание
SELECT SUM(quantity_ticket) AS 'Количество билетов'
FROM sale;

-- 6 задание
SELECT name_route AS 'Название маршрута' , AVG(ticket_price) AS 'Средняя цена'
FROM sale
GROUP BY name_route;

USE Tour
GO

USE Tour
GO

-- 7 задание
SELECT 
    c.id_country AS 'ID страны',
    c.name_country AS 'Страна',
    c.capital AS 'Столица',
    c.cost_visa AS 'Стоимость визы ($)',
    c.languagee AS 'Язык',
    c.currency AS 'Валюта',
    COUNT(m.id_route) AS 'Количество маршрутов'
FROM country c
LEFT JOIN marshrut m ON c.id_country = m.id_country
LEFT JOIN sale s ON m.id_route = s.id_route AND YEAR(s.date_sale) = 2024
WHERE s.id_sale IS NULL
GROUP BY c.id_country, c.name_country, c.capital, c.cost_visa, c.languagee, c.currency
ORDER BY c.name_country;

-- 8 задание
WITH CountryAvgPrice AS (
    SELECT 
        m.id_country,
        AVG(s.ticket_price) AS avg_country_price
    FROM sale s
    INNER JOIN marshrut m ON s.id_route = m.id_route
    GROUP BY m.id_country
)
SELECT 
    m.name_route AS 'Маршрут',
    cnt.name_country AS 'Страна',
    s.ticket_price AS 'Цена путевки ($)',
    cap.avg_country_price AS 'Средняя цена по стране ($)',
    ROUND(s.ticket_price - cap.avg_country_price, 2) AS 'Разница с средней ($)',
    ROUND((s.ticket_price / cap.avg_country_price - 1) * 100, 2) AS 'Процент выше средней (%)',
    m.duration_days AS 'Длительность (дни)',
    m.organizer AS 'Организатор'
FROM sale s
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country
INNER JOIN CountryAvgPrice cap ON m.id_country = cap.id_country
WHERE s.ticket_price > cap.avg_country_price
ORDER BY (s.ticket_price - cap.avg_country_price) DESC;

-- 9 задание
SELECT 
    c.id_client AS 'ID клиента',
    c.full_name AS 'ФИО клиента',
    c.phone_number AS 'Телефон',
    c.email AS 'Электронная почта',
    s.date_sale AS 'Дата покупки',
    FORMAT(s.date_sale, 'dd.MM.yyyy') AS 'Дата продажи',
    s.number_contract AS 'Номер договора',
    CONCAT('$', s.ticket_price) AS 'Цена путевки',
    s.quantity_ticket AS 'Количество путевок',
    CONCAT('$', (s.ticket_price * s.quantity_ticket)) AS 'Общая сумма',
    s.sales_manager AS 'Менеджер',
    DATEDIFF(YEAR, c.birth_day, GETDATE()) AS 'Возраст клиента'
FROM client c
INNER JOIN sale s ON c.id_client = s.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
WHERE m.name_route = 'Рим и Неаполь'
ORDER BY s.date_sale DESC, c.full_name;

-- 10 задание
SELECT 
    c.id_country AS 'Код страны',
    c.name_country AS 'Страна',
    c.capital AS 'Столица',
    c.cost_visa AS 'Стоимость визы ($)',
    COUNT(m.id_route) AS 'Количество маршрутов',
    MIN(m.duration_days) AS 'Минимальная длительность',
    MAX(m.duration_days) AS 'Максимальная длительность',
    AVG(m.duration_days) AS 'Средняя длительность'
FROM country c
INNER JOIN marshrut m ON c.id_country = m.id_country
GROUP BY c.id_country, c.name_country, c.capital, c.cost_visa
HAVING MIN(m.duration_days) > 7
ORDER BY c.name_country;