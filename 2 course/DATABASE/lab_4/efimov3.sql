USE Tour

GO

SELECT 

    s.id_sale AS 'ID_продажи',
    s.number_contract AS 'Договор',

    s.date_sale AS 'Дата_продажи',
    c.birth_day AS 'Дата_рождения',


    c.full_name AS 'Клиент',
    c.phone_number AS 'Телефон',
    c.email AS 'Email',
    c.passport_data AS 'Паспорт',

    m.name_route AS 'Маршрут',
    cnt.name_country AS 'Страна',
    s.destination_country AS 'Цель',
    m.duration_days AS 'Дней',
    m.organizer AS 'Организатор',

    s.ticket_price AS 'Цена_USD',
    s.quantity_ticket AS 'Проданные билеты',
    (s.ticket_price * s.quantity_ticket) AS 'Сумма_USD',
    cnt.cost_visa AS 'Виза_USD',

    cnt.capital AS 'Столица',
    cnt.languagee AS 'Язык',
    cnt.currency AS 'Валюта',

    s.sales_manager AS 'Менеджер',
    

    CASE 
        WHEN cnt.cost_visa = 0 THEN 'Без визы'
        ELSE 'Виза требуется'
    END AS 'Виза',
    
    CASE 
        WHEN m.duration_days <= 5 THEN 'Короткий'
        WHEN m.duration_days <= 10 THEN 'Средний'
        ELSE 'Длительный'
    END AS 'Тип_тура'

FROM sale s
INNER JOIN client c ON s.id_client = c.id_client
INNER JOIN marshrut m ON s.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country

