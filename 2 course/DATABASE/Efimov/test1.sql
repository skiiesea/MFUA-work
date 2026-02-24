USE Tour

GO

UPDATE country SET cost_visa = cost_visa + 500;

UPDATE sale 
SET ticket_price = ticket_price * 1.1 
WHERE name_route = 'Париж и замки Луары';


UPDATE marshrut 
SET duration_days = 10 
WHERE name_route = 'Рим и Неаполь';


UPDATE sale 
SET destination_country = 'Обучение' 
WHERE name_route = 'Токио и Киото';

UPDATE sale 
SET date_sale = '2024-06-15' 
WHERE name_route = 'Стамбул и Каппадокия';