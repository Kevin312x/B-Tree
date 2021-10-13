#include <vector>

class Node {
    public:
        Node(){};
        std::vector<int> values;
        std::vector<Node*> children;
    private:
};