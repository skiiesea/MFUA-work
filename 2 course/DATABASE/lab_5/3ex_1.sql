USE Tour
GO

CREATE VIEW vw_country_visa_requirements
AS
SELECT 
    name_country AS 'Страна',
    cost_visa AS 'Стоимость визы',
    'Визовая' AS 'Тип визы',
    10 AS 'Срок оформления'
FROM country
WHERE cost_visa > 0

UNION ALL

SELECT 
    name_country AS 'Страна',
    cost_visa AS 'Стоимость визы',
    'Безвизовая' AS 'Тип визы',
    0 AS 'Срок оформления'
FROM country
WHERE cost_visa = 0;
GO
