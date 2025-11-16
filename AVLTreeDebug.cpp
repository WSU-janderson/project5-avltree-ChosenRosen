#include "AVLTree.h"
#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>

/*
 * Greg Rosen
 * Project 5: AVL Tree
 * Debug tests for AVL Tree
 * Presumes that KeyType = size_t and ValueType = std::string
 */

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
    AVLTree<std::string, size_t> tree;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    std::cout << "Creating another tree and inserting keys \"A\" through \"I\" in such a way that rebalancing is not required..." << std::endl;
    AVLTree<std::string, size_t> anotherTree;
    anotherTree.insert("F", 5);
    anotherTree.insert("B", 1);
    anotherTree.insert("H", 7);
    anotherTree.insert("A", 0);
    anotherTree.insert("D", 3);
    anotherTree.insert("G", 6);
    anotherTree.insert("I", 8);
    anotherTree.insert("C", 2);
    anotherTree.insert("E", 4);
    anotherTree.insert("J", 9);

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

    std::cout << "Does contains return true for value in tree?..." << (tree.contains("A") ? "yes" : "NO!ERROR!") << std::endl;
    std::cout << "Does contains return false for value not in tree?..." << (tree.contains("IMPOSTOR") ? "NO!ERROR!" : "yes") << std::endl;

    std::cout << "Attempting to insert duplicate..." << std::endl;
    const bool insFlag = tree.insert("A", 1337);
    std::cout << "Insert failed?..." << (insFlag ? "NO!" : "correct");
    std::cout << " Displaying tree..." << std::endl;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    std::cout << "Attempting to remove key not in tree..." << std::endl;
    const bool remFlag = tree.remove("IMPOSTOR");
    std::cout << "Remove failed?..." << (remFlag ? "NO!" : "correct");
    std::cout << " Displaying tree..." << std::endl;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;

    const std::string changeKey = "D";
    std::cout << "Changing value of " << changeKey << " using brackets..." << std::endl;
    tree[changeKey] = 1337;
    std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
    std::cout << tree << std::endl;
    std::cout << "Trying to access key not in tree using brackets..." << std::endl;
    try {
        tree["IMPOSTOR"] = 1337;
        std::cout << "The access did not throw an exception! ERROR!" << std::endl;
    }
    catch (std::out_of_range& e) {
        std::cout << "An exception was thrown. Its message is..." << std::endl;
        std::cout << e.what() << std::endl;
    }

    std::cout << "Getting value of key=" << changeKey << " using get..." << std::endl;
    std::optional<size_t> optReturn = tree.get(changeKey);
    if (optReturn.has_value()) std::cout << "Value=" << optReturn.value() << std::endl;
    else std::cout << "get returned nullopt. ERROR!" << std::endl;
    std::cout << "Trying to get value for key not in tree..." << std::endl;
    optReturn = tree.get("IMPOSTOR");
    if (optReturn.has_value()) std::cout << "Value=" << optReturn.value() << ". ERROR!" << std::endl;
    else std::cout << "get returned nullopt. Correct." << std::endl;

    std::cout << "Getting list of keys using keys() method..." << std::endl;
    std::vector<std::string> keyList = tree.keys();
    std::cout << "Keys: ";
    for (const std::string& key : keyList) std::cout << key << ", ";
    std::cout << std::endl;

    std::cout << "Getting list of values in entire tree using findRange() method with range including all keys..." << std::endl;
    std::vector<size_t> list = tree.findRange("@", "[");
    for (const size_t& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    const std::string minKey = "E";
    const std::string maxKey = "H";
    std::cout << "Getting list of values with keys between " << minKey << " and " << maxKey << std::endl;
    list = tree.findRange(minKey, maxKey);
    for (const size_t& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    std::cout << "Testing removal of keys one-by-one..." << std::endl;
    std::vector<std::string> removeList;
    removeList.emplace_back("J");
    removeList.emplace_back("B");
    removeList.emplace_back("F");
    removeList.emplace_back("D");
    removeList.emplace_back("A");
    removeList.emplace_back("E");
    removeList.emplace_back("I");
    removeList.emplace_back("G");
    removeList.emplace_back("C");
    removeList.emplace_back("H");
    for (const std::string& key : removeList) {
        std::cout << "Removing key=" << key << "..." << std::endl;
        tree.remove(key);
        std::cout << "Empty?..." << (tree.empty() ? "true" : "false") << " Size=" << tree.size() << " Height=" << tree.getHeight() << std::endl;
        std::cout << tree << std::endl;
    }

    return 0;
}

int rebalancingTest() {
    std::cout << "Testing AVL-style tree rebalancing." << std::endl;
    std::cout << "Keys from \"A\" to \"Z\" will be inserted one-by-one at random, which will almost certainly require many rotations along the way to rebalance" << std::endl;
    AVLTree<std::string, size_t> tree;
    std::vector<std::string> randInserts;
    for (char c = 65; c < 90; ++c) {
        randInserts.emplace_back(1,c);
    }
    std::mt19937 rngEngine(std::random_device{}());
    std::ranges::shuffle(randInserts, rngEngine);
    for (const std::string& key : randInserts) {
        std::cout << "Inserting key = " << key << "..." << std::endl;
        tree.insert(key, 0);
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
        AVLTree<size_t, std::string> tree;
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


