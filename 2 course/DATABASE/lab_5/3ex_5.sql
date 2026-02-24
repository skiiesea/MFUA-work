USE Tour
GO

CREATE TABLE hotel_partners (
    id_hotel INT IDENTITY(1,1) PRIMARY KEY,
    hotel_name VARCHAR(100) NOT NULL,
    id_route INT NOT NULL FOREIGN KEY REFERENCES marshrut(id_route),
    city VARCHAR(50) NOT NULL,
    room_price DECIMAL(10,2) NOT NULL,
    rating DECIMAL(2,1) DEFAULT 4.0,
    total_bookings INT DEFAULT 0,
    cancelled_bookings INT DEFAULT 0
);
GO

INSERT INTO hotel_partners (hotel_name, id_route, city, room_price, rating, total_bookings, cancelled_bookings)
VALUES 
('Париж Отель', 1, 'Париж', 150.00, 4.5, 120, 5),
('Лувр Гранд', 1, 'Париж', 200.00, 4.8, 85, 3),
('Рим Палас', 2, 'Рим', 130.00, 4.3, 95, 7),
('Колизей Отель', 2, 'Рим', 110.00, 4.0, 110, 12),
('Москва Сити', 3, 'Москва', 90.00, 4.2, 150, 8),
('Киев Отель', 4, 'Киев', 70.00, 3.9, 80, 5),
('Нью Йорк Плаза', 5, 'Нью-Йорк', 250.00, 4.7, 200, 15),
('Дубай Палас', 6, 'Дубай', 300.00, 4.9, 180, 4),
('Мехико Гранд', 7, 'Мехико', 95.00, 4.1, 70, 6),
('Токио Отель', 8, 'Токио', 180.00, 4.6, 130, 9);
GO

CREATE VIEw vw_hotel_partners_analysis
AS
SELECt
	h.hotel_name AS 'Отель',
    m.name_route AS 'Маршрут',
    cnt.name_country AS 'Страна',
    h.city AS 'Город',
    h.total_bookings AS 'Количество бронирований',
    h.cancelled_bookings AS 'Отменённые бронирования',
    ROUND((h.cancelled_bookings * 100.0 / NULLIF(h.total_bookings, 0)), 1) AS 'Доля отмен (%)',
    h.room_price AS 'Средняя стоимость номера',
    h.rating AS 'Рейтинг',
    (h.total_bookings - h.cancelled_bookings) AS 'Успешные бронирования'
FROM hotel_partners h
INNER JOIN marshrut m ON h.id_route = m.id_route
INNER JOIN country cnt ON m.id_country = cnt.id_country;
GO
