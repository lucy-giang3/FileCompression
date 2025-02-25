#include <string>
#include <vector>
#include "HCTree.hpp"
#include "Helper.hpp"
#include <algorithm>
using namespace std;

// build function 
void HCTree::build(const vector<int>& freqs) {
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for (int i = 0; i < 256; i++) {
        if (freqs[i] != 0) {
            leaves[i] = new HCNode(freqs[i], (unsigned char)i);
            pq.push(leaves[i]);
        }
    }

    if (pq.size() == 1) {
        root = pq.top();
        pq.pop();
        return;
    }

    while (pq.size() > 1) {
        HCNode* c0 = pq.top();
        pq.pop();
        HCNode* c1 = pq.top();
        pq.pop();
        HCNode* parent = new HCNode(c0->count + c1->count, c0->symbol);
        parent->c0 = c0;
        parent->c1 = c1;
        c0->p = parent;
        c1->p = parent;
        pq.push(parent);
    }
    root = pq.top();
}

// helper for destructor 
void deleteNode(HCNode* node) {
    // if node is a leaf node
    if (node == nullptr) {
        return;
    }
    deleteNode(node->c0);
    deleteNode(node->c1);
    delete node;
}

// destructor
HCTree::~HCTree() {
    deleteNode(root);
    leaves.clear();
}

// encode function
void HCTree::encode(unsigned char symbol, FancyOutputStream& out) const {
    // base
    HCNode* currNode = leaves[symbol];
    string code = "";
    // traverse and use tree to encode 
    while (currNode->p != nullptr) {
        if (currNode->p->c0 == currNode) code = '0' + code;
        else code = '1' + code;
        currNode = currNode->p;
    }
    for (char c : code) {
        out.write_bit(c - '0');
    }
}

// decode function
unsigned char HCTree::decode(FancyInputStream& in) const {
    // base 
    HCNode* currNode = root;
    // traverse and use tree to decode 
    while (currNode->c0 != nullptr || currNode->c1 != nullptr) {
        if (in.read_bit() == 0) {
            currNode = currNode->c0;
        } else {
            currNode = currNode->c1;
        }
    }
    return currNode->symbol;
}

