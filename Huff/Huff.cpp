#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::ofstream;
using std::priority_queue;
using std::sort;
using std::streamoff;
using std::string;
using std::vector;

//TODO:
//End Goal: Create new compressed file with .huf extension
//Structure: Header Metadata, Compressed Data

class HuffNode {
public:
	int glyph;
	int freq;
	int lPtr;
	int rPtr;

	HuffNode() {
		this->glyph = 0;
		this->freq = 0;
		this->lPtr = 0;
		this->rPtr = 0;
	}

	HuffNode(int glyph, int freq) {
		this->glyph = glyph;
		this->freq = freq;
		this->lPtr = -1;
		this->rPtr = -1;
	}

	HuffNode(int glyph, int freq, int lPtr, int rPtr) {
		this->glyph = glyph;
		this->freq = freq;
		this->lPtr = lPtr;
		this->rPtr = rPtr;
	}

	friend std::ostream& operator<<(std::ostream& os, const HuffNode& node) {
		os << "Glyph: " << node.glyph 
			<< ", Freq: " << node.freq 
			<< ", Left Pointer: " << node.lPtr 
			<< ", Right Pointer: " << node.rPtr;
		return os;
	}

	void clearNode() {
		int glyph = 0;
		int freq = 0;
		int lPtr = 0;
		int rPtr = 0;
	}
};

void Reheap(vector<HuffNode>& huffTable, int length);

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
		streamoff fileSize = fin.tellg();
		fin.seekg(0, ios::beg);

		map<int, int> glyphMap;

		for (int i = 0; i < fileSize; i++) {
			char glyph;
			fin >> std::noskipws >> glyph;

			auto result = glyphMap.insert({ (int)glyph, 1});

			if (!result.second) {
				//Increment at result.first
				result.first->second++;
			}
		}

		//print glyph map
		for(auto glyph : glyphMap) {
			cout << glyph.first << " " << glyph.second << endl;

		}

		//Build Initial Huffman Table
		const size_t tableSize = glyphMap.size() + (glyphMap.size() - 1);
		vector<HuffNode> huffTable;

		for (const auto& glyph : glyphMap) {
			huffTable.push_back(HuffNode(glyph.first, glyph.second));
		}

		sort(huffTable.begin(), huffTable.end(), [](HuffNode a, HuffNode b) {
				return a.freq < b.freq;
			});

		//Build Huffman Table
		int M = 0;
		int H = glyphMap.size() -1;
		int F = huffTable.size();


		do {
			//M = lower freq of indx 1 and 2
			if (huffTable[1].freq <= huffTable[2].freq) {
				M = 1;
			}
			else {
				M = 2;
			}

			//Move M to F (push_back)
			huffTable.push_back(huffTable[M]);
			huffTable[M].clearNode();

			//If M < H; Move H to M
			if (M < H) {
				huffTable[M] = huffTable[H];
				huffTable[H].clearNode();
			}
			//Reheap if Necessary
			Reheap(huffTable, H);
			//Move indx 0 to H
			huffTable[H] = huffTable[0];
			huffTable[0].clearNode();
			//Create Freq Node,Glyph = -1, Freq = H Freq + F Freq, lPtr = H, rPtr = F
			huffTable[0] = HuffNode(-1, huffTable[H].freq + huffTable[F].freq, H, F);
			//Reheap if Necessary
			Reheap(huffTable, H);
			//Decrement H
			H--;
			//Increment F
			F++;

			if (huffTable[1].freq <= huffTable[2].freq) {
				M = 1;
			}
			else {
				M = 2;
			}
		} while (M >= H);
		
		for (const auto& huffNode : huffTable) {
			cout << huffNode << endl;
		}
	}
	else {
		cout << "Failed to connect to file: " << fileName << endl;
		cout << "Deleting System32..." << endl;
	}

	return 0;
}

void Reheap(vector<HuffNode>& huffTable, int length) {
	//Left 2i + 1
	//Right 2i + 2
	bool didChange;

	do {
		didChange = false;

		for (int i = 0; i < length; i++) {
			if (huffTable[i].freq < huffTable[2 * i + 1].freq) {
				HuffNode temp = huffTable[i];
				huffTable[i] = huffTable[2 * i + 1];
				huffTable[2 * i + 1] = temp;
				didChange = true;
			}
			if (huffTable[i].freq < huffTable[2 * i + 2].freq) {
				HuffNode temp = huffTable[i];
				huffTable[i] = huffTable[2 * i + 2];
				huffTable[2 * i + 1] = temp;
				didChange = true;
			}
			else {
				break;
			}
		}
	} while (didChange);
}



//Generate Huffman Codes
void huffCodes()
{
	//make a min heap
	//comparison operator for huffs
	priority_queue<> leafNodes;

}

