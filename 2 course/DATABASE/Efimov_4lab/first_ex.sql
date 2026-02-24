USE Tour

GO

SELECT
id_client AS 'Индификатор клиента',
full_name AS 'ФИО',
phone_number AS 'Номер телефона',
email AS 'Электронная почта',
passport_data AS 'Паспортные данные',
birth_day AS 'Дата рождения'
FROM client;

SELECT
id_country AS 'Индификатор страны',
name_country AS 'Название страны',
cost_visa AS 'Стоимость визы',
capital AS 'Столица',
languagee AS 'Язык',
currency AS 'Валюта'
FROM country;

SELECT
id_route AS 'Индификатор маршрута',
name_route AS 'Название маршрута',
id_country AS 'Индификатор страны',
duration_days AS 'Длительность',
organizer AS 'Организатор'
FROM marshrut;

SELECT
id_sale AS 'Индификатор продаж',
id_client AS 'Индификатор клиента', 
id_route AS 'Индификатор маршрута',
number_contract AS 'Номер договора',
full_name_client AS 'ФИО Клиента',
name_route AS 'Название поездки',
destination_country AS 'Страна назначения',
ticket_price AS 'Стоимость билета',
quantity_ticket AS 'Количество билетов',
date_sale AS 'Дата продажи',
sales_manager AS 'Менеджер продаж'
FROM sale;