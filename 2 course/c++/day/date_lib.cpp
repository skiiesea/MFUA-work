#include "date_lib.h"
#include <iostream>
int Day::get_day() const {return d;}

int Year::get_year() const {return y;}

bool Year::is_leap() const {

    return (y % 4) == 0 && (y % 400) != 0;

}

Month Month::Jan() {return Month(1);}
Month Month::Feb() {return Month(2);}
Month Month::Mar() {return Month(3);}
Month Month::Apr() {return Month(4);}
Month Month::May() {return Month(5);}
Month Month::Jun() {return Month(6);}
Month Month::Jul() {return Month(7);}
Month Month::Aug() {return Month(8);}
Month Month::Sep() {return Month(9);}
Month Month::Oct() {return Month(10);}
Month Month::Nov() {return Month(11);}
Month Month::Dec() {return Month(12);}

int Month::get_month() const {return m;}
const Month &Month::operator++(){
    m += 1;
    if (m > 12) m = 1;
    return *this;
}

const Month Month::operator++(int){
    Month tmp{*this};
    ++*this;
    return tmp;
}

 
const Month &Month::operator--(){
    m -= 1;
    if (m < 1) m = 12;
    return *this;
}

const Month Month::operator--(int){
    Month tmp{*this};
    ++*this;
    return tmp;
}

int Month::get_max_days(const Year &y) const{
    switch (m) {
        case 1: return 31;
        case 2: return y.is_leap() ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
    }

}

MyDate::MyDate(Day day, Month month, Year year) : day(day), month(month), year(year)
{
    if (day.get_day() > month.get_max_days(year) || 
    day.get_day() < 1 || year.get_year() < 1) { std::terminate(); }
}

const MyDate &MyDate::add_day(int days){
    day = Day(day.get_day() + days);
    while (day.get_day() > month.get_max_days(year)) {
        day = Day(day.get_day() - month.get_max_days(year));
        ++month;
        if (month == Month::Jan()) year = Year(year.get_year() - 1);
    }
    while (day.get_day() > month.get_max_days(year)) {
        day = Day(day.get_day() - month.get_max_days(year));
        --month;
        if (month == Month::Jan()) year = Year(year.get_year() - 1)
    }
    if (day.get_day() > month.get_max_days(year)) 
        day = Day(month.get_max_days(year));
        
    return *this;
}

const MyDate &MyDate::add_month(int months) {
    int year = months / 12;
    year = Year(year.get_year() + year);
    if (year >= 0){
    months = months % 12;
        while (months > 0) {
            ++month;
            if (month == Month::Jan()) year = Year(year.get_year() + 1);
            --months;}
    } else {
        months = months % 12;
        while (months < 0) {
            --month;
            if (month = Month::Dec())
            year = Year(year.get_year() - 1);
            ++months;
        }
    }
    return *this;
}

const MyDate &MyDate::add_year(int years){
    year = Year(year.get_year() + year);
    return *this;
}
void MyDate::dump(std::ostream &os) const{
    os << day.get_day() <<
            '.' << month.get_month() <<
            '.' << year.get_year();
};

std::ostream& operator<<(std::ostream &os, const MyDate &obj){
    obj.dump(os);
    return os;
}
