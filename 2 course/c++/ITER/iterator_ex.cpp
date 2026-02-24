#include <iostream>
#include <cstring>

class MyString {
private:
    template <typename Iter>
    class MyStringIterator {
      friend MyString;
    public:
        typedef Iter iterator_type;
        typedef std::forward_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        typedef iterator_type value_type;
        typedef iterator_type& reference;
        typedef iterator_type* pointer;

        iterator_type* value_;
    private:
        MyStringIterator(Iter* p) : value_(p) {};
    public:
        MyStringIterator(const MyStringIterator& rhs) : value_(rhs.value_) {}
        MyStringIterator(MyStringIterator&& rhs) { std::swap(value_, rhs.value_); }
        bool operator==(const MyStringIterator& rhs) const noexcept {return value_ == rhs.value_; }
        bool operator!=(const MyStringIterator& rhs) const noexcept {return value_ != rhs.value_; }
        reference operator*() const noexcept {return *value_;}
        MyStringIterator& operator++() noexcept {
            ++value_;
            return *this;
        }
        MyStringIterator operator++(int) noexcept {
            MyStringIterator temp {*this};
            ++(*this);
            return temp;
        }
    };
private:
    size_t sz_;
    char* str_;
public:
    MyString() : sz_(0), str_(nullptr) {}
    MyString(const char* src) {
        if (src == "" || src == nullptr) {
            sz_ = 0;
            str_ = nullptr;
        } else {
            sz_ = strlen(src);
            str_ = new char[sz_];
            char* current = str_;
            for (size_t i = 0; i < sz_; ++i) {
                *current = src[i];
                ++current;
            }
        }
    }
    MyString(const MyString& src) {
        if (src.sz_ > 0) {
            sz_ = src.sz_;
            str_ = new char[sz_];
            for (size_t i = 0; i < sz_; ++i) 
                str_[i] = src.str_[i];
        } else {
            sz_ = 0;
            str_ = nullptr;
        }
    }

    MyString(MyString&& rhs) : sz_(0), str_(nullptr) {
        std::swap(sz_, rhs.sz_);
        std::swap(str_, rhs.str_);
    }

    ~MyString() {
        delete[] str_;
    }

    typedef MyStringIterator<char> iterator;
    typedef MyStringIterator<const char> const_iterator;

    iterator begin() { return iterator(str_);}
    iterator end() { return iterator(str_ + sz_); }

    const_iterator begin() const { return const_iterator(str_);}
    const_iterator end() const { return const_iterator(str_ + sz_); }

    void dump(std::ostream& os) const {
        for (auto& ch : *this)
            os << ch;
    }
};

std::ostream& operator<<(std::ostream& os, const MyString& str) {
    str.dump(os);
    return os;
}

int main() {
    MyString hello = "Hello, world!";
    std::cout << hello << std::endl;
    auto p = hello.begin();
    std::cout << "begin(): " << *p << std::endl;
    for (auto& c : hello) 
        std::cout << c << '\n';
    return 0;
}