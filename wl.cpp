#include <iostream>
#include <stdint>
#include <stdio>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "wl.h"

using namespace std;

const int CHILD_SIZE = 37;

/* *****************************
	Trie Node
**********************************/
class Node
{

public:
	char data;								// the letter
	bool endOfWord;							// indicates if is end of a word
	std::map<int, int> occurrNumFileNumMap; // map of occurrence to Word Num
	Node *children[CHILD_SIZE];				// the children of the node

	Node();

	Node(char val);
};

Node::Node() : data('-'),
			   endOfWord(false)
{
}

Node::Node(char val) : data(val),
					   endOfWord(false) {}

/*******************************************************
	Gets the encoded index for any acceptable character.
********************************************************/
int getChildIndex(char key)
{
	if (key >= 'a' && key <= 'z')
	{
		return key - 'a';
	}
	else if (key >= '0' && key <= '9')
	{
		return key - '0' + 26;
	}
	else
	{
		return 36
	}
}

/******************************
* Insert word into the Trie
*******************************/
void InsertWord(string word, int wordNum, Node *node)
{
	Node *crawl = node;

	for (int i = 0; i < word.length(); i++)
	{
		int index = getChildIndex(word.at(i));

		if (!crawl->children[index])
		{
			crawl->children[index] = new Node(word.at(i));
		}

		crawl = crawl->children[index];
	}
	int mapSize = crawl->occurrNumFileNumMap.size();

	crawl->endOfWord = true;
	crawl->occurrNumFileNumMap.insert(pair<int, int>(mapSize + 1, wordNum));
}

/*************************************
	Searches for the word in the Trie
**************************************/
int SearchWord(string word, int occurrence, Node *node)
{
	Node *crawl = node;

	for (int i = 0; i < word.length(); i++)
	{
		int index = getChildIndex(word.at(i));

		if (!crawl->children[index])
		{
			return -1;
		}

		crawl = crawl->children[index];
	}

	return crawl->occurrNumFileNumMap[occurrence];
}

// Helper for making strings lower case
string toLowerCase(string str)
{
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
			  { return std::tolower(c); }););

	return str
}

enum Commands
{
	LOAD,
	LOCATE,
	NEW,
	END
}

// Main Method
int
main()
{
	// First want to accept input
	string command;

	Node root = new Node();

	while (1)
	{
		cout << '>';
		cin >> command;

		stringstream ss(command);
		string word;

		/////// First Command Check /////////
		ss >> word;
		word = toLowerCase(word);

		if (word == "load")
		{
			//there is not enough arguments
			if (!(ss >> word))
			{
				cout << "ERROR: Invalid command" << endl;
			}
			else
			{
				// test if too many input
				string testWord;
				if (ss >> testWord)
				{
					cout << "ERROR: Invalid command" << endl;
				}
				else
				{
					string line;
					ifstream myfile(word);

					if (myfile.is_open())
					{
						int wordCount = 0;
						while (getline(myfile, line))
						{
							stringstream stream(line);
							string wordToAdd;

							// Add Each word
							while (ss >> wordToAdd)
							{
								wordCount++;
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '!'), wordToAdd.end());
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '?'), wordToAdd.end());
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '.'), wordToAdd.end());
								wordToAdd = toLowerCase(wordToAdd);

								InsertWord(wordToAdd, wordCount, root)
							}
						}
						myfile.close();
					}
					else
					{
						cout << "ERROR: Invalid command" << endl;
					}
				}
			}
		}
		else if (word == "locate")
		{
			string wordToLocate;
			// get the word

			if (ss >> wordToLocate)
			{
				int wordOccurrence;
				if (ss >> wordOccurrence)
				{
					string testWord;
					if (!(ss >> testWord))
					{
						cout << "ERROR: Invalid command" << endl;
					}
					else
					{
						int wordNumber = SearchWord(wordToLocate, wordOccurrence, root);

						if (wordNumber != -1)
						{
							cout << wordNumber << endl;
						}
						else
						{
							cout << "No entry found" << endl;
						}
					}
				}
				else
				{
					cout << "ERROR: Invalid command" << endl;
				}
			}
			else
			{
				cout << "ERROR: Invalid command" << endl;
			}
		}
		else if (word == "new")
		{
		}
		else if (word == "end")
		{
		}
		else
		{
			cout << "ERROR: Invalid command" << endl;
		}
	}
}
