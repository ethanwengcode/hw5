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
void wordleHelper(
    string temp,
    const string& in,
    string floating,
    const set<string>& dict,
    set<string>& res,
    int index
);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> res;
    string temp = in; 
    wordleHelper(temp, in, floating, dict, res, 0);
    return res;
}

// Define any helper functions here
void wordleHelper(
    string temp,
    const string& in,
    string floating,
    const set<string>& dict,
    set<string>& res,
    int index
) {
    // full word is created 
    if(index == in.size()) {
        // check if it's a true English-language word and uses the floating characters
        if(dict.find(temp) != dict.end() && floating.empty()) {
            res.insert(temp);  // store in results
        }
        return;
    }

    char c = in[index]; // get current character
    if(c == '-') { // blank
        // count number of blank locations
        int numBlank = 0;
        for(size_t i = index; i < in.size(); i++) {
            if( in[i] == '-') {
                numBlank++;
            }
        }

        // try all characters
        for(char j = 'a'; j <= 'z'; j++) {
            string floatingChars = floating; // stores floating chars

            if( floating.find(j) != string::npos) { // floating character
                floatingChars.erase(floating.find(j), 1); // remove the single character; index is where the char was found at
                temp[index] = j; // replace '-' with the floating char
                wordleHelper(temp, in, floatingChars, dict, res, index +1);
            }
            else if( floating.size() < numBlank ) { // have more blanks than floating characters
                temp[index] = j;
                wordleHelper(temp, in, floatingChars, dict, res, index +1);
            }
        }
    }
    else { // not blank
        temp[index] = c;
        wordleHelper(temp, in, floating, dict, res, index+1); // go to next character
    }
} 