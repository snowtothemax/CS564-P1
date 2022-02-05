#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <fstream>

#include "wl.h"

using namespace std;

/*
	Node Class Decl.
*/

// Default Constructor
Node::Node() : data('-'),
			   endOfWord(false)
{
}

// Construct Node with data val
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
		return 36;
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

/*
Returns lower case string of input str
*/
string toLowerCase(string str)
{
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
			  { return std::tolower(c); });

	return str;
}

/***********************
	Main Method
*************************/
void driver()
{
	// First want to accept input
	string command;

	Node *root;

	while (1)
	{
		cout << '>';
		getline(cin, command);

		stringstream ss(command);
		string word;

		/////// First Command Check /////////
		if (!(ss >> word))
		{
			cout << "ERROR: Invalid command" << endl;
			continue;
		}
		word = toLowerCase(word);

		if (word == "load")
		{
			// ERROR
			//there is not enough arguments
			if (!(ss >> word))
			{
				cout << "ERROR: Invalid command" << endl;
				continue;
			}
			else
			{
				// ERROR too many args
				// test if too many input
				string testWord;
				if (ss >> testWord)
				{
					cout << "ERROR: Invalid command" << endl;
					continue;
				}
				else
				{
					string line;
					ifstream myfile(word);

					// TEST if file is valid or invalid
					if (myfile.is_open())
					{
						// Clear data structure
						root = new Node();

						int wordCount = 0;
						while (getline(myfile, line))
						{
							stringstream stream(line);
							string wordToAdd;

							// Add Each word
							while (stream >> wordToAdd)
							{
								wordCount++;
								// ignore all but apostrophe
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '!'), wordToAdd.end());
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '?'), wordToAdd.end());
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '.'), wordToAdd.end());
								wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), ','), wordToAdd.end());
								wordToAdd = toLowerCase(wordToAdd);

								InsertWord(wordToAdd, wordCount, root);
							}
						}
						myfile.close();
					}
					else
					{
						cout << "ERROR: Invalid command" << endl;
						continue;
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
				cout << wordToLocate << endl;

				int wordOccurrence;

				// get the occurrence
				if (ss >> wordOccurrence)
				{
					// ERROR
					// Too many args
					string testWord;
					if (!(ss >> testWord))
					{
						cout << "ERROR: Invalid command" << endl;
						continue;
					}
					// Valid Command
					else
					{
						// Searches for the word and if the returned number is -1, output not found
						int wordNumber = SearchWord(wordToLocate, wordOccurrence, root);

						if (wordNumber != -1)
						{
							cout << wordNumber << endl;
						}
						else
						{
							cout << "No entry found" << endl;
							continue;
						}
					}
				}
				else
				{
					cout << "ERROR: Invalid command" << endl;
					continue;
				}
			}
			else
			{
				cout << "ERROR: Invalid command" << endl;
				continue;
			}
		}
		else if (word == "new")
		{
			// test if valid comand
			string testWord;
			if (ss >> testWord)
			{
				cout << "ERROR: Invalid command" << endl;
				continue;
			}
			else
			{
				// reset data structure
				root = new Node();
			}
		}
		else if (word == "end")
		{
			// end program
			break;
		}
		else
		{
			cout << "ERROR: Invalid command" << endl;
			continue;
		}
	}
}

void test_insertWithSomeLocate()
{
	Node *root;
	string word = "sixpence.txt";
	string line;
	ifstream myfile(word);

	// TEST if file is valid or invalid
	if (myfile.is_open())
	{
		// Clear data structure
		root = new Node();

		int wordCount = 0;
		while (getline(myfile, line))
		{
			stringstream stream(line);
			string wordToAdd;

			// Add Each word
			while (stream >> wordToAdd)
			{
				wordCount++;
				// ignore all but apostrophe
				wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '!'), wordToAdd.end());
				wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '?'), wordToAdd.end());
				wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), '.'), wordToAdd.end());
				wordToAdd.erase(remove(wordToAdd.begin(), wordToAdd.end(), ','), wordToAdd.end());
				wordToAdd = toLowerCase(wordToAdd);

				InsertWord(wordToAdd, wordCount, root);
			}
		}
		myfile.close();
	}

	int num = SearchWord("song", 1, root);
	cout << num << endl;
	num = SearchWord("pie", 1, root);
	cout << num << endl;
	num = SearchWord("pie", 2, root);
	cout << num << endl;
	num = SearchWord("pie", 3, root);
	cout << num;
}

int main()
{
	driver();
}
