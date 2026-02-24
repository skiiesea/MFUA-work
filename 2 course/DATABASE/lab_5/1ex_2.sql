USE Tour
GO

CREATE VIEW vw_client
	AS SELECT c.full_name as 'ФИО клиента', c.phone_number as 'Номер телефона', c.email as 'Почта', c.passport_data as 'Паспортные данные', c.birth_day as 'День рождения' FROM client c
	
	GO 

CREATE VIEW vw_country
	AS SELECT cnt.name_country as 'Название страны', cnt.cost_visa as 'Стоимость визы', cnt.capital as 'Столица', cnt.languagee as 'Язык', cnt.currency as 'Валюта' FROM country cnt

	GO

CREATE VIEW vw_marshrut
	AS SELECT m.name_route as 'Название маршрута', m.duration_days as 'Длительность(дней)', m.organizer as 'Организатор' FROM marshrut m

	go

CREATE VIEW vw_sale
	AS SELECT s.number_contract as 'Номер договора', s.full_name_client as 'ФИО клиента', s.name_route as 'Название маршрута', s.destination_country as 'Страна назначения'
			, s.ticket_price as 'Стоимость билета', s.quantity_ticket as 'Количество билетов', s.date_sale as 'Дата продажи', s.sales_manager as 'Менеджер продажи' FROM sale s
	
	GO

