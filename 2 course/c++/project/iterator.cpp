#include "iterator.hpp"
#include "tree.hpp"
#include <iostream>
#include <algorithm>



// Конструктор по умолчанию
AVLTreeIterator::AVLTreeIterator() : current(nullptr) {}

// Конструктор с корнем дерева
AVLTreeIterator::AVLTreeIterator(AVLNode* root) {
    pushLeft(root);
    if (!nodeStack.empty()) {
        current = nodeStack.top();
        nodeStack.pop();
    } else {
        current = nullptr;
    }
}

// Вспомогательная функция: добавляем все левые узлы в стек
void AVLTreeIterator::pushLeft(AVLNode* node) {
    while (node != nullptr) {
        nodeStack.push(node);
        node = node->left;
    }
}

// Оператор разыменования
int AVLTreeIterator::operator*() const {
    if (current == nullptr) {
        throw std::runtime_error("Attempt to dereference end iterator");
    }
    return current->val;
}

// Префиксный инкремент
AVLTreeIterator& AVLTreeIterator::operator++() {
    if (current == nullptr) {
        return *this;
    }
    
    // Если есть правое поддерево, добавляем его левую ветвь
    if (current->right != nullptr) {
        pushLeft(current->right);
    }
    
    // Берем следующий узел из стека
    if (!nodeStack.empty()) {
        current = nodeStack.top();
        nodeStack.pop();
    } else {
        current = nullptr;
    }
    
    return *this;
}

// Постфиксный инкремент
AVLTreeIterator AVLTreeIterator::operator++(int) {
    AVLTreeIterator temp = *this;
    ++(*this);
    return temp;
}

// Оператор сравнения
bool AVLTreeIterator::operator==(const AVLTreeIterator& other) const {
    return current == other.current;
}

// Оператор неравенства
bool AVLTreeIterator::operator!=(const AVLTreeIterator& other) const {
    return current != other.current;
}



// Конструктор контейнера
AVLTreeContainer::AVLTreeContainer(AVLTree& t) : tree(t) {}

// Начальный итератор
AVLTreeIterator AVLTreeContainer::begin() {
    return AVLTreeIterator(tree.getRoot());
}

// Конечный итератор
AVLTreeIterator AVLTreeContainer::end() {
    return AVLTreeIterator(nullptr);
}