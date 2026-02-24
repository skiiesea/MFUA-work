#include <cstring>
#include <iostream>
int sum_code(char const *str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res += str[i];
    return res;
}

int main() {
    char const *str1 = "Hello, world!";
    char const *str2 = "Finaly I'm in classroom!";
    std::cout << "str1 sum = " << sum_code(str1) << std::endl;
    std::cout << "str2 sum = " << sum_code(str2) << std::endl;
}