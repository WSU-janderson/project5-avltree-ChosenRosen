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
    using KeyType = size_t;
    using ValueType = std::string;

protected:
    class AVLNode {
    public:
        KeyType key;
        ValueType value;
        size_t height;
        AVLNode* left;
        AVLNode* right;

        AVLNode(KeyType key, ValueType value);
        [[nodiscard]] unsigned char numChildren() const; // 0, 1 or 2
        [[nodiscard]] bool isLeaf() const; // true or false
        void recalcHeight();
    };

public:

    AVLTree();
    AVLTree(const AVLTree& other);

    ~AVLTree();

    AVLTree& operator=(const AVLTree& other);
    ValueType& operator[](const KeyType& key);

    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] std::vector<KeyType> keys() const;
    [[nodiscard]] std::optional<ValueType> get(const KeyType& key) const;
    [[nodiscard]] std::vector<ValueType> findRange( const KeyType& lowKey, const KeyType& highKey) const;

    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool contains(const KeyType& key) const;

    void clear();
    bool insert(const KeyType& key, const ValueType& value);
    bool remove(const KeyType& key);

    friend std::ostream& operator<<(std::ostream& os, const AVLTree & avlTree);
    friend std::ostream& operator<<(std::ostream& os, const AVLNode & node);

private:
    AVLNode* root;
    size_t numNodes;

    static AVLNode* findPredecessor(AVLNode* node);
    static AVLNode* findSuccessor(AVLNode* node) ;
    static AVLNode* findSmallest(AVLNode* localRoot);
    static AVLNode* findLargest(AVLNode* localRoot);

    static AVLNode* binarySearch(const KeyType& key, AVLNode* currNode);
    static bool binaryInsert(const KeyType& key, const ValueType& value, AVLNode*& currNode);
    static void binaryRemove(const KeyType& key, AVLNode*& currNode);

    static AVLNode* cloneTree(const AVLNode* currRoot);
    static void postOrderTraversalRemove(const AVLNode* currNode);
    static void inOrderTraversal_Keys(std::vector<KeyType>& keyVector, const AVLNode* currNode);
    static void inOrderTraversal_ValuesForKeysInRange(std::vector<ValueType>& valueVector, const AVLNode* currNode, const KeyType& minKey, const KeyType& maxKey);
    static void reverseInOrderTraversal(std::ostream& os, AVLNode* currNode, size_t depthCtr);
};

#endif //AVLTREE_H
