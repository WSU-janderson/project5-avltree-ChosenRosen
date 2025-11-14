#include "AVLTree.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

/*
 * Greg Rosen
 * Project 5: AVL Tree
 * Declaration for AVLTree Class and AVLNode subclass
 */

AVLTree::AVLTree() :
    root(nullptr), numNodes(0){}

AVLTree::AVLTree(const AVLTree& other) :
    root(cloneTree(other.root)), numNodes(other.numNodes) {}

AVLTree::~AVLTree() {
    clear();
}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this != &other) {
        clear();
        AVLTree temp(other);
        this->root = temp.root;
        this->numNodes = temp.numNodes;
        temp.root = nullptr;
    }
    return *this;
}

AVLTree::ValueType& AVLTree::operator[](const KeyType& key) {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) throw std::out_of_range("Key is not present in tree");
    return nodeFound->value;
}

size_t AVLTree::size() const {
    return numNodes;
}

size_t AVLTree::getHeight() const {
    if (empty()) return 0;
    return root->height;
}

std::vector<AVLTree::KeyType> AVLTree::keys() const {
    std::vector<KeyType> keyList;
    inOrderTraversal_Keys(keyList,root);
    return keyList;
}

std::optional<AVLTree::ValueType> AVLTree::get(const KeyType &key) const {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) return std::nullopt;
    return nodeFound->value;
}

std::vector<AVLTree::ValueType> AVLTree::findRange(const KeyType &lowKey, const KeyType &highKey) const {
    std::vector<AVLTree::ValueType> valueVector;
    inOrderTraversal_ValuesForKeysInRange(valueVector, root, lowKey, highKey);
    return valueVector;
}

bool AVLTree::empty() const {
    return (root == nullptr);
}

bool AVLTree::contains(const KeyType &key) const {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) return false;
    return true;
}

void AVLTree::clear() {
    postOrderTraversalRemove(root);
    root = nullptr;
    numNodes = 0;
}

bool AVLTree::insert(const KeyType& key, const ValueType& value) {
    const bool insResult = binaryInsert(key, value, root);
    if (insResult) ++numNodes;
    return insResult;
}

bool AVLTree::remove(const KeyType &key) {
    AVLNode* target = binarySearch(key, root);
    if (target == nullptr) return false;
    auto nChildren = target->numChildren();
    if (nChildren < 2) binaryRemove(key, root);
    else {
        const AVLNode* newTarget = findSuccessor(target);
        const KeyType newTargetKey = newTarget->key;
        const ValueType newTargetVal = newTarget->value;
        binaryRemove(newTarget->key,root);
        target->key = newTargetKey;
        target->value = newTargetVal;
    }
    --numNodes;
    return true;
}

std::ostream & operator<<(std::ostream &os, const AVLTree &avlTree) {
    AVLTree::reverseInOrderTraversal(os, avlTree.root,0);
    return os;
}

std::ostream & operator<<(std::ostream &os, const AVLTree::AVLNode &node) {
    os << "<" << node.key << ", " << node.value << ">";
    return os;
}

// Private methods for AVLTree

AVLTree::AVLNode * AVLTree::findPredecessor(AVLNode *node) {
    AVLNode*& leftChild = node->left;
    if (leftChild == nullptr) return nullptr;
    return findLargest(leftChild);
}

AVLTree::AVLNode* AVLTree::findSuccessor(AVLNode* node) {
    AVLNode*& rightChild = node->right;
    if (rightChild == nullptr) return nullptr;
    return findSmallest(rightChild);
}

AVLTree::AVLNode * AVLTree::findSmallest(AVLNode *localRoot) {
    if (localRoot->left == nullptr) return localRoot;
    return findSmallest(localRoot->left);
}

AVLTree::AVLNode * AVLTree::findLargest(AVLNode *localRoot) {
    if (localRoot->right == nullptr) return localRoot;
    return findLargest(localRoot->right);
}

AVLTree::AVLNode* AVLTree::binarySearch(const KeyType& key, AVLNode* currNode) {
    if (currNode == nullptr) return nullptr;
    if (currNode->key == key) return currNode;
    if (key < currNode->key) return binarySearch(key, currNode->left);
    return binarySearch(key, currNode->right);
}

bool AVLTree::binaryInsert(const KeyType& key, const ValueType& value, AVLNode*& currNode) {
    if (currNode == nullptr) {
        currNode = new AVLNode(key,value);
        return true;
    }
    if (key == currNode->key) return false;
    bool wasIns;
    if (key < currNode->key) wasIns = binaryInsert(key,value,currNode->left);
    else wasIns = binaryInsert(key,value,currNode->right);
    if (wasIns) currNode->recalcHeight();
    return wasIns;
}

void AVLTree::binaryRemove(const KeyType &key, AVLNode *&currNode) {
    if (key == currNode->key) {
        const AVLNode* toDelete = currNode;
        if (currNode->isLeaf()) currNode = nullptr;
        else if (currNode->right == nullptr) currNode = currNode->left;
        else currNode = currNode->right;
        delete toDelete;
    }
    else {
        if (key < currNode->key) binaryRemove(key, currNode->left);
        else binaryRemove(key, currNode->right);
        currNode->recalcHeight();
    }
}

AVLTree::AVLNode* AVLTree::cloneTree(const AVLNode* currRoot) {
    if (currRoot == nullptr) return nullptr;
    auto* clonedCurrRoot = new AVLNode(currRoot->key, currRoot->value);
    clonedCurrRoot->height = currRoot->height;
    clonedCurrRoot->left = cloneTree(currRoot->left);
    clonedCurrRoot->right = cloneTree(currRoot->right);
    return clonedCurrRoot;
}

void AVLTree::postOrderTraversalRemove(const AVLNode *currNode) {
    if (currNode == nullptr) return;
    postOrderTraversalRemove(currNode->left);
    postOrderTraversalRemove(currNode->right);
    delete currNode;
}

void AVLTree::inOrderTraversal_Keys(std::vector<AVLTree::KeyType> &keyVector, const AVLNode *currNode) {
    if (currNode == nullptr) return;
    inOrderTraversal_Keys(keyVector, currNode->left);
    keyVector.push_back(currNode->key);
    inOrderTraversal_Keys(keyVector, currNode->right);
}

void AVLTree::inOrderTraversal_ValuesForKeysInRange(std::vector<AVLTree::ValueType> &valueVector, const AVLNode *currNode, const KeyType &minKey, const KeyType &maxKey) {
    if (currNode == nullptr) return;
    if (currNode->key > minKey) {
        inOrderTraversal_ValuesForKeysInRange(valueVector, currNode->left, minKey, maxKey);
    }
    if (currNode->key >= minKey && currNode->key <= maxKey) {
        valueVector.push_back(currNode->value);
    }
    if (currNode->key < maxKey) {
        inOrderTraversal_ValuesForKeysInRange(valueVector, currNode->right, minKey, maxKey);
    }
}

void AVLTree::reverseInOrderTraversal(std::ostream& os, AVLNode *currNode, size_t depthCtr) {
    if (currNode == nullptr) return;
    reverseInOrderTraversal(os, currNode->right, depthCtr + 1);
    for (size_t i = 0; i < depthCtr; ++i) {
        os << "   ";
    }
    os << *currNode << std::endl;
    reverseInOrderTraversal(os, currNode->left, depthCtr + 1);
}

// Public methods for AVLNode

AVLTree::AVLNode::AVLNode(KeyType key, ValueType value)
    : key(std::move(key)), value(std::move(value)), height(0), left(nullptr), right(nullptr) {
}

unsigned char AVLTree::AVLNode::numChildren() const {
    unsigned char numReturned = 0;
    if (left != nullptr) {++numReturned;}
    if (right != nullptr) {++numReturned;}
    return numReturned;
}

bool AVLTree::AVLNode::isLeaf() const {
    return ((left == nullptr) && (right == nullptr));
}

void AVLTree::AVLNode::recalcHeight() {
    if ((left == nullptr) && (right == nullptr)) {height = 0;}
    else if (left == nullptr) {height = right->height + 1;}
    else if (right == nullptr) {height = left->height + 1;}
    else {
        const size_t leftHeight = left->height;
        const size_t rightHeight = right->height;
        if (leftHeight > rightHeight) {height = leftHeight + 1;}
        else {height = rightHeight + 1;}
    }
}