USE Tour
GO

SELECT 
    id_country AS 'Индификатор страны',
    name_country AS 'Название страны',
    cost_visa AS 'Стоимость визы',
    capital AS 'Столица',
    languagee AS 'Язык',
    currency AS 'Валюта'
FROM country;


SELECT 
    m.id_route AS 'Индификатор маршрута',
    m.name_route AS 'Название маршрута',
    c.name_country AS 'Страна назначения',
    m.duration_days AS 'Длительность',
    m.organizer AS 'Организатор'
FROM marshrut m
INNER JOIN country c ON m.id_country = c.id_country;

SELECT 
    id_client AS 'Индификатор клиента',
    full_name AS 'Полное имя',
    phone_number AS 'Номер телефона',
    email AS 'Электронная почта',
    passport_data AS 'Паспортные данные',
    birth_day AS 'Дата рождения'
FROM client;

SELECT 
    s.id_sale AS 'Индификатор продажи',
    s.number_contract AS 'Номер договора',
    c.full_name AS 'Клиент',
    m.name_route AS 'Маршрут',
    s.destination_country AS 'Цель',
    s.ticket_price AS 'Цена билета',
    s.quantity_ticket AS 'Количество билетов',
    s.ticket_price * s.quantity_ticket AS 'Общая стоимость',
    s.date_sale AS 'Дата продажи',
    s.sales_manager AS 'Менеджер по продажам',
    cnt.name_country AS 'Страна маршрута',
    cnt.cost_visa AS 'Стоимость визы',
    m.duration_days AS 'Длительность тура',
    m.organizer AS 'Организатор тура'
FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country;
