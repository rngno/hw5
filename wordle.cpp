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


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    return wordleHelper(in, floating, dict);
}

// Define any helper functions here

// need this to go through each location of the string, main logic of the recursion
// need some way to remove used floating letters
std::set<std::string> wordleHelper(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // base case, check if word is in dict now at the end
    size_t dashPos = in.find('-');
    if (dashPos == string::npos) {
        if (dict.find(in) != dict.end()) {
            return {in};
        } 
        // no found valid word in dict, return empty so we can keep going
        else {
            return {};
        }
    }

    std::set<std::string> words; // store resulting words
    if (dashPos != string::npos) {
        // try all letters a-z in the dash position
        for (char letter = 'a'; letter <= 'z'; ++letter) {
            std::string newIn = in; // might need this copy to be modified
            newIn[dashPos] = letter;

            std::string newFloating = floating; // might need this copy to be modified

            // if letter is in floating, remove it from floating
            size_t floatPos = newFloating.find(letter);

            // find and nuke all floating letters used
            if (floatPos != string::npos) {
                newFloating.erase(floatPos, 1);
            }

            // recurse with modified params to move on to next empty space
            std::set<std::string> subWords = wordleHelper(newIn, newFloating, dict);

            // insert all found subwords into words set
            words.insert(subWords.begin(), subWords.end());
        }
    }
    return words;
}