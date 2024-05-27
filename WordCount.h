// WordCount.cpp

#include "WordCount.h"

using namespace std;

// Default constructor
WordCount::WordCount() {}

// Simple hash function. Do not modify.
size_t WordCount::hash(std::string word) const {
	size_t accumulator = 0;
	for (size_t i = 0; i < word.size(); i++) {
		accumulator += word.at(i);
	}
	return accumulator % CAPACITY;
}

int WordCount::getTotalWords() const {
	int totalWords = 0; // Initialize a counter to keep track of the total words
    for (size_t i = 0; i < CAPACITY; ++i) { // Loop through each bucket in the hash table
        for (const auto &word_pair : table[i]) { // Loop through each pair in the current bucket
            totalWords += word_pair.second; // Add the number of occurrences of the current word to the total count
        }
    }
    return totalWords; // Return the computed total
}

int WordCount::getNumUniqueWords() const {
	int totalWords = 0; // Initialize a counter to keep track of the total words
    for (size_t i = 0; i < CAPACITY; ++i) { // Loop through each bucket in the hash table
        for (const auto &word_pair : table[i]) { // Loop through each pair in the current bucket
            if(word_pair.second!=0){
				totalWords++;
			} // Add the number of occurrences of the current word to the total count
        }
    }
    return totalWords;
}

int WordCount::getWordCount(std::string word) const {
	string valid = makeValidWord(word);
	size_t index = hash(valid);
	for(size_t i = 0; i < table[index].size(); i++) {
		if(table[index][i].first == valid) {
			return table[index][i].second;
		}
	}
	return 0;
}
	
int WordCount::incrWordCount(std::string word) {
	std::string newWord = makeValidWord(word);
    if (newWord.empty()) {
        return 0;
    }

    size_t index = hash(newWord);
    for (auto& pair : table[index]) {
        if (pair.first == newWord) {
            return ++pair.second;
        }
    }
	pair<string, int> newPair = make_pair(newWord, 1);
    table[index].push_back(newPair);
    return 1;
}

int WordCount::decrWordCount(std::string word) {
	 std::string validatedWord = makeValidWord(word);
    if (validatedWord.empty()) {
        return -1;
    }

    size_t index = hash(validatedWord);
    for (size_t i = 0; i < table[index].size(); ++i) {
        if (table[index][i].first == validatedWord) {
            if (table[index][i].second == 1) {
                table[index].erase(table[index].begin() + i);
                return 0;
            } else {
                return --table[index][i].second;
            }
        }
    }

    return -1;
}

bool WordCount::isWordChar(char c) {
	if((c>= 'a'&& c <= 'z')|| (c <='Z' &&  c>='A')){
		return true;
	}
	return false;
}

std::string WordCount::makeValidWord(std::string word) {
	string validword = "";
	if (word.size() == 0){
		return validword;
	}
	for(size_t i = 0; i < word.size(); i++){
		if(isWordChar(word.at(i))==false && word.at(i)!= '-' && word.at(i)!='\''){
			continue;
		}
		validword+= tolower(word.at(i));

	}
	if(validword.size() == 0){
		return validword;
	}
	bool first = false;
	
	while(first == false && validword.size()>0){
		if(validword.at(0) == '-' || validword.at(0) == '\''){
			validword.erase(0,1);
		}
		else{
			first = true;
		}
	}
	bool end = false;
	while(end == false && validword.size()>0){
		if(validword.at(validword.size()-1) == '-' || validword.at(validword.size()-1) == '\''){
			validword.erase(validword.size()-1,1);
		}
		else{
			end = true;
		}
	}
	return validword;
}
void WordCount::dumpWordsSortedByWord(std::ostream &out) const {
    if (getTotalWords() == 0) {
        return;
    }

    std::vector<std::pair<std::string, int>> sortedTable;

    // Collect all words and their counts into the sortedTable vector
    for (size_t i = 0; i < CAPACITY; ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) {
            sortedTable.push_back(table[i][j]);
        }
    }

    // Sort the vector alphabetically by the word
    std::sort(sortedTable.begin(), sortedTable.end());

    // Output the sorted words and their counts
    for (const auto &entry : sortedTable) {
        out << entry.first << ", " << entry.second << "\n";
    }
}
bool sortbysec(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
    if (a.second != b.second) {
        return a.second < b.second;
    }
    return a.first < b.first;
}
void WordCount::dumpWordsSortedByOccurence(std::ostream &out) const {
    if (getTotalWords() == 0) {
        return;
    }

    std::vector<std::pair<std::string, int>> sortedTable;

    // Collect all words and their counts into the sortedTable vector
    for (size_t i = 0; i < CAPACITY; ++i) {
        for (const auto &entry : table[i]) {
            sortedTable.push_back(entry);
        }
    }

    // Sort the vector by occurrence (using the custom sortbysec function)
    std::sort(sortedTable.begin(), sortedTable.end(), sortbysec);

    // Output the sorted words and their counts
    for (const auto &entry : sortedTable) {
        out << entry.first << ", " << entry.second << "\n";
    }
}
void WordCount::addAllWords(const std::string text) {
    std::string newWord;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text.at(i) == ' ' || text.at(i) == '\n' || text.at(i) == '\t' || i == text.size() - 1) {
            if (i == text.size() - 1 && (text.at(i) != ' ' && text.at(i) != '\n' && text.at(i) != '\t')) {
                newWord += text.at(i);
            }
            if (!newWord.empty()) {
                incrWordCount(newWord);
                newWord.clear();
            }
        } else {
            newWord += text.at(i);
        }
    }
}
