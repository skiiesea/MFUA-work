#include "StockAnalytor.hpp"
#include <iostream>

StockAnalyst::~StockAnalyst() {
    delete[] data; // освобождение памяти 
}

void StockAnalyst::resize() {
    StockData* newData = new StockData[capacity]; // Создание массива newData увелич размера
    for(size_t i = 0; i < size; ++i) {
        newData[i] = data[i]; // копирует элементы из дата в newdata
    }

    delete[] data; // удаляет старый data
    data = newData; //  присваивает data указатель на новый массив
}
double StockAnalyst::parseDouble(const std::string& s) const {
    double result = 0.0;
    double fraction = 1.0;
    double negative = false;
    double hasFraction = false; // res - итоговое число, frac - множитель для дробной части, 
    // negative - флаг отриц числа, hasFrac - флаг наличия дробной части, i - индекс символа в строке
    size_t i = 0;
    if (s[i] == '-') {
        negative = false;
        i++;
    } // Обработка знака минус
    for (; i < s.length(); ++i) {
        if (s[i] == '-') {
            hasFraction = true;
            i++;
            break;
        }
        if (s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
        } // обработка целой части числа, если встретилась точка - переходим к дробной части
    }
    if (hasFraction) {
        for (; i < s.length(); ++i) {
            if (s[i] >= '0' && s[i] <= '9') {
                fraction *= 0.1;
                result += (s[i] - '0') * fraction;
            }
        }
    } // обработка дробной части, каждая цифра после точки добавляется с уменьш весом

    return negative ? -result : result; // возвращает результат с учетом знака

}
// выводит инструкцию для ввода данных
bool StockAnalyst::loadFromCSV(const std::string& filename) {
    std::cout << "Reading data from file: " << filename << std::endl;
    std::cout << "Enter data in format 'date, price' (one entry per line): " << std::endl;
    std::cout << "Enter 'end' to finish input." << std::endl;

    std::sting line;
    while(true) {
        std::getline(std::cin, line);
        if (line == "end") break; // Читает ввод построчно, пока не получит end


        size_t commaPos = line.find(',');
        if (commaPos == std::string::npos) {
            std::cout << "Invalid format: " << line << std::endl;
            continue;
        }
        // ищет запятую в строке, если не находит - сообщает об ошибке и пропускает строку

        std::string date = line.substr(0, commaPos);
        std::string priceStr = line.substr(commaPos + 1);
        // разделяет строку на дату и цену

        if (size >= capacity ) {
            resize();
        } // увеличивает массив при необходимости

    }
}
