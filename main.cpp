#include "GeneralTree.h"
#include <iostream>

// Constructor y Destructor de TreeNode
TreeNode::TreeNode(BookData data) : data(data) {}
TreeNode::~TreeNode() {
    for (TreeNode* child : children) {
        delete child;
    }
}

// Constructor y Destructor de GeneralTree
GeneralTree::GeneralTree() : root(nullptr) {}
GeneralTree::~GeneralTree() { delete root; }

void GeneralTree::setRoot(TreeNode* newRoot) { this->root = newRoot; }

// --- IMPLEMENTACIÓN DE LISTAR ---
void GeneralTree::listar() {
    if (this->root != nullptr) preOrderList(this->root);
}
void GeneralTree::preOrderList(TreeNode* node) {
    if (node == nullptr) return;
    std::cout << node->data.id << std::endl;
    for (TreeNode* child : node->children) {
        preOrderList(child);
    }
}

// --- IMPLEMENTACIÓN DE BORRAR RATINGS ---
void GeneralTree::borrar_ratings(double r) {
    this->root = pruneRatings(this->root, r);
}
TreeNode* GeneralTree::pruneRatings(TreeNode* node, double r) {
    if (node == nullptr) return nullptr;
    
    std::vector<TreeNode*> validChildren;
    for (TreeNode* child : node->children) {
        TreeNode* processedChild = pruneRatings(child, r);
        if (processedChild != nullptr) {
            validChildren.push_back(processedChild);
        }
    }
    node->children = validChildren;

    if (node->data.averageRating <= r) {
        delete node;
        return nullptr;
    }
    return node;
}

// --- IMPLEMENTACIÓN DE PRECURSORES ---
void GeneralTree::precursores() {
    if (this->root != nullptr) findPrecursors(this->root);
}
void GeneralTree::findPrecursors(TreeNode* node) {
    if (node == nullptr) return;

    bool isPrecursor = !node->data.similarBooks.empty();
    for (const SimilarBook& similar : node->data.similarBooks) {
        if (similar.publicationYear <= node->data.publicationYear) {
            isPrecursor = false;
            break;
        }
    }

    if (isPrecursor) {
        std::cout << node->data.id << std::endl;
    }

    for (TreeNode* child : node->children) {
        findPrecursors(child);
    }
}