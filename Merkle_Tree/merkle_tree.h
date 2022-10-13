#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include "node.h"
#include "picosha2.h"
#include "misc.h"
#include <vector>
#include <string>

struct MerkleTree {
    Node* root;
    MerkleTree(std::vector<Node*> blocks);
    ~MerkleTree();
    void printTree(Node *n, int indent);
    void deleteTree(Node *n);
};

// MerkleTree::MerkleTree(std::vector<Node*> blocks);
// MerkleTree::~MerkleTree();
// void MerkleTree::printTree(Node* n, int indent);
// void MerkleTree::deleteTree(Node* n);

#endif /* MERKLE_TREE_H */
