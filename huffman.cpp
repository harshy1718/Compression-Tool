#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

struct Node {
    char ch;
    int freq;
    Node *left, *right;

    Node(char ch, int freq, Node* left = nullptr, Node* right = nullptr) 
        : ch(ch), freq(freq), left(left), right(right) {}
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class HuffmanCoding {
public:
    Node* root;

    HuffmanCoding() : root(nullptr) {}

    void buildHuffmanTree(const std::string& text) {
        std::unordered_map<char, int> freq;
        for (char ch : text) {
            freq[ch]++;
        }

        std::priority_queue<Node*, std::vector<Node*>, compare> pq;
        for (auto pair : freq) {
            pq.push(new Node(pair.first, pair.second));
        }

        while (pq.size() != 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            int sum = left->freq + right->freq;
            pq.push(new Node('\0', sum, left, right));
        }

        root = pq.top();
    }

    void buildHuffmanCode(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
        if (root == nullptr)
            return;

        if (!root->left && !root->right) {
            huffmanCode[root->ch] = str;
        }

        buildHuffmanCode(root->left, str + "0", huffmanCode);
        buildHuffmanCode(root->right, str + "1", huffmanCode);
    }

    void encode(const std::string& text, std::string& encodedStr, std::unordered_map<char, std::string>& huffmanCode) {
        buildHuffmanTree(text);
        buildHuffmanCode(root, "", huffmanCode);

        for (char ch : text) {
            encodedStr += huffmanCode[ch];
        }
    }

    std::string decode(const std::string& encodedStr, Node* root) {
        std::string decodedStr;
        Node* current = root;
        for (int i = 0; i < encodedStr.size(); i++) {
            current = encodedStr[i] == '0' ? current->left : current->right;

            if (!current->left && !current->right) {
                decodedStr += current->ch;
                current = root;
            }
        }
        return decodedStr;
    }

    void saveToFile(const std::string& filename, const std::string& encodedStr, const std::unordered_map<char, std::string>& huffmanCode) {
        std::ofstream outFile(filename, std::ios::binary);

        if (!outFile.is_open()) {
            std::cerr << "Could not open the file for writing: " << filename << std::endl;
            return;
        }

        size_t mapSize = huffmanCode.size();
        outFile.write((char*)&mapSize, sizeof(mapSize));

        for (const auto& pair : huffmanCode) {
            outFile.write((char*)&pair.first, sizeof(pair.first));
            size_t strSize = pair.second.size();
            outFile.write((char*)&strSize, sizeof(strSize));
            outFile.write(pair.second.c_str(), strSize);
        }

        size_t encodedSize = encodedStr.size();
        outFile.write((char*)&encodedSize, sizeof(encodedSize));
        outFile.write(encodedStr.c_str(), encodedSize);

        outFile.close();
    }

    void loadFromFile(const std::string& filename, std::string& encodedStr, std::unordered_map<char, std::string>& huffmanCode) {
        std::ifstream inFile(filename, std::ios::binary);

        if (!inFile.is_open()) {
            std::cerr << "Could not open the file for reading: " << filename << std::endl;
            return;
        }

        size_t mapSize;
        inFile.read((char*)&mapSize, sizeof(mapSize));

        for (size_t i = 0; i < mapSize; ++i) {
            char ch;
            size_t strSize;
            inFile.read((char*)&ch, sizeof(ch));
            inFile.read((char*)&strSize, sizeof(strSize));

            std::string str(strSize, ' ');
            inFile.read(&str[0], strSize);

            huffmanCode[ch] = str;
        }

        size_t encodedSize;
        inFile.read((char*)&encodedSize, sizeof(encodedSize));

        encodedStr.resize(encodedSize);
        inFile.read(&encodedStr[0], encodedSize);

        inFile.close();
    }

    Node* rebuildTree(const std::unordered_map<char, std::string>& huffmanCode) {
        Node* root = new Node('\0', 0);
        for (const auto& pair : huffmanCode) {
            Node* current = root;
            for (char bit : pair.second) {
                if (bit == '0') {
                    if (!current->left) current->left = new Node('\0', 0);
                    current = current->left;
                } else {
                    if (!current->right) current->right = new Node('\0', 0);
                    current = current->right;
                }
            }
            current->ch = pair.first;
        }
        return root;
    }

    void freeTree(Node* root) {
        if (root == nullptr) return;

        freeTree(root->left);
        freeTree(root->right);

        delete root;
    }
};

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file for reading: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open the file for writing: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }
    file << content;
    file.close();
}

int main() {
    HuffmanCoding huffman;

    while (true) {
        int choice;
        std::cout << "Select an option:\n";
        std::cout << "1. Compress a file\n";
        std::cout << "2. Decompress a file\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string inputFilename, outputFilename;
            std::cout << "Enter the filename to compress: ";
            std::getline(std::cin, inputFilename);

            std::string text = readFile(inputFilename);
            std::string encodedStr;
            std::unordered_map<char, std::string> huffmanCode;

            huffman.encode(text, encodedStr, huffmanCode);

            std::cout << "Enter the filename to save compressed data: ";
            std::getline(std::cin, outputFilename);

            huffman.saveToFile(outputFilename, encodedStr, huffmanCode);

            std::cout << "File compressed and saved successfully!" << std::endl;
            huffman.freeTree(huffman.root);

        } else if (choice == 2) {
            std::string inputFilename, outputFilename;
            std::cout << "Enter the filename to decompress: ";
            std::getline(std::cin, inputFilename);

            std::string loadedEncodedStr;
            std::unordered_map<char, std::string> loadedHuffmanCode;

            huffman.loadFromFile(inputFilename, loadedEncodedStr, loadedHuffmanCode);

            huffman.root = huffman.rebuildTree(loadedHuffmanCode);

            std::string decompressedText = huffman.decode(loadedEncodedStr, huffman.root);

            std::cout << "Enter the filename to save decompressed data: ";
            std::getline(std::cin, outputFilename);

            writeFile(outputFilename, decompressedText);

            std::cout << "File decompressed and saved successfully!" << std::endl;
            huffman.freeTree(huffman.root);

        } else if (choice == 3) {
            std::cout << "Exiting..." << std::endl;
            break;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}
