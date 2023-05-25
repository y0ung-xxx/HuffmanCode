#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
using namespace std;

struct Node
{
    char data;
    int frequency;
    Node *left, *right;
};

void decompressFile(const string &compressedFilename, const string &decompressedFilename);
map<char, string> readCodeTable(ifstream &file);
Node *buildHuffmanTree(map<char, string> &codeTable);

string readCompressedText(ifstream &file);
string decompressText(const string &compressedText, Node *root);

void generateDecompressedFile(const string &text, const string &filename);

int main(int argc, char *argv[])
{
    // string compressedFilename = "test01.txt.huffman";
    // string decompressedFilename = "new_test01.txt";

    // string compressedFilename = "test02.txt.huffman";
    // string decompressedFilename = "new_test02.txt";

    // string compressedFilename = "test03.txt.huffman";
    // string decompressedFilename = "new_test03.txt";

    // string compressedFilename = "test04.txt.huffman";
    // string decompressedFilename = "new_test04.txt";

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <待解压缩文件名>" << endl;
        return 1;
    }
    string compressedFilename = argv[1];
    string decompressedFilename = "new_"+compressedFilename.substr(0, compressedFilename.find_last_of('.'));
    decompressFile(compressedFilename, decompressedFilename);

    return 0;
}

void decompressFile(const string &compressedFilename, const string &decompressedFilename)
{
    ifstream file(compressedFilename, ios::binary);
    if (!file.is_open())
    {
        cout << "Fail to open file: " << compressedFilename << endl;
        return;
    }

    // 读取编码表
    map<char, string> codeTable = readCodeTable(file);
    // for (auto item : codeTable)
    //     cout << item.first << ':' << item.second << endl;

    // 根据codeTable建树
    Node *root = buildHuffmanTree(codeTable);

    // 读取压缩文本
    string decompressedText = readCompressedText(file);
    // cout << decompressedText << endl;

    // 根据huffman树 转换
    string Text = decompressText(decompressedText, root);
    // cout << Text << endl;

    // 生成文件
    generateDecompressedFile(Text, decompressedFilename);
    cout << "Decompression completed. File saved as: " << decompressedFilename << endl;
    file.close();
}

map<char, string> readCodeTable(ifstream &file)
{
    map<char, string> codeTable;

    int codeTableSize;
    file.read(reinterpret_cast<char *>(&codeTableSize), sizeof codeTableSize);
    // cout << codeTableSize << endl;

    for (int i = 0; i < codeTableSize; i++)
    {
        char c;
        file.read(&c, sizeof c);

        int codeLength;
        file.read(reinterpret_cast<char *>(&codeLength), sizeof codeLength);

        string code(codeLength, ' ');
        file.read(&code[0], codeLength);

        codeTable[c] = code;
    }
    return codeTable;
}

Node *buildHuffmanTree(map<char, string> &codeTable)
{
    Node *root = new Node();
    root->data = '\0';
    root->frequency = 0;
    root->left = nullptr;
    root->right = nullptr;

    for (const auto &item : codeTable)
    {
        char c = item.first;
        const string &code = item.second;

        Node *current = root;
        for (char bit : code)
        {
            if (bit == '0')
            {
                if (current->left == nullptr)
                {
                    current->left = new Node();
                    current->left->data = '\0';
                    current->left->frequency = 0;
                    current->left->left = nullptr;
                    current->left->right = nullptr;
                }
                current = current->left;
            }
            else
            {
                if (current->right == nullptr)
                {
                    current->right = new Node();
                    current->right->data = '\0';
                    current->right->frequency = 0;
                    current->right->left = nullptr;
                    current->right->right = nullptr;
                }
                current = current->right;
            }
        }
        current->data = c;
    }
    return root;
}

string readCompressedText(ifstream &file)
{
    string compressedText;

    long long compressedTextSize;
    file.read(reinterpret_cast<char *>(&compressedTextSize), sizeof(compressedTextSize));
    // std::cout << compressedTextSize << endl;
    
    char byte;
    while (file.read(&byte, sizeof(byte)))
    {
        bitset<8> bits(byte);
        compressedText += bits.to_string();
    }
    while (compressedText.size() > compressedTextSize)
        compressedText.pop_back();
    
    return compressedText;

}

string decompressText(const string &compressedText, Node *root)
{
    string decompressedText;
    Node *current = root;
    for (char bit : compressedText)
    {
        if (bit == '0')
            current = current->left;
        else
            current = current->right;

        if (current->left == nullptr && current->right == nullptr)
        {
            decompressedText += current->data;
            current = root;
        }
    }   
    return decompressedText; 
}

void generateDecompressedFile(const string &text, const string &filename)
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Fail to open file: " << filename << endl;
        return;
    }
    file << text;
    file.close();
}
