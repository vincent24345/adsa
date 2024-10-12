#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable {
private:
    static const int SIZE = 26;  
    vector<string> table;        
    vector<int> status;        

public:
    // Constructor to initialize the hash table and statuses
    HashTable() : table(SIZE, ""), status(SIZE, 0) {}

    // Hash function: takes the last character of the word
    int hash(const string& key) const {
        return key.back() - 'a';  
    }

    // Insert a key into the hash table
    void insert(const string& key) {
        int idx = hash(key);
        int start = idx;

        do {
            if (status[idx] == 0 || status[idx] == 1) {  
                table[idx] = key;
                status[idx] = 2;  // Mark as "occupied"
                return;
            }
            if (table[idx] == key) return;  // Key already exists
            idx = (idx + 1) % SIZE;         // Linear probing
        } while (idx != start);  // Stop if we loop back to the start
    }

    // Delete a key from the hash table
    void remove(const string& key) {
        int idx = hash(key);
        int start = idx;

        do {
            if (status[idx] == 0) return; 
            if (status[idx] == 2 && table[idx] == key) {  // If key is found
                status[idx] = 1; 
                return;
            }
            idx = (idx + 1) % SIZE;  // Linear probing
        } while (idx != start);
    }

    // Retrieve the keys from the hash table
    void display() const {
        bool first = true;
        for (int i = 0; i < SIZE; ++i) {
            if (status[i] == 2) {  
                if (!first) cout << " ";
                cout << table[i];
                first = false;
            }
        }
        cout << endl;
    }
};

int main() {
    HashTable ht;
    string line;
    getline(cin, line);

    string command;
    for (size_t i = 0; i < line.length(); ++i) {
        if (line[i] == ' ') continue;  // Skip spaces
        if (line[i] == 'A') {
            command = line.substr(i + 1, line.find(' ', i) - i - 1);
            ht.insert(command);
        } else if (line[i] == 'D') {
            command = line.substr(i + 1, line.find(' ', i) - i - 1);
            ht.remove(command);
        }
        i += command.length();
    }

    ht.display();  // Output 
    return 0;
}
