#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <queue>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ios;
using std::map;
using std::unordered_map;
using std::ofstream;
using std::priority_queue;
using std::pair;
using std::sort;
using std::streamoff;
using std::string;
using std::vector;

//TODO:
//End Goal: Create new compressed file with .huf extension
//Structure: Header Metadata, Compressed Data

const int EOFVALUE = 256;

class HuffNode {
public:
	unsigned char glyph;
	unsigned int freq;
	HuffNode* lPtr;
	HuffNode* rPtr;

	HuffNode() {
		this->glyph = NULL;
		this->freq = NULL;
		this->lPtr = nullptr;
		this->rPtr = nullptr;
	}

	HuffNode(unsigned char glyph, int freq) {
		this->glyph = glyph;
		this->freq = freq;
		this->lPtr = nullptr;
		this->rPtr = nullptr;
	}

	HuffNode(unsigned char glyph, int freq, HuffNode* lPtr, HuffNode* rPtr) {
		this->glyph = glyph;
		this->freq = freq;
		this->lPtr = lPtr;
		this->rPtr = rPtr;
	}

	friend std::ostream& operator<<(std::ostream& os, const HuffNode& node) {
		os << node.glyph 
		   << node.lPtr
		   << node.rPtr;
		return os;
	}

	void clearNode() {
		unsigned char glyph = 0;
		int freq = 0;
		HuffNode* lPtr = nullptr;
		HuffNode* rPtr = nullptr;
	}

};


struct compNodes
{
	bool operator()(HuffNode* l, HuffNode* r)
	{
		return l->freq > r->freq;
	}

};


void Reheap(vector<HuffNode>& huffTable, int length);

void GenerateCodes(HuffNode* root, string code, unordered_map<unsigned char, string>& huffTable) {
	if (root == nullptr)
	{
		return;
	}

	if (!root->lPtr && !root->rPtr)
	{
		huffTable[root->glyph] = code;
	}

	GenerateCodes(root->lPtr, code + "0", huffTable);
	GenerateCodes(root->rPtr, code + "1", huffTable);

}


unordered_map<unsigned char, string> BuildHuffTable(map<unsigned char, unsigned int>& freqTable, priority_queue<HuffNode*, vector<HuffNode*>, compNodes>& minHeap)
{
	for (auto glyph : freqTable)
	{
		minHeap.push(new HuffNode(glyph.first, glyph.second));
	}

	while (minHeap.size() != 1) {
		HuffNode* left = minHeap.top();
		minHeap.pop();

		HuffNode* right = minHeap.top();
		minHeap.pop();

		HuffNode* newNode = new HuffNode('-1', left->freq + right->freq);
		newNode->lPtr = left;
		newNode->rPtr = right;
		minHeap.push(newNode);
	}

	//add to a vector and pass to output
	//remove copy

	priority_queue<HuffNode*, vector<HuffNode*>, compNodes> minHeap2 = minHeap;

	unordered_map<unsigned char, string> huffTable;
	GenerateCodes(minHeap2.top(), "", huffTable);

	return huffTable;
}




int main() {

	string fileName;
	ifstream fin;
	ofstream foust;
	
	cout << "Name of file you want to Huff: ";
	cin >> fileName;
	
	fin.open(fileName, ios::in | ios::binary);

	if (fin.is_open()) {

		cout << "Connected to file: " << fileName << endl;;

		fin.seekg(0, ios::end);
		streamoff fileSize = fin.tellg();
		fin.seekg(0, ios::beg);

		map<unsigned char, unsigned int> glyphMap;

		unsigned char glyph;
		while (fin.read(reinterpret_cast<char*>(&glyph), sizeof(unsigned char)))
		{
			auto result = glyphMap.insert({ glyph, 1 });

			if (!result.second) {
				//Increment at result.first
				result.first->second++;
			}
		}

		glyphMap.insert({ EOFVALUE, 1 });

		//print glyph map
		for(auto glyph : glyphMap) {
			cout << glyph.first << " " << glyph.second << endl;

		}

		//Build Initial Huffman Table
		const size_t tableSize = glyphMap.size() + (glyphMap.size() - 1);
		//vector<HuffNode> huffTable;

	/*	for (const auto& glyph : glyphMap) {
			huffTable.push_back(HuffNode(glyph.first, glyph.second));
		}

		sort(huffTable.begin(), huffTable.end(), [](HuffNode a, HuffNode b) {
				return a.freq < b.freq;
			});*/


		//make a huff tree
		priority_queue<HuffNode*, vector<HuffNode*>, compNodes> minHeap;

		unordered_map<unsigned char, string> huffTable = BuildHuffTable(glyphMap, minHeap);


		/* COMMENTED OUT TO ATTEMPT A NEW METHOD OF HUFFING
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
			huffTable[M] = HuffNode();

			//If M < H; Move H to M
			if (M < H) {
				huffTable[M] = huffTable[H];
				huffTable[H] = HuffNode();
			}
			//Reheap if Necessary
			Reheap(huffTable, H);
			//Move indx 0 to H
			huffTable[H] = huffTable[0];
			huffTable[0] = HuffNode();
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
		} while (H >= M);
		
		for (const auto& huffNode : huffTable) {
			cout << huffNode << endl;
		}
		*/





		//find the file extension in fileName and remove it
		size_t periodLoc = fileName.find('.');
		string hufName;

		if (periodLoc != string::npos)
		{
			hufName = fileName.substr(0, periodLoc) + ".huf";
		}
		else
		{
			hufName = fileName + ".huf";
		}
		
		//open .huf file for output
		foust.open(hufName, ios::out | ios::binary);
		if (!foust.is_open())
		{
			cerr << "ERROR OPENING .huf FILE." << endl << endl << "DELETING SYSTEM32..." << endl;
			EXIT_FAILURE;
		}

		//write the file name size and then the file name to the .huf file
	
		int fileNameSize = fileName.size();
		foust.write(reinterpret_cast<const char*>(&fileNameSize), sizeof(fileNameSize));
		foust.write(fileName.c_str(), fileNameSize);

		//output the Huffman table size
		int huffTableSize = huffTable.size();
		foust.write(reinterpret_cast<const char*>(&huffTableSize), sizeof(huffTableSize));

		//output the Huffman table
		while (!minHeap.empty())
		{
			auto huffNode = minHeap.top();
			minHeap.pop();
			foust.write(reinterpret_cast<const char*>(huffNode), sizeof(huffNode));
		}
		



	}
	else {
		cout << "Failed to connect to file: " << fileName << endl;
		cout << "Deleting System32..." << endl;
		EXIT_FAILURE;
	}


	return 0;
}

void Reheap(vector<HuffNode>& huffTable, int length) {
	//Left 2i + 1
	//Right 2i + 2
	bool didChange;

	do {
		didChange = false;

		for (int i = 0; (2 * i + 2) < length; i++) {
			if (huffTable[i].freq > huffTable[2 * i + 1].freq && huffTable[i].freq != 0 && huffTable[2 * i + 1].freq != 0) {
				HuffNode temp = huffTable[i];
				huffTable[i] = huffTable[2 * i + 1];
				huffTable[2 * i + 1] = temp;
				didChange = true;
			}
			if (huffTable[i].freq > huffTable[2 * i + 2].freq && huffTable[i].freq != 0 && huffTable[2 * i + 2].freq != 0) {
				HuffNode temp = huffTable[i];
				huffTable[i] = huffTable[2 * i + 2];
				huffTable[2 * i + 2] = temp;
				didChange = true;
			}
		}
	} while (didChange);
}



//Generate Huffman Codes
void huffCodes()
{
	//make a min heap
	//comparison operator for huffs
	//priority_queue<> leafNodes;

}

