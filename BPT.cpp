#include "BPT.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>

BPTree::BPTree() {}

BPTree::BPTree(int order) {
    this->order = order;
    degree = this->order / 2;
}

/** Inserts value into B+ Tree
 * @param value: value to be inserted
 * @return: none
*/
void BPTree::insert(int value) {
    std::stack<Node*> ancestors;
    Node* copy = root;
    int current_depth = depth;

    for(int i = 0; i <= current_depth; ++i) {
        if(i == current_depth) {
            copy->values.push_back(value);
            std::sort(copy->values.begin(), copy->values.end());
            if(copy->values.size() > (order - 1)) { split(copy, ancestors, true); }
        } else {
            if(copy->values.size() > 0) {
                ancestors.push(copy);
                for(int j = copy->values.size() - 1; j >= 0 ; --j) {
                    if(value > copy->values[j]) { copy = copy->children[j+1]; }
                }
                if(value < copy->values[0]) { copy = copy->children[0]; }
            }
        }
    }
}

/** Removes a value from B+ Tree
 * @param value: value to be removed
 * @return bool: true is value is removed, false otherwise
*/
bool BPTree::remove(int value) {
    switch(depth) {
        case 0:
            if(root->values.size() != 0) {
                for(int i = 0; i < root->values.size(); ++i) {
                    if(value == root->values[i]) {
                        root->values.erase(root->values.begin()+i);
                        return 1;
                    }
                }
            }

            break;
        default:
            break;
    }

    return 0;
}

/** Checks if value exists in B+ Tree
 * @param value: value to be checked
 * @return bool: true is value exists, false if not
*/
bool BPTree::exists(int value) {
    Node* node = root;

    for(int i = 0; i <= depth; ++i) {
        if(i != depth) {
            if(value > node->values[node->values.size() - 1]) { node = node->children[node->values.size()]; }
            else {
                for(int j = node->values.size(); j >= 0; --j) {
                    if(value <= node->values[j]) {
                        node = node->children[j];
                    }
                }
            }
        } else {
            return std::find(node->values.begin(), node->values.end(), value) != node->values.end();
        }
    }

    return 0;
}

/** Returns the order of the B+ Tree
 * @param: none
 * @return order: order of the tree
*/
int BPTree::get_order() { return order; }

/** Set order of the tree
 * @param new_order: new order of the B+ Tree
 * @return: none
*/
void BPTree::set_order(int new_order) { order = new_order; degree = new_order / 2; }

/** Prints out the B+ Tree in Breadth First order
 * @param: none
 * @return: none
*/
void BPTree::print() {
    std::queue<Node*> nodes;
    nodes.push(root);
    while(!nodes.empty()) {
        Node* current_node = nodes.front();
        nodes.pop();

        for(int i = 0; i < current_node->children.size(); ++i) {
            nodes.push(current_node->children[i]);
        }

        for(int i = 0; i < current_node->values.size(); ++i) {
            std::cout << current_node->values[i] << ' ';
        }
        std::cout << '\n';
    }
}

/** Splits node into two if node's values are greater than order
 * @param node: Node containing the array of values and children to be split
 * @param ancestors: stack containing all the ancestors of the node
 * @param leaf: bool depending if the node is a leaf node or not
*/
void BPTree::split(Node* node, std::stack<Node*> ancestors, bool leaf) {
    Node* parent = nullptr;
    if(!ancestors.empty()) {
        parent = ancestors.top();
        ancestors.pop();
    }

    Node* new_node = new Node();
    std::vector<int> split_one (node->values.begin(), node->values.begin() + (node->values.size() / 2));
    std::vector<int> split_two (node->values.begin() + (node->values.size() / 2), node->values.end());
    node->values = split_one;
    new_node->values = split_two;
    if(!leaf) { new_node->values.erase(new_node->values.begin()); }

    if(!leaf) {
        std::vector<Node*> split_one_c (node->children.begin(), node->children.begin() + (node->children.size() / 2));
        std::vector<Node*> split_two_c (node->children.begin() + (node->children.size() / 2), node->children.end());
        node->children = split_one_c;
        new_node->children = split_two_c;
    }

    if(!parent) {
        parent = new Node();
        parent->values.push_back(split_two[0]);
        parent->children.insert(parent->children.end(), {node, new_node});
        root = parent;
        ++depth;
    } else {
        std::vector<int>::iterator it;
        it = (leaf) ? std::find(parent->values.begin(), parent->values.end(), node->values[0])
                    : std::lower_bound(parent->values.begin(), parent->values.end(), new_node->values[1]);
        if(it != parent->values.end()) {
            int position = std::distance(parent->values.begin(), it) + 1;
            parent->values.insert(parent->values.begin() + position, new_node->values[0]);
            parent->children.insert(parent->children.begin() + position + 1, new_node);
        } else {
            parent->values.insert(parent->values.begin(), new_node->values[0]);
            parent->children[0] = new_node;
            parent->children.insert(parent->children.begin(), node);
        }
    }

    if(parent->values.size() > order - 1) { split(parent, ancestors, false); }
}