#include <iostream>
#include <string>

class StockAnalytor {
private: 
    struct StockData {
        std::string date; // data - дата, строка
        double price; // price - цена, число с плавающей точкой
    }
    StockData* data;// data - указатель на динам массив структур stockdata
    size_t size; // size - текущее кол-во элементов в массиве
    size_t capacity; // capacity - текущая вместительность в массиве

    void respize(); // respize - увелич размер массива
    double parseDouble(const std::string& s) const; // преобразует строку в число

public:
    StockAnalyst(); 
    ~StockAnalyst(); // создает и удаляет объект класса, освобождая память

    bool loadFromCSV(const std::string& filename); // loadfromcsv - загружает данные из CSV файла
    // возвращает true при успешной загрузке, принимает имя файла как параметр
    double getMinPrice() const; // возвращает мин цену
    double getMaxPrice() const; // возвращает макс цену
    double getAveragePrice() const; // возвращает среднюю цену
    void printSummary() const; // печатает свободную статистику
};
