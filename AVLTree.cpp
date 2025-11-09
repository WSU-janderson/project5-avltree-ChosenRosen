#include "AVLTree.h"

#include <string>
#include <utility>

/*
 * Greg Rosen
 * Project 5: AVL Tree
 * Declaration for AVLTree Class and AVLNode subclass
 */

AVLTree::AVLTree() : root(nullptr), numNodes(0){
}

AVLTree::AVLTree(KeyType &key, const ValueType value) {
    root = new AVLNode(key,value);
    numNodes = 1;
}

AVLTree::AVLTree(const AVLTree &other) {
    numNodes = other.numNodes;
    root = cloneTree(other.root);
}

AVLTree::~AVLTree() {
    postOrderTraversalRemove(root);
}

AVLTree & AVLTree::operator=(const AVLTree &other) {
    if (this != &other) {
        postOrderTraversalRemove(root);
        AVLTree temp(other);
        this->root = temp.root;
        this->numNodes = temp.numNodes;
        temp.root = nullptr;
    }
    return *this;
}

AVLTree::ValueType & AVLTree::operator[](const KeyType &key) {
}

size_t AVLTree::size() const {
    return numNodes;
}

size_t AVLTree::getHeight() const {
    return root->height;
}

std::vector<AVLTree::KeyType> AVLTree::keys() const {
}

std::optional<AVLTree::ValueType> AVLTree::get(const KeyType &key) const {
}

std::vector<AVLTree::KeyType> AVLTree::findRange(const KeyType &lowKey, const KeyType &highKey) const {
}

bool AVLTree::empty() const {
    return (root == nullptr);
}

bool AVLTree::contains(const KeyType &key) const {
}

bool AVLTree::insert(const KeyType &key, ValueType value) {
}

bool AVLTree::remove(const KeyType &key) {
}

std::ostream & operator<<(std::ostream &os, const AVLTree &avlTree) {
}

// Private methods for AVLTree

AVLTree::AVLNode* AVLTree::cloneTree(const AVLNode* currRoot) {
    if (currRoot == nullptr) {
        return nullptr;
    }

    auto* clonedCurrRoot = new AVLNode(currRoot->key, currRoot->value);

    clonedCurrRoot->height = currRoot->height;
    clonedCurrRoot->left = cloneTree(currRoot->left);
    clonedCurrRoot->right = cloneTree(currRoot->right);

    return clonedCurrRoot;
}

void AVLTree::postOrderTraversalRemove(const AVLNode *currNode) {
    if (currNode == nullptr) {
        return;
    }
    postOrderTraversalRemove(currNode->left);
    postOrderTraversalRemove(currNode->right);
    delete currNode;
}

// Public methods for AVLNode

AVLTree::AVLNode::AVLNode(KeyType key, const ValueType value)
    : key(std::move(key)), value(value), height(0), left(nullptr), right(nullptr) {
}

size_t AVLTree::AVLNode::numChildren() const {
    size_t numReturned = 0;
    if (left != nullptr) {++numReturned;}
    if (right != nullptr) {++numReturned;}
    return numReturned;
}

bool AVLTree::AVLNode::isLeaf() const {
    return ((left == nullptr) && (right == nullptr));
}

size_t AVLTree::AVLNode::getBalance() const {
    size_t leftHeight, rightHeight;
    if (left == nullptr) {leftHeight = -1;}
    else {leftHeight = left->height;}
    if (right == nullptr) {rightHeight = -1;}
    else {rightHeight = right->height;}
    return (leftHeight - rightHeight);
}

// Private methods for AVLNode

void AVLTree::AVLNode::recalcHeight() {
    if (isLeaf()) {height = 0;}
    else if (left == nullptr) {height = right->height + 1;}
    else if (right == nullptr) {height = left->height + 1;}
    else {
        const size_t leftHeight = left->height;
        const size_t rightHeight = right->height;
        if (leftHeight > rightHeight) {height = leftHeight + 1;}
        else {height = rightHeight + 1;}
    }
}


// bool AVLTree::removeNode(AVLNode*& current){
//     if (!current) {
//         return false;
//     }
//
//     AVLNode* toDelete = current;
//     auto nChildren = current->numChildren();
//     if (current->isLeaf()) {
//         // case 1 we can delete the node
//         current = nullptr;
//     } else if (current->numChildren() == 1) {
//         // case 2 - replace current with its only child
//         if (current->right) {
//             current = current->right;
//         } else {
//             current = current->left;
//         }
//     } else {
//         // case 3 - we have two children,
//         // get smallest key in right subtree by
//         // getting right child and go left until left is null
//         AVLNode* smallestInRight = current->right;
//         // I could check if smallestInRight is null,
//         // but it shouldn't be since the node has two children
//         while (smallestInRight->left) {
//             smallestInRight = smallestInRight->left;
//         }
//         std::string newKey = smallestInRight->key;
//         int newValue = smallestInRight->value;
//         remove(root, smallestInRight->key); // delete this one
//
//         current->key = newKey;
//         current->value = newValue;
//
//         current->height = current->getHeight();
//         balanceNode(current);
//
//         return true; // we already deleted the one we needed to so return
//     }
//     delete toDelete;
//
//     return true;
// }
//
// bool AVLTree::remove(AVLNode *&current, KeyType key) {
//     return false;
// }
//
// void AVLTree::balanceNode(AVLNode *&node) {
// }
