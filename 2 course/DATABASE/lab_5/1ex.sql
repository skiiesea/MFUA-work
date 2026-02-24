USE Tour
GO

CREATE VIEW vw_country_updatable
AS SELECT id_country,
    name_country,
    cost_visa,
    capital,
    languagee,
    currency
FROM country;
GO

UPDATE vw_country_updatable 
SET name_country = 'Франция (обновлено)' 
WHERE id_country = 1;
GO