#include <iostream>
#include <vector>
#include <string>

using namespace std;

class HashTable {
private:
    static const int SIZE = 26;  
    vector<string> table;
    vector<int> status;  // 0: empty, 1: deleted, 2: occupied

    int hash(const string& key) const {
        return key.back() - 'a';  
    }

public:
    HashTable() : table(SIZE, ""), status(SIZE, 0) {}

    void insert(const string& key) {
        int idx = hash(key);

        while (status[idx] == 2 && table[idx] != key) {
            idx = (idx + 1) % SIZE;  // Linear probing
        }

        if (status[idx] != 2) {
            table[idx] = key;
            status[idx] = 2;  // Mark as occupied
        }
    }

    void remove(const string& key) {
        int idx = hash(key);

        while (status[idx] != 0) {
            if (status[idx] == 2 && table[idx] == key) {
                status[idx] = 1;  // Mark as deleted
                return;
            }
            idx = (idx + 1) % SIZE;
        }
    }

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
    string input;
    getline(cin, input);

    size_t pos = 0;
    while (pos < input.length()) {
        if (input[pos] == 'A') {
            size_t spacePos = input.find(' ', pos + 1);
            string key = input.substr(pos + 1, spacePos - pos - 1);
            ht.insert(key);
            pos = spacePos;
        } else if (input[pos] == 'D') {
            size_t spacePos = input.find(' ', pos + 1);
            string key = input.substr(pos + 1, spacePos - pos - 1);
            ht.remove(key);
            pos = spacePos;
        }
        pos++;
    }

    ht.display();  
    return 0;
}
