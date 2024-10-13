#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable {
private:
//hash table and vector to store keys
    static const int TABLE_SIZE = 26;  
    vector<string> table;         
    vector<int> entryStatus;            

public:
    //constructor for hash table
    HashTable() : table(TABLE_SIZE, ""), entryStatus(TABLE_SIZE, 0) {}

    //returns index 
    int getHashIndex(const string& key) const {
        return key[key.length() - 1] - 'a';
    }
    //linear probing inserting string
    void insertKey(const string& key) {
        int idx = getHashIndex(key);
        int originalIdx = idx;  

        //loop for empty space
        while (entryStatus[idx] == 2) {  
            if (table[idx] == key) return;  
            idx = (idx + 1) % TABLE_SIZE;  
            if (idx == originalIdx) return;
        }
        
        //place key in found spot
        table[idx] = key;
        entryStatus[idx] = 2; 
    }

    //delete string
    void deleteKey(const string& key) {
        int idx = getHashIndex(key);
        int originalIdx = idx;  
    //keep searching while slots are occupied
        while (entryStatus[idx] != 0) {  
            if (entryStatus[idx] == 2 && table[idx] == key) {  
                entryStatus[idx] = 1;  
                return;
            }
            idx = (idx + 1) % TABLE_SIZE; 
            if (idx == originalIdx) return; 
        }
    }

    //display non-emptry keys
    void displayKeys() const {
        bool firstPrint = true;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (entryStatus[i] == 2) {  
                if (!firstPrint) cout << " ";
                cout << table[i];
                firstPrint = false;
            }
        }
        cout << endl;
    }
};

int main() {
    HashTable hashTable;
    string inputLine;
    getline(cin, inputLine);  

    string currentCommand;
    string currentKey;

    for (size_t i = 0; i < inputLine.length(); ++i) {
        if (inputLine[i] == ' ') continue;  

        if (inputLine[i] == 'A') {  
            currentCommand = "ADD";
            size_t j = i + 1;
            while (j < inputLine.length() && inputLine[j] != ' ') j++;
            currentKey = inputLine.substr(i + 1, j - i - 1); 
            hashTable.insertKey(currentKey);
            i = j - 1;  
        } 
        //delete command
        else if (inputLine[i] == 'D') {  
            currentCommand = "DELETE";
            size_t j = i + 1;
            while (j < inputLine.length() && inputLine[j] != ' ') j++;
            currentKey = inputLine.substr(i + 1, j - i - 1); 
            hashTable.deleteKey(currentKey);
            i = j - 1;  
        }
    }

    hashTable.displayKeys();  
    return 0;
}
