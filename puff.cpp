/*
puff.cpp
Authors: Avery Hawkins and Carter Jones
Date: November 16, 2023
Description: This program decompresses a file 
according to the huffman algorithm.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <ctime>

using namespace std;


const int END_OF_FILE = 256;

struct Entry {
    int glyph;
    int left;
    int right;
};


int main()
{
	string filename;

	//1. OPEN FILE
	//Get the name of the file and open it
	cout << "Name of file: ";
	cin >> filename;

	//Start the clock
	clock_t start, end;
	start = clock();

	ifstream fin(filename, ios::binary);
	if (fin.fail()) {
		cout << "File not found" << endl;
		return 0;
	}



	//2. READ THE FILE
	//Read the length of the file name
	int nameLength = 0;
	fin.read((char*)&nameLength, sizeof nameLength);

	//Read the file name
	char* name;
	name = new char[nameLength + 1];
	fin.read(name, nameLength);
	name[nameLength] = '\0';


	//Read the number of huffman table entries
	int numEntries = 0;
	fin.read((char*)&numEntries, sizeof numEntries);

	//Read each huffman table entry and store the entries in a vector
	vector<Entry> huffmanTable;
	for (int i = 0; i < numEntries; i++) {
		Entry e;
		fin.read((char*)&e.glyph, sizeof e.glyph);
		fin.read((char*)&e.left, sizeof e.left);
		fin.read((char*)&e.right, sizeof e.right);
		huffmanTable.push_back(e);
	}


	//Read the compressed data as hex numbers and store them in a vector
	vector<int> hexNums;
	ifstream::pos_type size;
	char hexNum;
	while (fin.read(reinterpret_cast<char*>(&hexNum), sizeof(hexNum))) {
		hexNums.push_back(hexNum);
	}

	fin.close();

	

	//3. DECOMPRESS THE DATA

	//Convert hex numbers to to binary and create one string of all the
	// encoded binary data
	string encoded = "";
	for (int i = hexNums.size() - 1; i >= 0; i--) {
		bitset<8> binNum(hexNums[i]);
		encoded.append(binNum.to_string());
	}
	reverse(encoded.begin(), encoded.end());


	//Create the output file
	ofstream fout(name, ios::binary);

	if (fout.fail()) {
		cout << "File not created" << endl;
		return 0;
	}

	//Traverse the tree until a glyph is found and then
	// output the glyph to the output file
	
	bool eofFound = false;
	
	struct Entry curr = huffmanTable.at(0);

	

	for (int i = 0; i < encoded.size(); i++) {
		if (!eofFound) {
			if (encoded[i] == '0') {
				curr = huffmanTable.at(curr.left);
			}
			else {
				curr = huffmanTable.at(curr.right);
			}

			if (curr.left == -1 && curr.right == -1) {
				if (curr.glyph == END_OF_FILE) {
					eofFound = true;
				}
				else {
					char g = (char)curr.glyph;
					fout.write((char*)&g, sizeof g);
					curr = huffmanTable.at(0);
				}
			}
		}
	}

	fout.close();
	
	//Stop the clock
	end = clock();

	//Output the time it took to run the program
	cout << "The time was " << (double(end - start) / CLOCKS_PER_SEC)
		<< "seconds." << endl;
	
}