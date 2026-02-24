#include <vector>
#include<stack>
#include<iostream>

int main(){
    std::stack<int> s;
    std::stack<int, std::vector<long>> s1;
    std::stack<int, std::vector<char>> s2;

    s2.push(1000);
    std::cout << s2.top() << std::endl;

}