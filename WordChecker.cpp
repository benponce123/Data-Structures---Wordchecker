// WordChecker.cpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"
#include <iostream>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> suggestions;
    std::string temp, temp1, temp2;
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, j;


    // Swapping adjacent characters
    for(i = 0; i < word.length() - 1; i++)
    {
        temp = word;
        std::swap(temp[i], temp[i+1]);

        //repeating
        if (wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
        {
            suggestions.push_back(temp);
        }
    }


    // Inserting letter in between adjacent characters
    for (i = 0; i < alphabet.length(); i++)
    {
        for (j = 0; j <= word.length(); j++)
        {
            temp = word;
            temp.insert(j, 1, alphabet[i]);

            if (wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
            {
                suggestions.push_back(temp);
            }
        }
    }


    // Deleting each character
    for (i = 0; i < word.length(); i++)
    {
        temp = word;
        temp.erase(i, 1);

        if (wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
        {
            suggestions.push_back(temp);
        }
    }
    

    // Replacing each character
    for (i = 0; i < alphabet.length(); i++)
    {
        for (j = 0; j < word.length(); j++)
        {
            temp = word;
            temp.replace(j, 1, 1, alphabet[i]);

            if (wordExists(temp) && std::find(suggestions.begin(), suggestions.end(), temp) == suggestions.end())
            {
                suggestions.push_back(temp);
            }
        }
    }


    // Splitting the word
    for (i = 1; i < word.length(); i++)
    {
        temp = word;
        temp1 = temp.substr(0, i);
        temp2 = temp.substr(i);
        
        if (wordExists(temp1) && wordExists(temp2) && std::find(suggestions.begin(), suggestions.end(), temp1) == suggestions.end() && std::find(suggestions.begin(), suggestions.end(), temp2) == suggestions.end())
        {
            suggestions.push_back(temp.insert(i, " "));
        }
    }

    return suggestions;
}

