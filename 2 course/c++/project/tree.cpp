#include "tree.hpp"
#include "iterator.hpp"
#include <iostream>
#include <algorithm>

AVLNode::AVLNode(int v) : val(v), left(nullptr), right(nullptr), parent(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr) {}
AVLTree::~AVLTree() { clear(root); }

int AVLTree::height(AVLNode* node) { return node ? node->height : 0; }
int AVLTree::balanceFactor(AVLNode* node) { return node ? height(node->left) - height(node->right) : 0; }
void AVLTree::updateHeight(AVLNode* node) { if (node) node->height = 1 + std::max(height(node->left), height(node->right)); }
AVLNode* AVLTree::getRoot() const { return root; }

AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    x->right = y; y->left = T2;
    x->parent = y->parent; y->parent = x;
    if (T2) T2->parent = y;
    
    updateHeight(y); updateHeight(x);
    return x;
}

AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    y->left = x; x->right = T2;
    y->parent = x->parent; x->parent = y;
    if (T2) T2->parent = x;
    
    updateHeight(x); updateHeight(y);
    return y;
}

AVLNode* AVLTree::balance(AVLNode* node, int val) {
    int bf = balanceFactor(node);
    
    if (bf > 1) {
        if (val < node->left->val) return rotateRight(node);
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (bf < -1) {
        if (val > node->right->val) return rotateLeft(node);
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

AVLNode* AVLTree::insert(AVLNode* node, AVLNode* parent, int val) {
    if (!node) {
        AVLNode* newNode = new AVLNode(val);
        newNode->parent = parent;
        return newNode;
    }
    
    if (val < node->val) node->left = insert(node->left, node, val);
    else if (val > node->val) node->right = insert(node->right, node, val);
    else return node;
    
    updateHeight(node);
    return balance(node, val);
}

AVLNode* AVLTree::minNode(AVLNode* node) {
    while (node && node->left) node = node->left;
    return node;
}

AVLNode* AVLTree::deleteNode(AVLNode* node, int val) {
    if (!node) return nullptr;
    
    if (val < node->val) {
        node->left = deleteNode(node->left, val);
        if (node->left) node->left->parent = node;
    } else if (val > node->val) {
        node->right = deleteNode(node->right, val);
        if (node->right) node->right->parent = node;
    } else {
        if (!node->left || !node->right) {
            AVLNode* temp = node->left ? node->left : node->right;
            if (temp) temp->parent = node->parent;
            delete node;
            return temp;
        }
        
        AVLNode* temp = minNode(node->right);
        node->val = temp->val;
        node->right = deleteNode(node->right, temp->val);
        if (node->right) node->right->parent = node;
    }
    
    if (!node) return nullptr;
    updateHeight(node);
    return balance(node, node->val);
}

void AVLTree::inOrder(AVLNode* node, std::vector<int>& result) {
    if (node) {
        inOrder(node->left, result);
        result.push_back(node->val);
        inOrder(node->right, result);
    }
}

void AVLTree::clear(AVLNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void AVLTree::insert(int val) { root = insert(root, nullptr, val); }
void AVLTree::remove(int val) { root = deleteNode(root, val); }

std::vector<int> AVLTree::inOrderTraversal() {
    std::vector<int> result;
    inOrder(root, result);
    return result;
}

void AVLTree::printInOrder() {
    std::vector<int> result = inOrderTraversal();
    for (int val : result) std::cout << val << " ";
    std::cout << std::endl;
}