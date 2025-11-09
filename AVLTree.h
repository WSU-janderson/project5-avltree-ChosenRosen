#ifndef AVLTREE_H
#define AVLTREE_H

#include <optional>
#include <string>
#include <vector>

/*
 * Greg Rosen
 * Project 5: AVL Tree
 * Declaration for AVLTree Class and AVLNode subclass
 */

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;

    AVLTree();
    AVLTree(KeyType& key , ValueType value);
    AVLTree(const AVLTree& other);
    ~AVLTree();

    AVLTree& operator=(const AVLTree& other);
    ValueType& operator[](const KeyType& key);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] std::vector<KeyType> keys() const;
    [[nodiscard]] std::optional<ValueType> get(const KeyType& key) const;
    [[nodiscard]] std::vector<KeyType> findRange( const KeyType& lowKey, const KeyType& highKey) const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool contains(const KeyType& key) const;

    bool insert(const KeyType& key, ValueType value);
    bool remove(const KeyType& key);

    friend std::ostream& operator<<(std::ostream& os, const AVLTree & avlTree);

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(KeyType key, ValueType value);
        [[nodiscard]] size_t numChildren() const; // 0, 1 or 2
        [[nodiscard]] bool isLeaf() const; // true or false
        [[nodiscard]] size_t getBalance() const;
    private:
        void recalcHeight();
    };


private:
    AVLNode* root;
    size_t numNodes;

    static AVLNode* cloneTree(const AVLNode* currRoot);

    static void postOrderTraversalRemove(const AVLNode* currNode);

    // /* Helper methods for remove */
    // // this overloaded remove will do the recursion to remove the node
    // bool remove(AVLNode*& current, KeyType key);
    // // removeNode contains the logic for actually removing a node based on the number of children
    // bool removeNode(AVLNode*& current);
    // // You will implement this, but it is needed for removeNode()
    // void balanceNode(AVLNode*& node);

};

#endif //AVLTREE_H
