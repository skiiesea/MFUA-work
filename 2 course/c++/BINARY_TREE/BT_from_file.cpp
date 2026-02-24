#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

template <typename T>
struct Node {
     T val;
     Node *parent; // родитель
     Node *left;	// левый потомок
     Node *right;	// правый потомок
     Node(T val) : val(val), parent(nullptr), left(nullptr), right(nullptr) {}
     ~Node() {
        if (left) delete left;
        if (right) delete right;
        
     }
};


Node<int>* treeFromOrder(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open())
        throw std::invalid_argument("File is not available!");
    std::vector<int> inorder, preorder;
    {
    std::string line;
    std::getline(file,line);
    std::stringstream ss(line);
    int val;
    while(ss >> val) {
        preorder.push_back(val);
        }
    }
{
    std::string line;
    std::getline(file,line);
    std::stringstream ss(line);
    int val;
    while(ss >> val) {
        inorder.push_back(val);
        }
    }
    if(inorder.size() != preorder.size())
        throw std::length_error("lines have to have equal size");
    Node<int>* root = new Node<int>(preorder.at(0));
    std::vector<Node<int>*> nodes = {root};
    for (size_t i = 1, sz = preorder.size(); i < sz; ++i) {
        Node<int>* current_node = new Node<int>(preorder[i]);
        nodes.push_back(current_node);
        size_t self = 0, left = 0;
        for(size_t j = 0; j < sz; ++j) {
            if (inorder[j] == preorder[i]) self = j;
            else if (inorder[j] == preorder[i-1]) left = j;
        }
        if(self < left) 
        {
            nodes[i]->parent = nodes[i-1]; nodes[i-1]->left = nodes[i];
        } else 
        {
            int true_parent = inorder[self-1];
            size_t idx_true_parent = 0;
            while (preorder[idx_true_parent++] != true_parent);
            nodes[i]->parent = nodes[idx_true_parent];
            nodes[idx_true_parent-1]->right = nodes[i];
        }
    }
    return root;
}
Node<int>* treeFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::invalid_argument("File is not available!");
    
    int n;
    file >> n;
    file.ignore(); // чтобы скушать /n

    int* parents = new int[n];
    int* lefts = new int[n];
    int* rights = new int[n];
    for (int i = 0; i < n; ++i)
        parents[i] = -1, lefts[i] = -1, rights[i] = -1;
    for (int i = 0; i < n; ++i) {
        std::string line;
        std::getline(file, line);
        std::stringstream stream(line);
        int parent, left, right;
        stream >> parent >> left >> right;
        parents[left] = parent;
        parents[right] = parent;
        lefts[parent] = left;
        rights[parent] = right;
    }
    int root_i = -1;
    while (parents[++root_i] != -1);
    Node<int> *root = new Node<int>(root_i);
    Node<int> **nodes_p = new Node<int>*[n];
    for (int i = 0; i < n; ++i) 
        if (parents[i] != -1) nodes_p[i] = new Node<int>(i);
        else nodes_p[i] = root;
    for (int i = 0; i < n; ++i) {
        if (parents[i] != -1) nodes_p[i]->parent = nodes_p[parents[i]];
        if (lefts[i] != -1) nodes_p[i]->left = nodes_p[lefts[i]];
        if (rights[i] != -1) nodes_p[i]->right = nodes_p[rights[i]];
    }

    return root;
}


void print_inorder(Node<int>* root) {
    if (!root->left && !root->right) {std::cout << root->val; return;}
    if (root->left) print_inorder(root->left);
    std::cout << root->val;
    if (root->right) print_inorder(root->right);
    return;
}

void print_postorder(Node<int>* root) {
    if (root->left) print_postorder(root->left);
    if (root->right) print_postorder(root->right);
    std::cout << root->val;
    return;
}

void print_preorder(Node<int>* root) {
    std::cout << root->val;
    if (root->left) print_preorder(root->left);
    if (root->right) print_preorder(root->right);
    return;
}

int main() {
    auto root = treeFromOrder("test_1.txt");
    print_inorder(root);
    std::cout << std::endl;
    print_postorder(root);
    std::cout << std::endl;
    print_preorder(root);
    std::cout << std::endl;
    delete root;
    return 0;
}