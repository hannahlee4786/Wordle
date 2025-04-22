#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;

// Helper function prototype
void generateWords(
    const string& in,
    string word,
    string floating,
    const set<string>& dict,
    set<string>& results,
    int idx);

// Definition of primary wordle function
set<string> wordle(
    const string& in,
    const string& floating,
    const set<string>& dict)
{
    set<string> results;
    string word = in;
    generateWords(in, word, floating, dict, results, 0);
    return results;
}

// Helper function that creates all possible words given conditions
void generateWords(
    const string& in,
    string word,
    string floating,
    const set<string>& dict,
    set<string>& results,
    int idx)
{
    // Base case: word of the correct length + conditions are met
    if (idx == in.size()) {
        // If all floating letters are used + word in the dictionary
        if (floating.empty() && dict.find(word) != dict.end()) {
            results.insert(word);
        }
        return;
    }

    // Case 1: position is fixed
    if (in[idx] != '-') {
        word[idx] = in[idx];
        generateWords(in, word, floating, dict, results, idx + 1);
    }
    // Case 2: check floating
    else {
        // Iterate through all lowercase letters at this position
        for (char letter = 'a'; letter <= 'z'; ++letter) {
            // Place that letter at the current index
            word[idx] = letter;
            string updatedFloating = floating;

            size_t pos = updatedFloating.find(letter);
            
            // If letter is in floating, remove one instance
            if (pos != string::npos) {
                updatedFloating.erase(pos, 1);
                generateWords(in, word, updatedFloating, dict, results, idx + 1);
            }
            // Else only use non-floating letters if enough blanks left to place all floating
            else if (floating.size() < count(in.begin() + idx, in.end(), '-')) {
                generateWords(in, word, updatedFloating, dict, results, idx + 1);
            }
        }
    }
}
