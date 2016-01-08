//Garrett Rudolph
//CS-318-01
//11-20-15
//Use STL containers to read dictionary from file, sort words by length, 
//	and find the adjacent word for every word in the provided dicitionary

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <map>
#include <Windows.h>
using namespace std;


void fileRead(vector<string>& dictionary) {	//Read dictionary from file
	ifstream infile;
	string fileString = "?";
	set<string> adjSet;

	infile.open("//elm-file01/citrixhome$/e0577578/Visual Studio 2013/Projects/P4318Rudolph/P4318Rudolph/dctnryWords.txt");
	while (infile >> ws && !infile.eof()) {
		getline(infile, fileString);
		dictionary.push_back(fileString);	//dictionary already sorted & w/o duplicates, 
	}										//does not need to be sorted at this point,
	//so use vector for quick insert

	infile.close();

	return;
}

void findLengths(vector<string>& dictionary, array<vector<string>, 20>& lengthArray) {
	for (size_t i = 0; i < dictionary.size(); ++i) {
		lengthArray[dictionary.at(i).size() - 1].push_back(dictionary.at(i));	//still no need to be sorted,
	}																			//so use vectors for quick access/insert

	cout << "The words-only dictionary has a following distribution of word lengths..." << endl;
	for (int i = 0; i < 20; ++i) {
		cout << "The number of words of length " << i + 1 << " = " << lengthArray[i].size() << endl;
	}
	return;
}

void findAdj(const size_t MAX_LENGTH, array<vector<string>, 20>& lengthArray, map<string, set<string>>& adjMap) {
	string word;
	string subWord;

	for (size_t x = 0; x < MAX_LENGTH; ++x) {									//only grab words of same length
		const vector<string>& lengthVector = lengthArray[x];
		size_t length = x + 1;													//array[3] = vector of words of length 4, etc.
		map<pair<int, string>, set<string>> subMap;								//create temp map of all subsets of words,
		for (auto& str : lengthVector) {										//	uses pairs to avoid improper matches (king, bing, ings)
			for (size_t i = 0; i < length; ++i) {
				word = str;
				subWord = str;
				subWord.erase(i, 1);
				subMap[make_pair(i, subWord)].insert(word);						//insert word into subset's container
			}
		}

		for (auto& mapPtr : subMap) {											//loop thru temp map & fill in adjacency sets
			for (auto& wordKey : mapPtr.second) {								//grab base word to use as map 'key'
				for (auto& wordValue : mapPtr.second) {							//grab adjacent word to fill in base word's set
					if (mapPtr.second.size() != 0 && wordKey != wordValue) {	//	(only creates keys & fills sets 
						adjMap[wordKey].insert(wordValue);						//		for words with at least one adjacenct)
					}
				}
			}
		}
	}
	return;
}

void printAdj(const map<string, set<string>>& adjMap, const size_t numAdj) {
	for (map<string, set<string>>::const_iterator itr1 = adjMap.begin(); itr1 != adjMap.end(); ++itr1) {
		if (itr1->second.size() >= numAdj) {
			cout << itr1->first << "(" << itr1->second.size() << "): ";
			for (set<string>::iterator itr2 = itr1->second.begin(); itr2 != itr1->second.end(); ++itr2) {
				cout << *itr2 << " ";
			}
			cout << endl;
		}
	}
	return;
}

template <class Function>
__int64 time_call(Function&& f)			//provided function used to time function calls
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

int main() {
	const size_t MAX_LENGTH = 20;		//max length encountered in provided dictionary
	vector<string> dictionary;
	array<vector<string>, MAX_LENGTH> lengthArray;
	map<string, set<string>> adjMap;	//sets used in final map container to ensure alphabetically ordering
	size_t numAdj = 0;

	//	1. Prompt the user for N
	cout << "Please enter N: ";

	//	2. Read N
	cin >> numAdj;

	//	3. Read the dictionary words into a structure in RAM and time this task
	__int64 elapsed1 = time_call([&] { fileRead(dictionary); });

	//	4. Display the time it takes to read the dictionary into RAM
	cout << "Loading words into RAM took: " << elapsed1 << " ms.\n";

	//	 5. Compute the distribution of words from the dictionary according to their length...
	//	 6. ... Display the distributions.
	findLengths(dictionary, lengthArray);

	//	 7. Compute and print the words, the respective number of adjacent words, and respective adjacent words.
	_int64 elapsed3 = time_call([&] { findAdj(MAX_LENGTH, lengthArray, adjMap); });

	printAdj(adjMap, numAdj);

	//	 8. Display the time it takes to compute all words and respective adjacent words.
	cout << "Finding adjacent words took: " << elapsed3 << " ms.\n";

	return 0;
}