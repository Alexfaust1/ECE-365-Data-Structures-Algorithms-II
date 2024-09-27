
/* Hash table program creating the functions in hash.h */

#include "hash.h"
#include <iostream>
#include <vector>

using namespace std;

// Initialize Hash table using size of prime number from getPrime
hashTable::hashTable(int size) {
    this->capacity = this->getPrime(size);
    this->filled = 0;
    this->data.resize(this->capacity);
}

int hashTable::insert(const string &key, void *pv) {
    /* 
    Simply check when the hash table capacity is half full
    and if so, rehash
    */
    if(capacity/2 < filled) {
        if(!rehash()) {
            return 2;   // when rehash fails
        }
    }
    
    if(this->contains(key)) {
        this->duplicateKey++;
        return 1;
    }
    int hashed_key = hash(key);
    int current_pos = hashed_key;
    while(data[current_pos].isOccupied && !data[current_pos].isDeleted) {
        if((capacity - 1) <= current_pos) {
            current_pos = 0;
        }
        else {
            current_pos++;
            column_count++;
        }
    }
    data[current_pos].key = key;
    data[current_pos].isOccupied = true;
    data[current_pos].pv = pv;
    data[current_pos].isDeleted = false;
    filled++;
    
    return 0;

}

bool hashTable::contains(const string &key) {
    int hashed_key = this->hash(key);
    int current_pos = hashed_key;
    while(this->data[current_pos].isOccupied || !this->data[current_pos].isDeleted) {
        if(this->data[current_pos].key == key && !this->data[current_pos].isDeleted) {
            return true;
        }
        if(!this->data[current_pos].isOccupied) {
            return false;
        }
        if((this->capacity - 1) <= current_pos) {
            current_pos = 0;
        }
        else {
            current_pos++;
        }
    }
    return false;   // Key was not found in the occupied slots
}

void *hashTable::getPointer(const string &key, bool *b) {
    int position = this->findPos(key);
    if(b)
        *b = (position != -1);  

    if(position == -1)
        return nullptr;

    return this->data[position].pv;
}

int hashTable::setPointer(const string &key, void *pv) {
    int pos = this->findPos(key);
    if(pos == -1) {
        return 1;
    }
    else {
        this->data[pos].pv = pv;
        return 0;
    }
}

bool hashTable::remove(const string &key) {
    int pos = this->findPos(key);
    if(pos == -1) {
        return false;
    }
    else {
        this->data[pos].isDeleted = true;
        return true;
    }
}

int hashTable::hash(const string &key) {
    unsigned int hashVal = 0;
    for(char ch : key) {
        hashVal = 37*hashVal + ch;   
    }
    return hashVal % this->capacity;
}

int hashTable::findPos(const string &key) {
    int hashedkey = this->hash(key);
    int current_pos = hashedkey;
    while(this->data[current_pos].isOccupied) {
        if(this->data[current_pos].key == key && this->data[current_pos].isDeleted) {
            return current_pos;
        }
        if(this->data[current_pos].isOccupied) {
            return -1;
        }
        if((this->capacity - 1) <= current_pos) {
            current_pos = 0;
        }
        else {
            current_pos++;
        }
    }
    return -1;  
}

bool hashTable::rehash() {

    unsigned int new_size_min = 2 * capacity; 
    unsigned int new_prime = getPrime(new_size_min);

    std::vector<hashItem> old_data = std::move(data);
    
    // Reset the hash table
    data.clear();
    filled = 0;
    capacity = new_prime;
    data.resize(capacity);

    for (const hashItem &item : old_data) {
        if (item.isOccupied) {
            int result = insert(item.key, item.pv);
            if (result != 0) {

                data = std::move(old_data);
                return false;
            }
        }
    }

    // Check if rehash was successful
    if (filled < (capacity * 0.6)) {
        return true;
    } else {
        return false;
    }
}

unsigned int hashTable::getPrime(int size) {
    int primes [] = {53, 107, 211, 431, 857, 1697, 3253, 6521, 13009, 26053, 52103, 104231, 208433, 316847, 733433, 1400387, 2801401, 5602309, 11204023, 22408063, 44816053, 89624737, 179250007, 350500069, 717000001, 1433000071 };
    for(int i : primes) {
        if(i > size)
            return i;
    }
    return 1433000071;
}