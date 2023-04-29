#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <queue>
using namespace std;

// Node structure for Huffman tree
struct Node {
    char ch;
    int freq;
    Node *left;
    Node *right;

    Node(char ch, int freq, Node *left = nullptr, Node *right = nullptr) {
        this->ch = ch;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }

    ~Node() {
        delete left;
        delete right;
    }
};

// Comparison operator for priority queue
struct CompareNodes {
    bool operator()(const Node *a, const Node *b) const {
        return a->freq > b->freq;
    }
};

// Generate Huffman code for a given password
map<char, string> generateHuffmanCode(string password) {
    // Count frequency of each character in password
    map<char, int> freq;
    for (char ch : password) {
        freq[ch]++;
    }

    // Create leaf nodes for each character
    priority_queue<Node*, vector<Node*>, CompareNodes> pq;
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Construct Huffman tree
    while (pq.size() > 1) {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();
        Node *parent = new Node('$', left->freq + right->freq, left, right);
        pq.push(parent);
    }
    Node *root = pq.top();

    // Generate Huffman code for each character
    map<char, string> huffmanCode;
    function<void(Node*, string)> traverse = [&](Node *node, string code) {
        if (node->left == nullptr && node->right == nullptr) {
            huffmanCode[node->ch] = code;
        } else {
            traverse(node->left, code + "0");
            traverse(node->right, code + "1");
        }
    };
    traverse(root, "");

    // Clean up memory
    delete root;

    return huffmanCode;
}

// Encrypt a password using Huffman code
string encryptPassword(string password, map<char, string> huffmanCode) {
    string encryptedPassword;
    for (char ch : password) {
        encryptedPassword += huffmanCode[ch];
    }
    return encryptedPassword;
}

int main() {
    // Prompt user to enter a password
    cout << "Enter a password: ";
    string password;
    getline(cin, password);

    // Generate Huffman code for password
    map<char, string> huffmanCode = generateHuffmanCode(password);

    // Encrypt password using Huffman code
    string encryptedPassword = encryptPassword(password, huffmanCode);

    // Print encrypted password and Huffman code
    cout << "Encrypted Password: " << encryptedPassword << endl;
    for (auto pair : huffmanCode) {
        cout << pair.first << ": " << pair.second << endl;
    }

    return 0;
}
