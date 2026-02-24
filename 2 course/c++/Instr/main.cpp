std::string StockAnalyst::findDateWithMinPrice() const {
    if (size == 0) return "";

    size_t maxIndex = 0;

    for(size_t i = 1; i < size; ++i) {
        if(data[i].price > data[maxIndex].price) {
            maxIndex = i;
        }
    }
    return data[maxIndex].date;
} // линейный поиск максимального значения, возвращает соответствующую дату.

std::string StockAnalyst::findDateWithMinPrice() const {
    if (size == 0) return "";

    size_t minIndex = 0;

    for(size_t i = 1; i < size; ++i) {
        if(data[i].price < data[minIndex].price) {
            minIndex = i;
        }
    }
    return data[minIndex].date;
} // линейный поиск минимального значения, возвращает соответствующую дату.
std::cout << "Date with minimum price: " << findDateWithMinPrice() << std::endl;
    std::cout << "Date with maximum price: " << findDateWithMaxPrice() << std::endl;