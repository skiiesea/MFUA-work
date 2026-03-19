#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct Node {
    bool IsAccept;
    char charToParent;
    Node *parent;
    Node *suffix;
    Node *up;
    std::vector<int> acceptPatternNumber;
    std::map<char, Node*> paths;
    std::map<char, Node*> children;
};

class Trie {
private:
    Node* root;
public:        
     void addString(std::string str, int patternNumber) {
        Node *current = root;
        for (char c : str) {
            if(current->children.find(c) != current->children.end()) {
                current->children[c] = new Node{false, c, current};
            }
            current = current->children[c];
        }
        current->IsAccept = true;
        current->acceptPatternNumber.push_back(patternNumber);  
    }
    Node* getSuffix(Node *node) {
        if(!node->suffix) {
            if(node == root || node->parent == root)
                node->suffix = root;
            else
                node->suffix = getLink(getSuffix(node->parent), node->charToParent);
        }
        return node->suffix;
    }
    Node *getLink(Node *node, char c) {
        if(node->paths.find(c) == node->paths.end()) {
            if(node->children.find(c) != node->children.end())
                node->paths[c] = node->children[c];
            else if (node == root)
                node->paths[c] = root;
            else
                node->paths[c] = getLink(getSuffix(node), c);
        }
        return node->paths[c];
    }
    Node *getUp(Node *node) {
        if(!node->up) {
            Node *suff = getSuffix(node);
            if(suff->IsAccept)
                node->up = suff;
            else if (suff == root)
                node->up = root;
            else
                node->up = getUp(suff);
        }
        return node->up;
    }
    bool processText(std::string text) {
        Node *current = root;
        for(char c : text) {
            current = getLink(current, c);
            if (current->IsAccept) return true;
        }
        return false;
    }
};

