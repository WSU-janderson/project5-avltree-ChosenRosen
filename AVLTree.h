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
 * Declarations and Definitions for AVLTree template class and AVLNode subclass.
 */

/**
 * @class AVLTree
 * @brief Tree ADT implementation as an AVL Tree
 *
 * An AVL (Adelson-Velsky Landis) tree is a self-balancing binary tree with a height guaranteed to be less than
 * ~1.5log_2(S), where S is the size of the tree. Implemented as a template class which accepts any self-comparable
 * type for the key (K), and any type for the value (V).
 *
 * @author Greg Rosen
 * @date November 19, 2025
 *
 * @tparam K Key -- Any self-comparable type.
 * @tparam V Value -- Unrestricted type.
 */
template <typename K, typename V>
requires std::equality_comparable<K> && std::totally_ordered<K> // Ensures K is self-comparable
class AVLTree {
protected:
    /**
     * @class AVLNode
     *
     * @brief Node of AVL Tree
     *
     * Each node stores a key-value pair, as well as pointers to its child nodes.
     * Child node pointers are nullptr if the node does not have a child on that side.
     * Each node also keeps track of its height.
     */
    class AVLNode {
    public:
        K key; // Key of key-value pair. Can be any self-comparable type.
        V value; // Value of key-value pair. Unrestricted type.
        size_t height; // Height of node in AVL tree. Non-negative value which is zero for a leaf node.
        AVLNode* left; // Pointer to left child of node.
        AVLNode* right; // Pointer to right child of node.

        /**
         * @brief Parameterized Constructor for AVLNode.
         *
         * Constructs a node with the provided key-value pair.
         * The node is constructed as a leaf node (height 0, null child pointers).
         *
         * @param key (K) Key of key-value pair.
         * @param value (V) Value of key-value pair.
         */
        AVLNode(K key, V value)
            : key(std::move(key)), value(std::move(value)), height(0), left(nullptr), right(nullptr) {
        }

        /**
         * @brief Getter for number of child nodes of AVLNode.
         *
         * Every binary tree node is either a leaf node with 0 children,
         * or an interior node that may have 1 or 2 children.
         *
         * @return (unsigned char) 0, 1, or 2.
         */
        [[nodiscard]] unsigned char numChildren() const {
            unsigned char numReturned = 0;
            if (left != nullptr) {++numReturned;}
            if (right != nullptr) {++numReturned;}
            return numReturned;
        }

        /**
         * @brief Getter for balance of AVL Node.
         *
         * The balance of an AVL Tree node is the difference between the height of its left subtree and its right subtree.
         * The value is in the range [-h, +h], where h is the height of the node.
         * An AVL tree ensures that after every insertion or removal is processed,
         * no node in the tree will have a balance that is out of the range [-1,1].
         * For the purposes of this algorithm, a nonexistent child node has an effective height of -1.
         *
         * @return (ptrdiff_t) balance of node.
         */
        [[nodiscard]] ptrdiff_t getBalance() const {
            ptrdiff_t lht = -1;
            ptrdiff_t rht = -1;
            if (left != nullptr) {lht = left->height;}
            if (right != nullptr) {rht = right->height;}
            return lht - rht;
        }

        /**
         * @brief Predicate for whether AVLNode is leaf node.
         *
         * A tree node is a leaf node if it has no children nodes.
         * A leaf node is the bottom of at least one descending path through the tree.
         *
         * @return (bool) true if leaf node, false otherwise.
         */
        [[nodiscard]] bool isLeaf() const {
            return ((left == nullptr) && (right == nullptr));
        }

        /**
         * @brief Recalculate the height of an AVLNode.
         *
         * The height of a tree node is the number of edges between it and the deepest node of the subtree for which
         * it is the root. In effect, it is 1 greater than the greater of the heights of its child nodes.
         * For the purposes of this algorithm, a nonexistent child node has an effective height of -1.
         */
        void recalcHeight() {
            if ((left == nullptr) && (right == nullptr)) height = 0;
            else if (left == nullptr) height = right->height + 1;
            else if (right == nullptr) height = left->height + 1;
            else {
                const size_t lht = left->height;
                const size_t rht = right->height;
                if (lht > rht) height = lht + 1;
                else height = rht + 1;
            }
        }
    };

public:
    /**
     * @brief Default constructor for AVLTree.
     *
     * Constructs an empty AVLTree with no nodes and null root.
     */
    AVLTree()
        : root(nullptr), numNodes(0) {
    }

    /**
     * @brief Copy constructor for AVLTree.
     *
     * @param other (AVLTree&) Tree to be copied.
     */
    AVLTree(const AVLTree& other)
        : root(nullptr), numNodes(other.numNodes) {
        root = preOrderTraversal_Clone(other.root);
    }

    /**
     * @brief Destructor for AVLTree.
     *
     * @ref clear.
     */
    ~AVLTree() {
        clear();
    }

    /**
     * @brief Assignment operator overload for AVLTree.
     *
     * @param other (AVLTree&) Tree to be copied.
     * @return (AVLTree&) Deep copy of tree.
     */
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

    /**
     * Array subscript operator overload for AVLTree.
     *
     * Performs a binary search to find the given key.
     * If the key is present, returns a reference to the associated value.
     * If the key is not present, the operator throws a std::out_of_range exception.
     *
     * @param key (K&) key to be searched.
     * @throws std::out_of_range Thrown if key is not present.
     * @return (V&) Value associated with key.
     */
    V& operator[](const K& key) {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) throw std::out_of_range("Key is not present in tree");
        return nodeFound->value;
    }

    /**
     * @brief Returns the size of the AVLTree.
     *
     * The size of a tree is the number of nodes it contains. The size of the tree is tracked as the field numNodes
     * and updated upon node insertion or removal.
     *
     * @return (size_t) Size of tree.
     */
    [[nodiscard]] size_t size() const {
        return numNodes;
    }

    /**
     * @brief Returns the height of the AVLTree.
     *
     * The height of a tree is the height (@ref recalcHeight) of its root node.
     *
     * @return (size_t) Height of tree.
     */
    [[nodiscard]] size_t getHeight() const  {
        if (empty()) return 0;
        return root->height;
    }

    /**
     * @brief Returns a sorted list of keys present in the AVLTree.
     *
     * Performs an in-order traversal of the binary tree to obtain a sorted list of its keys.
     * Calls @ref inOrderTraversal_Keys.
     *
     * @return (std::vector<K>) Sorted list of keys.
     */
    [[nodiscard]] std::vector<K> keys() const {
        std::vector<K> keyList;
        inOrderTraversal_Keys(keyList,root);
        return keyList;
    }

    /**
     * @brief Get the value associated with a given key.
     *
     * Performs a binary search to find the given key.
     * If the key is present, returns a copy of its associated value.
     * If the key is not present, returns nullopt.
     *
     * @param key (K&) Key to be searched.
     * @return (std::optional<V>) Value associated with key, or nullopt if key not present.
     */
    [[nodiscard]] std::optional<V> get(const K& key) const {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) return std::nullopt;
        return nodeFound->value;
    }

    /**
     * @brief Get all values associated with keys in a provided range.
     *
     * Performs a modified in-order traversal of the tree which avoids any subtree with a root for which the key is
     * outside the provided range. Obtains a list of the associated values sorted in order of their keys.
     * NOTE: The values themselves will not be in a sorted order.
     *
     * @param lowKey (K&) Minimum key to be searched.
     * @param highKey (K&) Maximum key to be searched.
     * @return (std::vector<V>) List of values sorted by their keys.
     */
    [[nodiscard]] std::vector<V> findRange(const K& lowKey, const K& highKey) const {
        std::vector<V> valueVector;
        inOrderTraversal_ValuesForKeysInRange(valueVector, root, lowKey, highKey);
        return valueVector;
    }

    /**
     * @brief Predicate for whether AVLTree is empty.
     *
     * The tree is empty if its root is null, or equivalently if its size is 0 (numNodes == 0).
     *
     * @return true if tree is empty, false otherwise.
     */
    [[nodiscard]] bool empty() const  {
        return (root == nullptr);
    }

    /**
     * @brief Predicate for whether key is present is AVLTree.
     *
     * Performs a binary search to find the provided key.
     * @ref binarySearch
     *
     * @param key (K&) Key to be searched.
     * @return (bool) True if key found, false otherwise.
     */
    [[nodiscard]] bool contains(const K& key) const {
        AVLNode* nodeFound = binarySearch(key, root);
        if (nodeFound == nullptr) return false;
        return true;
    }

    /**
     * @brief Converts the tree to an empty state.
     *
     * Performs a post-order traversal of the tree, deleting every node encountered.
     * Leaves the tree empty (empty() will return true) with a null root and size 0.
     */
    void clear() {
        postOrderTraversal_Remove(root);
        root = nullptr;
        numNodes = 0;
    }

    /**
     *
     * @brief Inserts a key-value pair into the AVLTree.
     *
     * Calls @ref binaryInsert to perform the insertion.
     * If the insertion is successful, then the size of the tree is updated.
     *
     * @param key (K&) key of key-value pair to be inserted.
     * @param value (V&) value of key-value pair to be inserted.
     * @return (bool) True if insertion successful, false otherwise.
     */
    bool insert(const K& key, const V& value) {
        const bool insResult = binaryInsert(key, value, root);
        if (insResult) ++numNodes;
        return insResult;
    }

    /**
     * @brief Removes a key-value pair from the AVLTree.
     *
     * Calls @ref binarySearch to determine if the key is in the tree.
     * If the key is not found, the removal fails.
     * If the key is found, then the removal proceeds according to the number of child nodes of the node to be removed:
     * If the node is a leaf node, then it is simply removed.
     * If the node is an interior node with 1 child, then it is replaced by its lone child.
     * If the node is an interior node with 2 children, then it is replaced by its successor. (@ref findSuccessor).
     * The removal is effected using @ref binaryRemove.
     * If the removal is successful, then the size of the tree is updated.
     *
     * @param key (K&) key to be searched.
     * @return (bool) True if removal successful, false otherwise.
     */
    bool remove(const K& key) {
        AVLNode* target = binarySearch(key, root);
        if (target == nullptr) return false; // Binary search did not find key
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

    /**
     * @brief Stream insertion operator overload for AVLTree.
     *
     * Calls @ref reverseInOrderTraversal to print tree to output stream.
     *
     * @param os (std::ostream&) Output stream.
     * @param avlTree (AVLTree&) Tree to be printed.
     * @return (std::ostream&) Updated output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const AVLTree& avlTree) {
        AVLTree::reverseInOrderTraversal_Print(os, avlTree.root, 0);
        return os;
    }

    /**
     * @brief Stream insertion operator overload for AVLNode
     *
     * Prints the key-value pair of the node following the format: "<key, value>"
     *
     * @param os (std::ostream&) Output stream.
     * @param node (AVLNode&) Node to be printed.
     * @return (std::ostream&) Updated output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const AVLNode& node) {
        os << "<" << node.key << ", " << node.value << ">";
        return os;
    }

private:
    AVLNode* root; // Root of AVLTree. The node with the greatest height and no parent.
    size_t numNodes; // The size of the AVLTree. The numer of nodes it contains.

    /**
     * @brief Returns a pointer to the successor of the provided node.
     *
     * The successor of a binary tree node is the one in its right subtree with the smallest key.
     * If the pointer is null or the node does not have a right subtree, returns nullptr.
     * (This should not be possible, as this helper method is only called on a node with 2 children).
     * Upon confirming presence of right subtree, calls @ref findSmallest to find successor.
     *
     * @param node (AVLNode*) Node for which the successor is to be found.
     * @return (AVLNode*) Successor node.
     */
    static AVLNode* findSuccessor(AVLNode* node) {
        if (node == nullptr) return nullptr;
        AVLNode*& rightChild = node->right;
        if (rightChild == nullptr) return nullptr;
        return findSmallest(rightChild);
    }

    /**
     * @brief Finds the descendent of the provided node with the smallest key
     *
     * The 'smallest' node is found by recursively jumping to the left child of nodes until the left pointer is null.
     * If the initial node provided has no left child, then it has no smaller descendent.
     *
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     * @return (AVLNode*) Left child of localRoot, or nullptr if it has no left child.
     */
    static AVLNode* findSmallest(AVLNode* currNode) {
        if (currNode->left == nullptr) return currNode;
        return findSmallest(currNode->left);
    }

    /**
     * @brief Performs a binary search to find a given key in the AVLTree
     *
     * Recursively compares the key being searched to that of the current node,
     * and follows the appropriate child node pointer if it has not yet been found.
     * If the key is found, returns a pointer to the node containing it.
     * If a nullptr is reached, then the key is not in the tree. Returns nullptr.
     *
     * @param key (K) key to be searched.
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     * @return (AVLNode*) Pointer to appropriate next node, or nullptr.
     */
    static AVLNode* binarySearch(const K& key, AVLNode* currNode) {
        if (currNode == nullptr) return nullptr;
        if (currNode->key == key) return currNode;
        if (key < currNode->key) return binarySearch(key, currNode->left);
        return binarySearch(key, currNode->right);
    }

    /**
     * @brief Performs a binary search for the purpose of node insertion
     *
     * Called by @ref insert.
     * Recursively compares the key being searched to that of the current node,
     * and follows the appropriate child node pointer if it has not yet been found.
     * If the key is found, then the insertion fails and the node is not updated with a new value.
     * If a nullptr is reached, then the key may be inserted as that child of the last node investigated.
     * currNode is a reference to the appropriate child ptr field of the parent node, so it may be updated directly.
     * If the insertion occurs, returning through the recursive layers will update the height of each ancestor
     * of the newly inserted node and check if a rebalance is necessary (@ref reBalance).
     *
     * @param key (K&) Key of key-value pair to be inserted.
     * @param value (V&) Value of key-value pair to be inserted.
     * @param currNode (AVLNode*&) Current node being investigated at a given recursive layer.
     * @return (bool) True if insertion is ultimately successful, false otherwise.
     */
    static bool binaryInsert(const K& key, const V& value, AVLNode*& currNode)  {
        if (currNode == nullptr) { // Create new node here if nullptr reached.
            currNode = new AVLNode(key,value);
            return true;
        }
        if (key == currNode->key) return false; // Return false if duplicate key found.
        bool wasIns;
        if (key < currNode->key) wasIns = binaryInsert(key,value,currNode->left);
        else wasIns = binaryInsert(key,value,currNode->right);
        if (wasIns) {
            currNode->recalcHeight();
            reBalance(currNode);
        }
        return wasIns;
    }

    /**
     * @brief Performs a binary search for the purpose of node removal
     *
     * Called by @ref remove.
     * If the node to be removed was found to have 2 children during remove,
     * then its key was swapped with its successor.
     * Therefore, any node found by this method is guaranteed to have only 1 or 2 children.
     * Recursively compares the key being searched to that of the current node,
     * and follows the appropriate child node pointer if it has not yet been found.
     * The node is then deleted if it is a leaf node, or replaced by its lone child if it has 1 child.
     * If the removal occurs, returning through the recursive layers will update the height of each ancestor
     * of the newly inserted node and check if a rebalance is necessary (@ref reBalance).
     *
     * @param key (K) Key of key-value pair to be removed
     * @param currNode (AVLNode*&) Current node being investigated at a given recursive layer.
     */
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

    /**
     * @brief Performs a pre-order traversal for the purpose of cloning a tree node-by-node.
     *
     * Called by copy constructor of AVLTree.
     * Performs a pre-order traversal of the target tree, creating a deep copy of each node in the target tree
     * and connecting it appropriately to other nodes in the clone tree.
     *
     * @param currNode (AVLNode*) Current node being investigated in the target tree at a given recursive layer.
     * @return (AVLNode*) Pointer to deep copy of currNode.
     */
    static AVLNode* preOrderTraversal_Clone(const AVLNode* currNode) {
        if (currNode == nullptr) return nullptr;
        auto* currNodeClone = new AVLNode(currNode->key, currNode->value);
        currNodeClone->height = currNode->height;
        currNodeClone->left = preOrderTraversal_Clone(currNode->left);
        currNodeClone->right = preOrderTraversal_Clone(currNode->right);
        return currNodeClone;
    }

    /**
     * @brief Performs a post-order traversal of the tree for the purpose of emptying a tree.
     *
     * Called by @ref clear
     * Performs a post-order traversal of the tree, deleting each node encountered before proceeding.
     *
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     */
    static void postOrderTraversal_Remove(const AVLNode* currNode)  {
        if (currNode == nullptr) return;
        postOrderTraversal_Remove(currNode->left);
        postOrderTraversal_Remove(currNode->right);
        delete currNode;
    }

    /**
     * @brief Performs an in-order traversal for the purpose of finding all keys in the AVLTree.
     *
     * Called by @ref keys.
     * Performs and in-order traversal of the tree, appending a copy of every key found to the provided list.
     * If originally empty, the final list is guaranteed to be sorted.
     *
     * @param keyList (std::vector<K>&) Provided list to be filled with found keys.
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     */
    static void inOrderTraversal_Keys(std::vector<K>& keyList, const AVLNode* currNode) {
        if (currNode == nullptr) return;
        inOrderTraversal_Keys(keyList, currNode->left);
        keyList.push_back(currNode->key);
        inOrderTraversal_Keys(keyList, currNode->right);
    }

    /**
     *
     * @brief Performs an in-order traversal for the purpose of finding all values with keys in a given range
     *
     * Called by @ref findRange.
     * Performs a modified in-order traversal of the tree which avoids any subtree for which the key of the root
     * is outside the range [minKey, maxKey]. A copy of every found value is appended to the provided list.
     * If originally empty, the list will contain values in sorted order of their keys.
     *
     * @param valueVector (std::vector<V>&) Provided list to be filled with found values.
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     * @param minKey (K) Minimum value of key for which value is to be found.
     * @param maxKey (K) Maximum value of key for which value is to be found.
     */
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

    /**
     * @brief Performs a reverse in-order traversal in order to print all tree nodes.
     *
     * Called by stream insertion operator overload of AVLTree
     * Performs a reverse in-order traversal of the tree to print each node in reverse sorted order on separate lines.
     * Each node is printed with 3*depthCtr spaces preceding it, visually indicating its depth in the tree.
     * Calls the stream insertion operator for each node it prints.
     *
     * @param os (std::ostream&) Output Stream.
     * @param currNode (AVLNode*) Current node being investigated at a given recursive layer.
     * @param depthCtr (size_t) Counter passed between recursive layers which captures depth.
     */
    static void reverseInOrderTraversal_Print(std::ostream& os, const AVLNode* currNode, size_t depthCtr) {
        if (currNode == nullptr) return;
        reverseInOrderTraversal_Print(os, currNode->right, depthCtr + 1);
        for (size_t i = 0; i < depthCtr; ++i) {
            os << "   ";
        }
        os << *currNode << std::endl;
        reverseInOrderTraversal_Print(os, currNode->left, depthCtr + 1);
    }

    /**
     * @brief Rebalances an AVL Node
     *
     * A node with a balance (@ref getBalance) outside the range [-1,1] must be rebalanced in an AVL tree.
     * The rebalancing is accomplished with a sequence of 1 or 2 tree rotations.
     *
     * @param node (AVLNode*&) Node to be checked for rebalance.
     * @return (bool) true if rebalancing occurred, false otherwise.
     */
    static bool reBalance(AVLNode*& node) {
        auto bal = node->getBalance();
        if (bal < -1) {
            auto rcBal = node->right->getBalance();
            if (rcBal > 0) {
                rightRotation(node->right); // Double left rotation, first rotation.
            }
            leftRotation(node); // Single left rotation
            return true;
        }
        if (bal > 1) {
            auto lcBal = node->left->getBalance();
            if (lcBal < 0) {
                leftRotation(node->left); // Double right rotation, first rotation.
            }
            rightRotation(node); // Single right rotation.
            return true;
        }
        return false;
    }

    /**
     * @brief Performs a left tree rotation on the given problem node.
     *
     * A left rotation promotes the right child of the problem node (the hook node) to take its place.
     * If the hook node has a left child, it becomes the right child of the demoted problem node.
     *
     * @param prob (AVLNode*&) Problem node, the one requiring rebalancing.
     */
    static void leftRotation(AVLNode*& prob) {
        AVLNode* hook = prob->right;
        prob->right = hook->left;
        hook->left = prob;
        prob->recalcHeight();
        hook->recalcHeight();
        prob = hook;
    }

    /**
     * @brief Performs a right tree rotation on the given problem node.
     *
     * A right rotation promotes the left child of the problem node (the hook node) to take its place.
     * If the hook node has a right child, it becomes the left child of the demoted problem node.
     *
     * @param prob (AVLNode*&) Problem node, the one requiring rebalancing.
     */
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