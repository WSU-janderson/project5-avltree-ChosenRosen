/*
Driver code for testing your AVL Tree
This is NOT the test code for grading,
instead for you to get an idea of how to test the tree
 */
#include "AVLTree.h"
#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>

int basicFunctionalityTest();
int rebalancingTest();
int memLeakTest();

int main(){
    basicFunctionalityTest();
    rebalancingTest();
    memLeakTest();
    return 0;
}

int basicFunctionalityTest() {

    std::cout << "Creating Empty Tree..." << std::endl;
    AVLTree tree;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    std::cout << "Creating another tree and inserting keys 0 through 9 in such a way that rebalancing is not required..." << std::endl;
    AVLTree anotherTree;
    anotherTree.insert(5, "five");
    anotherTree.insert(1, "one");
    anotherTree.insert(7, "seven");
    anotherTree.insert(0, "zero");
    anotherTree.insert(3, "three");
    anotherTree.insert(6, "six");
    anotherTree.insert(8, "eight");
    anotherTree.insert(2, "two");
    anotherTree.insert(4, "four");
    anotherTree.insert(9, "nine");
    std::cout << "Empty?..." << (anotherTree.empty() ? "true" : "false") << " Size=" << anotherTree.size() << " Height=" << anotherTree.getHeight() << std::endl;
    std::cout << anotherTree << std::endl;

    std::cout << "Copying second tree to first tree and displaying first tree..." << std::endl;
    tree = anotherTree;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    std::cout << "Clearing second tree and displaying..." << std::endl;
    anotherTree.clear();
    std::cout << "Empty?..." << (anotherTree.empty() ? "true" : "false") << " Size=" << anotherTree.size() << " Height=" << anotherTree.getHeight() << std::endl;
    std::cout << anotherTree << std::endl;

    std::cout << "Does contains return true for value in tree?..." << (tree.contains(8) ? "yes" : "NO!") << std::endl;
    std::cout << "Does contains return false for value not in tree?..." << (tree.contains(11) ? "NO!" : "yes") << std::endl;

    std::cout << "Attempting to insert duplicate..." << std::endl;
    const bool insFlag = tree.insert(8, "IMPOSTOR!");
    std::cout << "Insert failed?..." << (insFlag ? "NO!" : "correct");
    std::cout << " Displaying tree..." << std::endl;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    std::cout << "Attempting to remove key not in tree..." << std::endl;
    const bool remFlag = tree.remove(11);
    std::cout << "Remove failed?..." << (remFlag ? "NO!" : "correct");
    std::cout << " Displaying tree..." << std::endl;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    constexpr AVLTree::KeyType changeKey = 3;
    std::cout << "Changing value of " << changeKey << " using brackets..." << std::endl;
    tree[changeKey] = "blarg";
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;
    std::cout << "Trying to access key not in tree using brackets..." << std::endl;
    try {
        tree[11] = "IMPOSTOR";
        std::cout << "The access did not throw an exception. Oh Noes!" << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << "An exception was thrown. Its message is..." << std::endl;
        std::cout << e.what() << std::endl;
    }

    std::cout << "Getting value of " << changeKey << " using get..." << std::endl;
    std::optional<AVLTree::ValueType> optReturn = tree.get(changeKey);
    if (optReturn.has_value()) std::cout << optReturn.value() << std::endl;
    else std::cout << "Not in tree" << std::endl;
    std::cout << "Trying to get value not in tree..." << std::endl;
    optReturn = tree.get(11);
    if (optReturn.has_value()) std::cout << optReturn.value() << std::endl;
    else std::cout << "Not in tree" << std::endl;

    std::cout << "Getting list of keys using keys() method..." << std::endl;
    std::vector<AVLTree::KeyType> keyList = tree.keys();
    for (const AVLTree::KeyType& key : keyList) std::cout << key << ", ";
    std::cout << std::endl;

    std::cout << "Getting list of values in entire tree using findRange() method with range including all keys..." << std::endl;
    std::vector<AVLTree::ValueType> list = tree.findRange(0,9);
    for (const AVLTree::ValueType& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    constexpr AVLTree::KeyType minKey = 4;
    constexpr AVLTree::KeyType maxKey = 7;
    std::cout << "Getting list of values with keys between " << minKey << " and " << maxKey << std::endl;
    list = tree.findRange(minKey, maxKey);
    for (const AVLTree::ValueType& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    std::vector<AVLTree::KeyType> removeList;
    removeList.push_back(9);
    removeList.push_back(1);
    removeList.push_back(5);
    removeList.push_back(3);
    removeList.push_back(0);
    removeList.push_back(4);
    removeList.push_back(8);
    removeList.push_back(6);
    removeList.push_back(2);
    removeList.push_back(7);
    std::cout << "Testing removal of keys one-by-one..." << std::endl;
    for (AVLTree::KeyType key : removeList) {
        std::cout << "Removing key=" << key << "..." << std::endl;
        tree.remove(key);
        std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
        std::cout << tree << std::endl;
    }

    return 0;
}

int rebalancingTest() {
    std::cout << "Testing AVL-style tree rebalancing." << std::endl;
    std::cout << "Keys from 0 to 30 will be inserted one-by-one at random, which will very likely require many rotations along the way to rebalance" << std::endl;
    AVLTree tree;
    std::vector<AVLTree::KeyType> randInserts;
    for (int i=0; i<31; ++i) {
        randInserts.push_back(i);
    }
    std::mt19937 rngEngine(std::random_device{}());
    std::ranges::shuffle(randInserts, rngEngine);
    for (AVLTree::KeyType key : randInserts) {
        std::cout << "Inserting key = " << key << "..." << std::endl;
        tree.insert(key, "");
        std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
        std::cout << tree << std::endl;
    }

    return 0;
}

int memLeakTest() {
    constexpr size_t numTrials = 100000;
    constexpr size_t treeSize = 100;
    std::cout << "Making " << numTrials << " identical trees one-by-one." << std::endl;
    std::cout << treeSize << " keys will be inserted, all removed, then inserted again before the tree leaves scope and is destructed." << std::endl;
    std::cout << "Beginning..." << std::endl;
    // Breakpoint here to check memory before beginning.
    for (size_t i = 0; i < numTrials; ++i) {
        // Breakpoint here to check memory for each loop if desired.
        AVLTree tree;
        size_t j;
        for (j = 0; j < treeSize ; ++j) {
            tree.insert(j, "");
        }
        for (j = 0; j < treeSize ; ++j) {
            tree.remove(j);
        }
        for (j = 0; j < treeSize ; ++j) {
            tree.insert(j, "");
        }
    }
    // Breakpoint here to check final memory usage.
    return 0;
}


