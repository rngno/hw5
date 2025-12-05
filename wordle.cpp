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
    // add a set to store the words here so that we can keep track thru recursions :3
    std::set<std::string> words;
    wordleHelper(in, floating, dict, words);
    return words;
}

// Define any helper functions here

// need this to go through each location of the string, main logic of the recursion
// need some way to remove used floating letters
void wordleHelper(
    const std::string in,
    const std::string floating,
    const std::set<std::string>& dict,
    std::set<std::string>& words)
{
    // base case, check if word is in dict now at the end
    size_t dashPos = in.find('-');
    if (dashPos == string::npos) {
        if(floating.empty()) {
            // all floating letters used, check if in dict
            if (dict.find(in) != dict.end()) {
                words.insert(in);
            } 
        }
        return;
    }

    // need to count how many dashes left so we don't go overboard (pruning for floating letters so we avoid timing out lol)
    int dashesLeft = 0;
    for(size_t i = dashPos; i < in.length(); i++) {
        if(in[i] == '-') dashesLeft++;
    }
    if (floating.length() > dashesLeft) {
        return;
    }

    // hooooly this nesting is so ugly but i don't wanna refactor this
    // now we actually go letter by letter for the current spot
    for (char letter = 'a'; letter <= 'z'; ++letter) {
        // need some modifiable strings since floating and in are const
        std::string nextFloating = floating;
        size_t floatPos = nextFloating.find(letter);
        std::string newIn = in;
        
        // only actually recurse if the letter is valid/usable
        if (floatPos != string::npos) {
            // this one is a required float!! use here
            nextFloating.erase(floatPos, 1);
            newIn[dashPos] = letter;
            wordleHelper(newIn, nextFloating, dict, words);
        } 
        else {
            // not a float, but we need to go through some dashes here
            // plus we have space anyway
            if (dashesLeft > floating.length()) {
                newIn[dashPos] = letter;
                wordleHelper(newIn, nextFloating, dict, words);
            }
        }
        
        // did NOT work, so time to go backwards
        newIn[dashPos] = '-'; 
    }
}