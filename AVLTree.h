#ifndef AVLTREE_H
#define AVLTREE_H

#include <cstdint>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

/*
 * Greg Rosen
 * Project 5: AVL Tree
 * Declaration for AVLTree Class and AVLNode subclass
 */

template <typename K, typename V>
class AVLTree {
protected:
    class AVLNode {
    public:
        K key;
        V value;
        size_t height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(K key, V value)
            : key(std::move(key)), value(std::move(value)),height(0), left(nullptr), right(nullptr) {
        }


        [[nodiscard]] unsigned char numChildren() const {
            unsigned char numReturned = 0;
            if (left != nullptr) {++numReturned;}
            if (right != nullptr) {++numReturned;}
            return numReturned;
        }

        [[nodiscard]] ptrdiff_t getBalance() const {
            ptrdiff_t leftHeight;
            ptrdiff_t rightHeight;
            if (left == nullptr) {leftHeight = -1;}
            else {leftHeight = left->height;}
            if (right == nullptr) {rightHeight = -1;}
            else {rightHeight = right->height;}
            return leftHeight - rightHeight;
        }

        [[nodiscard]] bool isLeaf() const {
            return ((left == nullptr) && (right == nullptr));
        }


        void recalcHeight() {
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
    };

public:
    AVLTree()
        : root(nullptr), numNodes(0) {
    }

    AVLTree(const AVLTree& other)
        : root(nullptr), numNodes(other.numNodes) {
        root = cloneTree(other.root);
    }

    ~AVLTree() {
        clear();
    }

    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            clear();
            AVLTree temp(other);
            this->root = temp.root;
            this->numNodes = temp.numNodes;
            temp.root = nullptr;
        }
        return *this;
    }

    V& operator[](const K& key) {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) throw std::out_of_range("Key is not present in tree");
        return nodeFound->value;
    }

    [[nodiscard]] size_t size() const {
        return numNodes;
    }

    [[nodiscard]] size_t getHeight() const  {
        if (empty()) return 0;
        return root->height;
    }

    [[nodiscard]] std::vector<K> keys() const {
        std::vector<K> keyList;
        inOrderTraversal_Keys(keyList,root);
        return keyList;
    }

    [[nodiscard]] std::optional<V> get(const K& key) const {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) return std::nullopt;
        return nodeFound->value;
    }

    [[nodiscard]] std::vector<V> findRange( const K& lowKey, const K& highKey) const {
        std::vector<V> valueVector;
        inOrderTraversal_ValuesForKeysInRange(valueVector, root, lowKey, highKey);
        return valueVector;
    }

    [[nodiscard]] bool empty() const  {
        return (root == nullptr);
    }

    [[nodiscard]] bool contains(const K& key) const {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) return false;
        return true;
    }

    void clear() {
        postOrderTraversalRemove(root);
        root = nullptr;
        numNodes = 0;
    }

    bool insert(const K& key, const V& value) {
        if (numNodes >= (SIZE_MAX - 1) / 2) {
            std::cout << "The tree is full! Cannot insert more nodes." << std::endl;
            return false;
        }
        const bool insResult = binaryInsert(key, value, root);
        if (insResult) ++numNodes;
        return insResult;
    }

    bool remove(const K& key) {
        AVLNode* target = binarySearch(key, root);
        if (target == nullptr) return false;
        auto nChildren = target->numChildren();
        if (nChildren < 2) binaryRemove(key, root);
        else {
            const AVLNode* newTarget = findSuccessor(target);
            const K newTargetKey = newTarget->key;
            const V newTargetVal = newTarget->value;
            binaryRemove(newTarget->key,root);
            target->key = newTargetKey;
            target->value = newTargetVal;
        }
        --numNodes;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const AVLTree & avlTree) {
        AVLTree::reverseInOrderTraversal(os, avlTree.root, 0);
        return os;
    }

    friend std::ostream& operator<<(std::ostream& os, const AVLNode & node) {
        os << "<" << node.key << ", " << node.value << ">";
        return os;
    }

private:
    AVLNode* root;
    size_t numNodes;

    static AVLNode* findSuccessor(AVLNode* node) {
        AVLNode*& rightChild = node->right;
        if (rightChild == nullptr) return nullptr;
        return findSmallest(rightChild);
    }

    static AVLNode* findSmallest(AVLNode* localRoot) {
        if (localRoot->left == nullptr) return localRoot;
        return findSmallest(localRoot->left);
    }

    static AVLNode* binarySearch(const K& key, AVLNode* currNode) {
        if (currNode == nullptr) return nullptr;
        if (currNode->key == key) return currNode;
        if (key < currNode->key) return binarySearch(key, currNode->left);
        return binarySearch(key, currNode->right);
    }

    static bool binaryInsert(const K& key, const V& value, AVLNode*& currNode)  {
        if (currNode == nullptr) {
            currNode = new AVLNode(key,value);
            return true;
        }
        if (key == currNode->key) return false;
        bool wasIns;
        if (key < currNode->key) wasIns = binaryInsert(key,value,currNode->left);
        else wasIns = binaryInsert(key,value,currNode->right);
        if (wasIns) {
            currNode->recalcHeight();
            reBalance(currNode);
        }
        return wasIns;
    }

    static void binaryRemove(const K& key, AVLNode*& currNode) {
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
            reBalance(currNode);
        }
    }

    static AVLNode* cloneTree(const AVLNode* currRoot) {
        if (currRoot == nullptr) return nullptr;
        auto* clonedCurrRoot = new AVLNode(currRoot->key, currRoot->value);
        clonedCurrRoot->height = currRoot->height;
        clonedCurrRoot->left = cloneTree(currRoot->left);
        clonedCurrRoot->right = cloneTree(currRoot->right);
        return clonedCurrRoot;
    }

    static void postOrderTraversalRemove(const AVLNode* currNode)  {
        if (currNode == nullptr) return;
        postOrderTraversalRemove(currNode->left);
        postOrderTraversalRemove(currNode->right);
        delete currNode;
    }

    static void inOrderTraversal_Keys(std::vector<K>& keyVector, const AVLNode* currNode) {
        if (currNode == nullptr) return;
        inOrderTraversal_Keys(keyVector, currNode->left);
        keyVector.push_back(currNode->key);
        inOrderTraversal_Keys(keyVector, currNode->right);
    }

    static void inOrderTraversal_ValuesForKeysInRange(std::vector<V>& valueVector, const AVLNode* currNode, const K& minKey, const K& maxKey) {
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

    static void reverseInOrderTraversal(std::ostream& os, const AVLNode* currNode, size_t depthCtr) {
        if (currNode == nullptr) return;
        reverseInOrderTraversal(os, currNode->right, depthCtr + 1);
        for (size_t i = 0; i < depthCtr; ++i) {
            os << "   ";
        }
        os << *currNode << std::endl;
        reverseInOrderTraversal(os, currNode->left, depthCtr + 1);
    }

    static void reBalance(AVLNode*& node) {
        auto bal = node->getBalance();
        if (bal < -1) {
            auto rcBal = node->right->getBalance();
            if (rcBal > 0) {
                rightRotation(node->right);
            }
            leftRotation(node);
        }
        else if (bal > 1) {
            auto lcBal = node->left->getBalance();
            if (lcBal < 0) {
                leftRotation(node->left);
            }
            rightRotation(node);
        }
    }

    static void leftRotation(AVLNode*& prob) {
        AVLNode* hook = prob->right;
        prob->right = hook->left;
        hook->left = prob;
        prob->recalcHeight();
        hook->recalcHeight();
        prob = hook;
    }

    static void rightRotation(AVLNode*& prob) {
        AVLNode* hook = prob->left;
        prob->left = hook->right;
        hook->right = prob;
        prob->recalcHeight();
        hook->recalcHeight();
        prob = hook;
    }
};

#endif //AVLTREE_H