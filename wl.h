#include <iostream>
#include <array>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <memory>

#ifndef WL_H
#define WL_H

const int CHILD_SIZE = 37;

/* *****************************
	Trie Node
**********************************/
class Node
{

public:
    char data;                                    // the letter
    bool endOfWord;                               // indicates if is end of a word
    std::map<int, int> occurrNumFileNumMap;       // map of occurrence to Word Num
    array<shared_ptr<Node>, CHILD_SIZE> children; // the children of the node

    Node();

    Node(char val);

    virtual ~Node();
};

#endif