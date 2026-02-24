USE Tour
GO

ALTER TABLE marshrut
ADD 
    summer_surcharge DECIMAL(5,2) DEFAULT 0,
    winter_surcharge DECIMAL(5,2) DEFAULT 0,
    spring_surcharge DECIMAL(5,2) DEFAULT 0,
    autumn_surcharge DECIMAL(5,2) DEFAULT 0;
GO


UPDATE marshrut SET summer_surcharge = 30, winter_surcharge = 20 WHERE id_route = 1;
UPDATE marshrut SET summer_surcharge = 25, winter_surcharge = 10 WHERE id_route = 2;
UPDATE marshrut SET summer_surcharge = 20, winter_surcharge = 15 WHERE id_route = 3;
UPDATE marshrut SET summer_surcharge = 35, winter_surcharge = 15 WHERE id_route = 5;
UPDATE marshrut SET summer_surcharge = 15, winter_surcharge = 25 WHERE id_route = 6;
UPDATE marshrut SET summer_surcharge = 40, winter_surcharge = 15 WHERE id_route = 8;
UPDATE marshrut SET summer_surcharge = 30, winter_surcharge = 20 WHERE id_route = 13;
UPDATE marshrut SET summer_surcharge = 25, winter_surcharge = 30 WHERE id_route = 23;
UPDATE marshrut SET summer_surcharge = 20, winter_surcharge = 20 WHERE id_route = 25;

UPDATE marshrut SET 
    summer_surcharge = 15,
    winter_surcharge = 10,
    spring_surcharge = 10,
    autumn_surcharge = 10
WHERE summer_surcharge = 0 AND winter_surcharge = 0;
GO

CREATE VIEW vw_seasonal_pricing
AS
SELECT 
    name_route AS 'Маршрут',
    (SELECT name_country FROM country WHERE id_country = marshrut.id_country) AS 'Страна',
    duration_days AS 'Длительность (дней)',
    organizer AS 'Организатор',
    summer_surcharge AS 'Надбавка летом (%)',
    winter_surcharge AS 'Надбавка зимой (%)',
    spring_surcharge AS 'Надбавка весной (%)',
    autumn_surcharge AS 'Надбавка осенью (%)'
FROM marshrut;
GO
