#include "node.h"
#include <iostream>
#include <stack>

class BPTree {
    public:
        BPTree();
        BPTree(int order);
        void insert(int value);
        bool remove(int value);
        bool exists(int value);
        int get_order();
        void set_order(int new_order);
        void print();
    private:
        Node* root = new Node();
        unsigned int order = 2;
        unsigned int degree = 1;
        int depth = 0;
        void split(Node* node, std::stack<Node*> ancestors, bool leaf);
};