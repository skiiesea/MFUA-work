USE Tour
GO

CREATE VIEW vw_tour AS 
	SELECT s.number_contract AS 'Номер договора',
		   s.date_sale AS 'Дата продажи',
		   c.full_name AS 'ФИО клиента',
		   c.phone_number AS 'Номер телефона', 
		   c.email AS 'Почта',
		   c.birth_day AS 'День рождения',
		   m.name_route AS 'Название тура',
		   m.organizer AS 'Туроператор',
		   cnt.name_country AS 'Страна путешествия',
           cnt.capital AS 'Столица',
           cnt.languagee AS 'Государственный язык',
           cnt.currency AS 'Валюта',
           cnt.cost_visa AS 'Стоимость визы',
		   s.destination_country AS 'Цель поездки',
           s.ticket_price AS 'Цена одной путевки',
           s.quantity_ticket AS 'Количество путевок',
           s.sales_manager AS 'Менеджер по продажам'
FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country;
GO
