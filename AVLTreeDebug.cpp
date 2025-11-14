/*
Driver code for testing your AVL Tree
This is NOT the test code for grading,
instead for you to get an idea of how to test the tree
 */
#include "AVLTree.h"
#include <iostream>
#include <random>
#include <string>
#include <ranges>
#include <vector>
#include <algorithm>
using namespace std;

int main() {

    AVLTree tree;
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    std::vector<size_t> randInserts;
    for (int i=0; i<20; ++i) {
        randInserts.push_back(i);
    }
    std::mt19937 rngEngine(std::random_device{}());
    ranges::shuffle(randInserts, rngEngine);
    for (size_t val : randInserts) {
        tree.insert(val, "");
    }
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

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
    std::cout << anotherTree.empty() << " " << anotherTree.getHeight() << " " << anotherTree.size() << std::endl;
    std::cout << anotherTree << std::endl;

    tree = anotherTree;
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    anotherTree.clear();
    std::cout << anotherTree.empty() << " " << anotherTree.getHeight() << " " << anotherTree.size() << std::endl;
    std::cout << anotherTree << std::endl;

    tree.insert(8, "IMPOSTOR!");
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree[3] = "blarg";
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;
    tree[3] = "three";

    std::optional<AVLTree::ValueType> optReturn = tree.get(3);
    if (optReturn.has_value()) std::cout << optReturn.value() << std::endl;
    else std::cout << "Not in tree" << std::endl;
    optReturn = tree.get(11);
    if (optReturn.has_value()) std::cout << optReturn.value() << std::endl;
    else std::cout << "Not in tree" << std::endl;

    std::vector<AVLTree::KeyType> keyList = tree.keys();
    for (const AVLTree::KeyType& key : keyList) std::cout << key << ", ";
    std::cout << std::endl;

    std::vector<AVLTree::ValueType> list = tree.findRange(0,15);
    for (const AVLTree::ValueType& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    list = tree.findRange(4,7);
    for (const AVLTree::ValueType& value : list) std::cout << value << ", ";
    std::cout << std::endl;

    tree.remove(9);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(1);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(5);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(3);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(0);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(4);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(8);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(11);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(6);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(2);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;

    tree.remove(7);
    std::cout << tree.empty() << " " << tree.getHeight() << " " << tree.size() << std::endl;
    std::cout << tree << std::endl;





    // AVLTree tree;
    // bool insertResult;
    // insertResult = tree.insert("F", 'F');
    // insertResult = tree.insert("F", 'F'); // false, no duplicates allowed
    // insertResult = tree.insert("K", 'K');
    // insertResult = tree.insert("X", 'X');// single rotate left
    // cout << endl << endl;
    // cout << tree << endl;
    //
    // insertResult = tree.insert("C", 'C');
    // insertResult = tree.insert("A", 'A'); // single rotate right
    // cout << endl << endl;
    // cout << tree << endl;
    //
    // insertResult = tree.insert("D", 'D'); // double rotate right
    // cout << endl << endl;
    // cout << tree << endl;
    //
    // insertResult = tree.insert("R", 'R'); // double rotate left
    // cout << endl << endl;
    // cout << tree << endl;
    //
    // insertResult = tree.insert("V", 'V');
    // insertResult = tree.insert("A", 'A'); // false, duplicate
    // insertResult = tree.insert("Z", 'Z');
    // insertResult = tree.insert("M", 'M');
    // insertResult = tree.insert("D", 'D'); // false, duplicate
    // cout << endl << endl;
    // cout << tree << endl;
    //
    // // size and getHeight
    // cout << "tree size: " << tree.size() << endl; // 10
    // cout << "tree height: " << tree.getHeight() << endl; // 3
    // cout << endl;
//
//    // contains
//    bool containsResult;
//    containsResult = tree.contains("A"); // true
//    containsResult = tree.contains("N"); // false
//
//    // get
//    optional<int> getResult;
//
//    getResult = tree.get("A"); // 65
//    cout << "A: " << getResult.value() << endl;
//
//    getResult = tree.get("C"); // 67
//    cout << "C: " << getResult.value() << endl;
//
//    getResult = tree.get("Q"); // getResult has no value
//    cout << "Q: " << getResult.has_value() << endl; // print 0
//    cout << endl;
//
//    // findRange
//    vector<int> rangeTest = tree.findRange("D", "W");
//    // 70 68 82 75 77 86
//    for (auto val: rangeTest) {
//        cout << val << " ";
//    }
//    cout << endl << endl;
//
//    // operator[]
//    tree["A"] = 108;
//    cout << tree << endl;
//    cout << endl;
//
//    // remove
//    bool removeResult;
//    removeResult= tree.remove("A"); // "A" is a leaf
//    cout << endl << endl;
//    cout << tree << endl;
//
//    removeResult = tree.remove("C"); // "C" has one child, single rotate left
//    cout << endl << endl;
//    cout << tree << endl;
//
//    removeResult = tree.remove("F"); // "F" has two children
//    cout << endl << endl;
//    cout << tree << endl;
//
//    removeResult = tree.remove("V");
//    removeResult = tree.remove("X");
//    removeResult = tree.remove("Z"); // double rotate right
//    cout << endl << endl;
//    cout << tree << endl;

    return 0;
}
