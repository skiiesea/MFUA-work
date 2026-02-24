#include "date_lib.h"
#include <iostream>

int main(){
    MyDate date {9, Month::Apr, 2025};
    std::cout << "Today: " << date << std::endl;
    date.add_days(22);
    std::cout << "After 22 days: " << date << std::endl;
    date.add_months(-10);
    std::cout << "Before 10 months: " << date << std::endl;
    date.add_years(7);
    std::cout << "after 7 years: " << date << std::endl;
}