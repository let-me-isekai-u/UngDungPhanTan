#include <iostream>
#include <vector>
#include <map>
#include <cmath>
using namespace std;

class Node {
public:
    int id;
    vector<int> keys;
    vector<Node*> fingerTable;
    Node* successor;

    Node(int id) : id(id), successor(nullptr) {
        fingerTable.resize(3, nullptr);
    }

    void findSuccessor(vector<Node*>& nodes, int id) {
        for (auto& node : nodes) {
            if (node->id >= id) {
                successor = node;
                return;
            }
        }
        successor = nodes[0];
    }

    void buildFingerTable(vector<Node*>& nodes, int m) {
        for (int i = 0; i < m; ++i) {
            int fingerId = (id + (int)pow(2, i)) % (int)pow(2, m);
            for (auto& node : nodes) {
                if (node->id >= fingerId) {
                    fingerTable[i] = node;
                    break;
                }
            }
        }
    }

    Node* findKeyOwner(int key) {
        if (find(keys.begin(), keys.end(), key) != keys.end())
            return this;
        for (int i = fingerTable.size() - 1; i >= 0; --i) {
            if (fingerTable[i] != nullptr && fingerTable[i]->id <= key)
                return fingerTable[i]->findKeyOwner(key);
        }
        return successor->findKeyOwner(key);
    }
};

int main() {
    int m = 3;
    vector<Node*> nodes = {
        new Node(0),
        new Node(3),
        new Node(5),
        new Node(7)
    };

    for (auto& node : nodes) {
        node->findSuccessor(nodes, node->id);
        node->buildFingerTable(nodes, m);
    }

    map<int, int> keys = {
        {1, 3},
        {4, 5},
        {6, 7}
    };

    for (auto& key : keys) {
        for (auto& node : nodes) {
            if (node->id >= key.first) {
                node->keys.push_back(key.first);
                break;
            }
        }
    }

    for (auto& key : keys) {
        Node* owner = nodes[0]->findKeyOwner(key.first);
        cout << "Key " << key.first << " is owned by Node " << owner->id << endl;
    }

    for (auto& node : nodes) delete node;
    return 0;
}
