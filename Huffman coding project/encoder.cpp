#include<bits/stdc++.h>
using namespace std;

// Defining node on Huffman tree
class Node{
    public:
    char c;
    int freq;
    Node* left;
    Node* right;

    // constructor
    Node(){
        left = NULL;
        right = NULL;
    }
};

// comparator for priority queue
class compare{
    public:
    bool operator()(Node *a, Node *b){
        return a->freq > b->freq;
    }
};

// Function for generating prefix codes
void generateCode(Node *curr, unordered_map<char,string> &Table, string &path){
    if(curr -> left == NULL && curr -> right == NULL){
        Table[curr -> c] = path;
        delete(curr);
        return;
    }
    path = path + "0";
    generateCode(curr -> left, Table,path);
    path[path.length() -1] = '1';
    generateCode(curr -> right, Table, path);
    path = path.substr(0,path.length() -1);
}

int main(){
    string filename("input.txt"); // input file
    unordered_map<char,int> count;
    vector<char> bytes;
    char byte;

    ifstream inputFile(filename);

    if(!inputFile.is_open()){
        cout << "Could not open the file _ " << filename << endl;
        return EXIT_FAILURE;
    }

    // Read input file
    while(inputFile.get(byte)){
        count[byte]++;
        bytes.push_back(byte);
    }

    priority_queue<Node* , vector<Node*>, compare > Tree;

    for(auto i:count){
        Node *n = new Node();
        n -> c = i.first;
        n -> freq = i.second;
        Tree.push(n);
    }

    while(Tree.size() != 1){
        Node *n1 = Tree.top();
        Tree.pop();
        Node *n2 = Tree.top();
        Tree.pop();

        Node *n = new Node();
        n -> left = n1;
        n -> right = n2;
        n -> freq = (n1->freq) + (n2->freq);
        Tree.push(n);
    }

    // Print prefix codes
    unordered_map<char,string> Table;
    string path = "";
    Node *p = Tree.top();
    generateCode(p,Table,path);

    // Write the compressed data to a text file
    ofstream outputFile("compressed_file.txt");
    if(!outputFile.is_open()){
        cout << "Could not open compressed_file.txt for writing." << endl;
        return EXIT_FAILURE;
    }

    for(int i = 0; i < bytes.size(); i++){
        outputFile << Table[bytes[i]];
    }

    outputFile.close();
    inputFile.close();

    // Write decompression info to a separate text file
    ofstream passInfo("DecompressInfo.txt");
    if(!passInfo.is_open()){
        cout << "Could not open DecompressInfo.txt for writing." << endl;
        return EXIT_FAILURE;
    }

    passInfo << bytes.size() << "\n";
    for(auto i : Table){
        passInfo << i.first << " " << i.second << "\n";
    }

    passInfo.close();

    return 0;
}
