#include <iostream>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

bool checkUnival(TreeNode* node, int value) {
    if (!node) return true;
    if (node->val != value) return false;
    return checkUnival(node->left, value) && checkUnival(node->right, value);
}

bool isUnivalTree(TreeNode* root) {
    if (!root) return true;
    return checkUnival(root, root->val);
}

int main() {
    
    TreeNode* root1 = new TreeNode(5, new TreeNode(5, new TreeNode(5), new TreeNode(5)), new TreeNode(5, nullptr, new TreeNode(5)));
    
    TreeNode* root2 = new TreeNode(5, new TreeNode(5), new TreeNode(4));
    
    std::cout << "Tree 1 is unival: " << (isUnivalTree(root1) ? "true" : "false") << std::endl;
    std::cout << "Tree 2 is unival: " << (isUnivalTree(root2) ? "true" : "false") << std::endl;
    
    return 0;
}