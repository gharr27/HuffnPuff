#include <iostream>
#include <fstream>
#include <map>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::ofstream;
using std::streampos;
using std::string;

//TODO:
//End Goal: Create new compressed file with .huf extension
//Structure: Header Metadata, Compressed Data

int main() {

	string fileName;
	ifstream fin;
	ofstream foust;
	
	cout << "Name of file you want to Huff: ";
	cin >> fileName;
	
	fin.open(fileName, ios::in);

	if (fin.is_open()) {

		cout << "Connected to file: " << fileName << endl;;

		fin.seekg(0, ios::end);
		int fileSize = fin.tellg();
		fin.seekg(0, ios::beg);

		map<char, int> glyphMap;

		for (int i = 0; i < fileSize; i++) {
			char glyph;
			fin >> std::noskipws >> glyph;

			auto result = glyphMap.insert({ glyph, 1 });

			if (!result.second) {
				//Increment at result.first
				result.first->second++;
			}
		}

		for(auto glyph : glyphMap) {
			cout << glyph.first << " " << glyph.second << endl;

		}
	}
	else {
		//Delete System32
		cout << "Failed to connect to file: " << fileName << endl;
		cout << "Deleting System32..." << endl;
	}

	return 0;
}


