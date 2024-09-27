

#include "hash.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

class spellChecker {
private:
    hashTable dictionary = hashTable(50000);

public:
    string makeValid(string word);
    bool is_alpha(string word);                    // Denotes when strings contain only characters
    string makeLowercase(string word);          // Convert string to lower case characters
    void readDictionary(string fileName);  
    void spellCheck(string inputFile, string outputFile);
    
};

void spellChecker::spellCheck(string inputFile, string outputFile) {
    
    ifstream file(inputFile);
    ofstream fout(outputFile);
    if(!file.is_open()) {
        cout << "Error: Unable to open input file: " << inputFile << endl;
        return;
    }

    int linenum = 1;
    
    string line;
    while(getline(file, line)) {
        vector<string> words;
        string temp = "";

        for(int i=0; i<line.length(); i++) {
            char c = line[i];
            if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                c == '-' || c == '\'') {

                temp += c;
            }
            else {
                if(!temp.empty()) {
                    words.push_back(temp);
                    temp = "";
                }
            }
        }
        // Add last word in line, if any
        if(!temp.empty()) {
            words.push_back(temp);
        }

        // Process words in the line
        for (auto & word : words) {
            if(is_alpha(word)) {
                if(word.length() > 20) {
                    fout << "Long word at line " << linenum << ", starts: " 
                    << this->makeLowercase(word.substr(0,20)) << '\n';
                }
                else {
                    string validate = this->makeValid(word);
                    bool cont = dictionary.contains(validate);
                    if(!cont && validate.length() > 0) {
                        fout << "Unknown word at line " << linenum <<": " << validate << '\n';
                    }
                }
            }
        }

        linenum++;
    }
    file.close();
    fout.close();
}

void spellChecker::readDictionary(string fileName) {
    
    ifstream file(fileName);
    if(file.is_open()) {
        string line;
        while(getline(file, line)) {
            string validWord = this->makeValid(line);
            if(validWord.length() != 0) {
                dictionary.insert(validWord);
            }
        }
        file.close();
    }
}

string spellChecker::makeValid(string word = "default") {
    word.erase(0, word.find_first_not_of(" \t\n\r"));
    word.erase(word.find_last_not_of( "\t\n\r") + 1);

    if(word.length() > 20) {
        return "";
    }

    for(auto iter = word.begin(); iter != word.end(); iter++) {
        if(!isalnum(*iter) && *iter != '-' && *iter != '\'') {
            return "";
        }
        else {
            // tolower() converts character to lowercase in-place which 
            // avoids an additional loop over the word
            *iter = ::tolower(*iter);
        }
    }
    return word;
}

string spellChecker::makeLowercase(string word) {
    for(char &c : word) {
        c = ::tolower(c);
    }
    return word;
}

bool spellChecker::is_alpha(string word) {
    
    for(int i = 0; i < word.length(); i++) {
        if(!isalpha(word[i]) && word[i] != '-' && word[i] != '\'') {
            return false;
        }
    }
    return true;
}

int main() {
    spellChecker spelling = spellChecker();
    cout << "Enter name of dictionary: ";
    string dictionary_file;
    cin >> dictionary_file;
    clock_t start = clock();
    spelling.readDictionary(dictionary_file);
    clock_t end = clock();
    cout << "Total time (in seconds) to load dictionary: " 
         << double(end - start) / CLOCKS_PER_SEC
         << " seconds" << endl;

    string input_file;
    string output_file;
    cout << "Enter name of input file: ";
    cin >> input_file;
    cout << "Enter name of output file: ";
    cin >> output_file;

    start = clock();
    spelling.spellCheck(input_file, output_file);
    end = clock();

    cout << "Total time (in seconds) to check document: " 
         << double(end - start) / CLOCKS_PER_SEC
         << " seconds" << endl;
    
    return 0;
}