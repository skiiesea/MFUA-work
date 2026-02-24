USE Tour

GO

SELECT
	s.id_client AS 'Индификатор продаж',
	s.number_contract AS 'Номер договора',
	s.date_sale AS 'Дата продажи',
	c.birth_day AS 'Дата рождения',
	c.full_name AS 'ФИО',
    c.phone_number AS 'Номер телефона',
    c.email AS 'Электронная почта',
    c.passport_data AS 'Паспортные данные',
    m.name_route AS 'Название маршрута',
    cnt.name_country AS 'Название страны',
    s.destination_country AS 'Цель',
    m.duration_days AS 'Длительность',
    m.organizer AS 'Организатор',
    s.ticket_price AS 'Цена',
    s.quantity_ticket AS 'Проданные билеты',
    (s.ticket_price * s.quantity_ticket) AS 'Сумма',
    cnt.cost_visa AS 'Цена визы',
    cnt.capital AS 'Столица',
    cnt.languagee AS 'Язык',
    cnt.currency AS 'Валюта',
    s.sales_manager AS 'Менеджер'

FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country;

