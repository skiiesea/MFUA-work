#include <iostream>
#include <string>

class StockAnalyst {
private: 
    struct StockData {
        std::string date; // data - дата, строка
        double price; // price - цена, число с плавающей точкой
    };
    StockData* data;
    size_t size; // size - текущее кол-во элементов в массиве
    size_t capacity; // capacity - текущая вместительность в массиве
    void respize(); // respize - увелич размер массива
    double parseDouble(const std::string& s) const; // преобразует строку в число

public:
    StockAnalyst(); 
    ~StockAnalyst(); // создает и удаляет объект класса, освобождая память

    bool loadFromCSV(const std::string& filename); // loadfromcsv - загружает данные из CSV файла
    // возвращает true при успешной загрузке, принимает имя файла как параметр
    double calculateAveragePrice() const; // расчет среднего знач цены
    double calculateStandartDeviation() const; // расчет стандартного отклонения
    std::string findDateWithMinPrice() const; // нахождение мин цены
    std::string findDateWithMaxPrice() const; // нахождение макс цены
    void printAnalysisReport() const; // вывод результатов
    double CorrelationCoefficient() const; // расчет коэффицентов
};
