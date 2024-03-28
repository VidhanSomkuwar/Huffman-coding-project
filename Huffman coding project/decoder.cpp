#include <bits/stdc++.h>
using namespace std;

//function to convert a byte into string form
string toBinary(unsigned char c)
{
    string ans;
    for(int i=0;i<8;i++)
    {
        if(c%2)
            ans+="1";
        else
            ans+="0";
        c/=2;
    }
    return ans;
}

int main()
{
    vector<char> bytes; // vector to store Decompressing Info
    char byte;
    unordered_map<string,char> mp;// Unordered Map to store reverse Symbol Table

    //Read and store Decompressing Information
    string filename("DecompressInfo.txt");
    ifstream input_file("compressed_file.txt");
    if (!input_file.is_open()) {
        cout << "Could not open the file -" << filename << endl;
        return EXIT_FAILURE;
    }
    while (input_file.get(byte)) {
        bytes.push_back(byte);
    }
    input_file.close();
    string sz;
    int j=0;
    while(bytes[j]!='\n')
    {
        sz+=bytes[j];
        j++;
    }
    int len=stoi(sz);
    j++;
    while(j<bytes.size())
    {
        string temp;
        while(bytes[j]!='\n'||(bytes[j]=='\n'&&bytes[j-1]=='\n'))
        {
            temp+=bytes[j];
            j++;
        }
        mp[temp.substr(2)]=temp[0];
        j++;
    }

    //Extract,Decode and Output data in Output File
    FILE* fp;
    fp=fopen("compressed_file.bin","rb");
    int count=0;
    string ans;
    string binary;
    FILE* op;
    unsigned char num;
    op=fopen("Original_msg.txt","w");
    while (count<len) {
        fread(&num,1,1,fp);
        binary+=toBinary(num);
        string temp;
        for(int i=0;i<binary.length();i++)
        {
            temp+=binary[i];
            if(mp.count(temp))
            {
                fwrite(&mp[temp],1,1,op);
                temp="";
                count++;
            }
            if(count>=len)
            {
                break;
            }
        }
        binary=temp;
    }

    //Close Opened Files
    fclose(op);
    fclose(fp);

    return 0;
}