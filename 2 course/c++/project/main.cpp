#include "tree.hpp"
#include "iterator.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    AVLTree tree;
    
    // Вставка элементов
    int keys[] = {10, 20, 30, 40, 50, 25, 5, 15};
    
    std::cout << "input elements: ";
    for (int key : keys) std::cout << key << " ";
    for (int key : keys) tree.insert(key);
    
    std::cout << "\n\n1. Range-based for loop:\n";
    AVLTreeContainer container(tree);
    for (int key : container) std::cout << key << " ";
    
    std::cout << "\n\n2. Hands iterators:\n";
    std::cout << "   Iter: ";
    for (auto it = container.begin(); it != container.end(); ++it) std::cout << *it << " ";
    
    std::cout << "\n\n3. STL algor:\n";
    auto min_it = std::min_element(container.begin(), container.end());
    auto max_it = std::max_element(container.begin(), container.end());
    if (min_it != container.end()) std::cout << "   min: " << *min_it << "\n";
    if (max_it != container.end()) std::cout << "   max: " << *max_it << "\n";
    
    // Счет элементов через итераторы
    int count = 0;
    for (auto it = container.begin(); it != container.end(); ++it) count++;
    std::cout << "   Total elements: " << count << "\n";
    
    // Поиск элементов
    std::cout << "\n4. Search elements:\n";
    std::cout << "   Search 25: ";
    bool found = false;
    for (int key : container) if (key == 25) found = true;
    std::cout << (found ? "find" : "not find") << "\n";
    
    // Удаление и проверка
    std::cout << "\n5. Delete element 30:\n";
    tree.remove(30);
    std::cout << "   After delete: ";
    for (int key : container) std::cout << key << " ";
    
    // Добавление новых
    std::cout << "\n\n6. ADD NEW ELEMENT (100, 1):\n";
    tree.insert(100);
    tree.insert(1);
    std::cout << "   New tree: ";
    for (int key : container) std::cout << key << " ";
    
    // Обратный порядок через вектор
    std::cout << "\n\n7. Reverse:\n";
    std::vector<int> vec;
    for (int key : container) vec.push_back(key);
    std::reverse(vec.begin(), vec.end());
    std::cout << "   (revers na russkom): ";
    for (int key : vec) std::cout << key << " ";
    
    // Проверка сортированности
    std::cout << "\n\n8. Check sort in-order:\n";
    bool sorted = std::is_sorted(container.begin(), container.end());
    std::cout << "   Tree sorted: " << (sorted ? "yes" : "no") << "\n";
    
    // Демонстрация префиксного и постфиксного инкремента
    std::cout << "\n9. Present inkr:\n";
    auto it = container.begin();
    std::cout << "   first elem: " << *it << "\n";
    std::cout << "   Oper ++: " << *(++it) << "\n";
    std::cout << "   Oper ++(int): " << *(it++) << " (now: " << *it << ")\n";
    
    // Тест на пустое дерево
    std::cout << "\n10. Test null tree:\n";
    AVLTree emptyTree;
    AVLTreeContainer emptyContainer(emptyTree);
    std::cout << "   null tree (range-based for): ";
    for (int key : emptyContainer) std::cout << key; // Ничего не выведет
    std::cout << " [null]\n";
    
    // Проверка begin() == end() для пустого дерева
    bool empty = (emptyContainer.begin() == emptyContainer.end());
    std::cout << "   begin() == end(): " << (empty ? "yes" : "no") << "\n";
    
    return 0;
}