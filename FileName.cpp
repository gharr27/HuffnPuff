#include <iostream>
#include <queue>
#include <unordered_map>

struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char sym, int freq) : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}
};

struct CompareNodes {
    bool operator()(HuffmanNode* lhs, HuffmanNode* rhs) {
        return lhs->frequency > rhs->frequency;
    }
};

void generateHuffmanCodesHelper(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanTable);

void generateHuffmanCodes(const std::unordered_map<char, int>& symbolFrequencies, std::unordered_map<char, std::string>& huffmanTable) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

    // Populate the priority queue with leaf nodes
    for (const auto& pair : symbolFrequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();

        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* internalNode = new HuffmanNode('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        pq.push(internalNode);
    }

    // Generate Huffman codes by traversing the tree
    if (!pq.empty()) {
        generateHuffmanCodesHelper(pq.top(), "", huffmanTable);
    }

    // Clean up memory
    while (!pq.empty()) {
        delete pq.top();
        pq.pop();
    }
}

void generateHuffmanCodesHelper(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanTable) {
    if (root) {
        if (root->symbol != '$') {
            huffmanTable[root->symbol] = code;
        }

        generateHuffmanCodesHelper(root->left, code + "0", huffmanTable);
        generateHuffmanCodesHelper(root->right, code + "1", huffmanTable);
    }
}

int main() {
    // Define symbols and their frequencies
    std::unordered_map<char, int> symbolFrequencies = {
        {'A', 45},
        {'B', 13},
        {'C', 12},
        {'D', 16},
        {'E', 9},
        {'F', 5}
    };

    // Create a Huffman table
    std::unordered_map<char, std::string> huffmanTable;
    generateHuffmanCodes(symbolFrequencies, huffmanTable);

    // Display the Huffman codes
    for (const auto& pair : huffmanTable) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    return 0;
}
