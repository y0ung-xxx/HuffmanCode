#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <bitset>

using namespace std;

struct Node
{
    char data;
    int frequency;
    Node *left, *right;
};

struct Compressor
{
    bool operator()(const Node *a, const Node *b)
    {
        return a->frequency > b->frequency;
    }
};

map<char, int> countFrequencies(const string &rawText);
Node *buildHuffmanTree(map<char, int> &frequencies);
void generateCodeTable(Node *root, string code, map<char, string> &codeTable);
string compressText(const string &rawText, const map<char, string> &codeTable);
void generateCopressedFile(const string &compressedText, const map<char, string> &codeTable, const string &filename);

int main()
{
    string fileName = "test01.txt";
    // string fileName = "test02.txt";
    // string fileName = "test03.txt";
    // string fileName = "test04.txt";

    string rawText;
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        std::cout << "Fail to open file: " << fileName << endl;
        return 1;
    }
    // 将文件内容读取到字符串中
    rawText.assign((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    // 统计字符频率
    map<char, int> frequencies = countFrequencies(rawText);
    for (auto item : frequencies)
        std::cout << item.first << ':' << item.second << endl;

    // 构建哈夫曼树
    Node *root = buildHuffmanTree(frequencies);

    // 生成编码表
    map<char, string> codeTable;
    generateCodeTable(root, "", codeTable);
    cout << codeTable.size() << endl;
    // for (auto item : codeTable)
    //     std::cout << item.first << ':' << item.second << endl;

    // 压缩文本
    string compressedText = compressText(rawText, codeTable);
    std::cout << compressedText << endl;

    // 生成压缩文件
    generateCopressedFile(compressedText, codeTable, fileName + ".huffman");

    return 0;
}

map<char, int> countFrequencies(const string &rawText)
{
    map<char, int> frequencies;
    for (char c : rawText)
    {
        frequencies[c]++;
    }
    return frequencies;
}

Node *buildHuffmanTree(map<char, int> &frequencies)
{
    priority_queue<Node *, vector<Node *>, Compressor> pq;

    for (auto item : frequencies)
    {
        Node *node = new Node();
        node->data = item.first;
        node->frequency = item.second;
        node->left = nullptr;
        node->right = nullptr;
        pq.push(node);
    }

    while (pq.size() > 1)
    {
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *parent = new Node();
        parent->left = left;
        parent->right = right;
        parent->data = '\0';
        parent->frequency = left->frequency + right->frequency;
        pq.push(parent);
    }

    Node *root = pq.top();
    pq.pop();
    return root;
}

void generateCodeTable(Node *root, string code, map<char, string> &codeTable)
{
    if (root->left == nullptr && root->right == nullptr)
    {
        codeTable[root->data] = code;
        return;
    }
    if (root->left != nullptr)
        generateCodeTable(root->left, code + "0", codeTable);
    if (root->right != nullptr)
        generateCodeTable(root->right, code + "1", codeTable);
}

string compressText(const string &rawText, const map<char, string> &codeTable)
{
    string compressedText;
    for (char c : rawText)
    {
        compressedText += codeTable.at(c);
    }
    return compressedText;
}

void generateCopressedFile(const string &compressedText, const map<char, string> &codeTable, const string &filename)
{
    ofstream outputFile(filename, ios::binary);
    if (!outputFile.is_open())
    {
        std::cout << "Fail to open file: " << filename << endl;
        return;
    }
    // 写入编码表
    int codeTableSize = codeTable.size();
    outputFile.write(reinterpret_cast<const char *>(&codeTableSize), sizeof(codeTableSize));

    for (auto item : codeTable)
    {
        outputFile.write(&item.first, sizeof(char));

        int codeSize = item.second.size();
        outputFile.write(reinterpret_cast<const char *>(&codeSize), sizeof(codeSize));
        outputFile.write(item.second.c_str(), codeSize);
    }

    // 写入压入后的文本
    long long compressedTextSize = compressedText.size();
    outputFile.write(reinterpret_cast<const char *>(&compressedTextSize), sizeof(compressedTextSize));

    bitset<8> bits;
    int bitIndex = 0;
    for (char bit : compressedText)
    {
        if (bit == '1')
            bits.set(bitIndex);
        bitIndex++;
        if (bitIndex == 8)
        {
            bitset<8> temp;
            for (int i = 0; i < 8; i++)
            {
                temp[i] = bits[7 - i];
            }
            char byte = static_cast<char>(temp.to_ulong());
            outputFile.write(&byte, sizeof(byte));
            bits.reset();
            bitIndex = 0;
        }   
    }
    if (bitIndex != 0)
    {
        bitset<8> temp;
        for (int i = 0; i < 8; i++)
        {
            temp[i] = bits[7 - i];
        }
        char byte = static_cast<char>(temp.to_ulong());
        outputFile.write(&byte, sizeof(byte));
    }

    outputFile.close();
    cout << "Compression completed. File save as: " << filename << endl;
}
