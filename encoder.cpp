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

// Function to write a compressed file using new prefix codes
void writeFile(int bit, int &current, unsigned char &bitBuffer, FILE *f){
    if(bit){
        bitBuffer |= (1<< current);       // To reduce write operations
    }
    current ++;
    if(current == 8){
        fwrite(&bitBuffer,1,1,f);
        current = 0;
        bitBuffer = 0;
    }
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

    for(auto i:Table){
        cout << i.first << " - " << i.second << endl;
    }

    string S;

    for(int i=0; i<bytes.size();i++){
        S += Table[bytes[i]];
    }

    cout << S << endl;

    FILE *output;
    output = fopen("compressed_file.bin","wb");
    if(!output){
        cout << "Could not open compressed_file,bin" << endl;
    }

    int current = 0;
    unsigned char bitBuffer = 0;

    for(int i=0; i<S.length(); i++){
        writeFile(S[i] - 48, current,bitBuffer, output);
            
    }
        while(current){
            writeFile(0,current,bitBuffer,output);
        }

    //Create file containing information to Decompress the Compressed file    
    FILE* passInfo;
    passInfo=fopen("DecompressInfo.txt","w");
    if(!passInfo)
    {
        cout << "Could not open File - DecompressInfo.txt" << endl;
    }
    string sz=to_string(bytes.size())+"\n";
    for(int i=0;i<sz.length();i++)
    {
        fwrite(&sz[i],1,1,passInfo);
    }
    for(auto i:Table)
    {
        fwrite(&i.first,1,1,passInfo);
        char space=' ';
        fwrite(&space,1,1,passInfo);
        for(int j=0;j<i.second.length();j++)
        {
            fwrite(&i.second[j],1,1,passInfo);
        }
        char endLine='\n';
        fwrite(&endLine,1,1,passInfo);
    }

    //Close Opened Files
    fclose (output);
    inputFile.close();
    fclose (passInfo);

    return 0;  
}