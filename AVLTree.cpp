#include "AVLTree.h"

#include <stdexcept>
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
    clear();
}

AVLTree & AVLTree::operator=(const AVLTree &other) {
    if (this != &other) {
        clear();
        AVLTree temp(other);
        this->root = temp.root;
        this->numNodes = temp.numNodes;
        temp.root = nullptr;
    }
    return *this;
}

AVLTree::ValueType & AVLTree::operator[](const KeyType &key) {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) {throw std::out_of_range("Key is not present in tree");}
    return nodeFound->value;
}

size_t AVLTree::size() const {
    return numNodes;
}

size_t AVLTree::getHeight() const {
    return root->height;
}

std::vector<AVLTree::KeyType> AVLTree::keys() const {
    std::vector<AVLTree::KeyType> keyVector;
    inOrderTraversalKeys(keyVector,root);
    return keyVector;
}

std::optional<AVLTree::ValueType> AVLTree::get(const KeyType &key) const {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) {return std::nullopt;}
    return nodeFound->value;
}

std::vector<AVLTree::ValueType> AVLTree::findRange(const KeyType &lowKey, const KeyType &highKey) const {
    std::vector<AVLTree::ValueType> valueVector;
    inOrderTraversalValues(valueVector, root, lowKey, highKey);
    return valueVector;
}

bool AVLTree::empty() const {
    return (root == nullptr);
}

bool AVLTree::contains(const KeyType &key) const {
    AVLNode* nodeFound = binarySearch(key, root);
    if (nodeFound == nullptr) {return false;}
    return true;
}

void AVLTree::clear() {
    postOrderTraversalRemove(root);
    root = nullptr;
    numNodes = 0;
}

bool AVLTree::insert(const KeyType &key, ValueType value) {
    if (root == nullptr) {
        root = new AVLNode(key,value);
        return true;
    }
    return binaryInsert(key, value, root);
    // Recalculate height and balance of every ancestor of newly inserted node
}

bool AVLTree::remove(const KeyType &key) {
    // if (root->key == key) {}
    // bool sideWithChild;
    // AVLNode* ParentOfNodeToRemove = binarySearchParent(key, root, sideWithChild);
    // if (ParentOfNodeToRemove == nullptr) {
    //     return false;
    // }
    // AVLNode* NodeToRemove;
    // sideWithChild ? NodeToRemove = ParentOfNodeToRemove->right : NodeToRemove = ParentOfNodeToRemove->left;
    // auto nChildren = NodeToRemove->numChildren();
    // if (nChildren == 0) {
    //     delete NodeToRemove;
    //     sideWithChild ? ParentOfNodeToRemove->right = nullptr : ParentOfNodeToRemove->left = nullptr;
    // }
    // else if (nChildren == 1) {
    //     AVLNode* childOfNodeRemoved;
    //
    // }


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

AVLTree::AVLNode * AVLTree::binarySearchParent(const KeyType &key, AVLNode *currNode, bool& sideWithChild) const {
    if (key < currNode->key) {
        if (currNode->left == nullptr) {
            return nullptr;
        }
        else if (key == currNode->left->key) {
            sideWithChild = false;
            return currNode;
        }
        else {
            return binarySearchParent(key, currNode->left, sideWithChild);
        }
    }
    else {
        if (currNode->right == nullptr) {
            return nullptr;
        }
        else if (key == currNode->right->key) {
            sideWithChild = true;
            return currNode;
        }
        else {
            return binarySearchParent(key, currNode->right, sideWithChild);
        }
    }
}

AVLTree::AVLNode * AVLTree::binarySearch(const KeyType& key, AVLNode* currNode) const {
    if (currNode == nullptr) {
        return nullptr;
    }
    else if (currNode->key == key) {
        return currNode;
    }
    else if (key < currNode->key) {
        return binarySearch(key, currNode->left);
    }
    else {
        return binarySearch(key, currNode->right);
    }
}

bool AVLTree::binaryInsert(const KeyType &key, ValueType value, AVLNode *currNode) {
    if (currNode->key == key) {
        return false;
    }
    else if (key < currNode->key) {
        if (currNode->left == nullptr) {
            auto* newNode = new AVLNode(key,value);
            currNode->left = newNode;
            return true;
        }
        else {
            return binaryInsert(key, value, currNode->left);
        }
    }
    else {
        if (currNode->right == nullptr) {
            auto* newNode = new AVLNode(key,value);
            currNode->right = newNode;
            return true;
        }
        else {
            return binaryInsert(key, value, currNode->right);
        }
    }
}

void AVLTree::postOrderTraversalRemove(const AVLNode *currNode) {
    if (currNode == nullptr) {
        return;
    }
    postOrderTraversalRemove(currNode->left);
    postOrderTraversalRemove(currNode->right);
    delete currNode;
}

void AVLTree::inOrderTraversalKeys(std::vector<AVLTree::KeyType> &keyVector, const AVLNode *currNode) {
    if (currNode == nullptr) {
        return;
    }
    inOrderTraversalKeys(keyVector, currNode->left);
    keyVector.push_back(currNode->key);
    inOrderTraversalKeys(keyVector, currNode->right);
}

void AVLTree::inOrderTraversalValues(std::vector<AVLTree::ValueType> &valueVector, const AVLNode *currNode, const KeyType &minKey, const KeyType &maxKey) {
    if (currNode == nullptr) {
        return;
    }
    else if (currNode->key > maxKey) {
        inOrderTraversalValues(valueVector, currNode->left, minKey, maxKey);
    }
    else if (currNode->key < minKey) {
        inOrderTraversalValues(valueVector, currNode->right, minKey, maxKey);
    }
    else {
        inOrderTraversalValues(valueVector, currNode->left, minKey, maxKey);
        valueVector.push_back(currNode->value);
        inOrderTraversalValues(valueVector, currNode->right, minKey, maxKey);
    }
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

