#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  string s = token;
  while (!s.empty() && ispunct(s.front())) {
    s.erase(0, 1);
  }

  while (!s.empty() && ispunct(s.back())) {
    s.pop_back();
  }

  bool hasLetter = false;
  for (char c : s) {
    if (isalpha(c)) {
      hasLetter = true;
      break;
    }
  }

  if (!hasLetter) {
    return "";
  }

  for (char& c : s) {
    c = tolower(c);
  }

  return s;
}

set<string> gatherTokens(const string& text) {
  set<string> tokens;
  stringstream ss(text);
  string rawWord;

  while (ss >> rawWord) {
    string cleaned = cleanToken(rawWord);

    if (!cleaned.empty()) {
      tokens.insert(cleaned);
    }
  }

  return tokens;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  ifstream infile(filename);
  if (!infile.is_open()) {
    return 0;
  }

  string url, body;
  int pageCount = 0;

  while (getline(infile, url)) {
    if (getline(infile, body)) {
      set<string> tokens = gatherTokens(body);

      for (const string& token : tokens) {
        index[token].insert(url);
      }
      pageCount++;
    }
  }

  return pageCount;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  set<string> result;
  stringstream ss(sentence);
  string rawWord;

  if (ss >> rawWord) {
    string cleanedWord = cleanToken(rawWord);
    if (index.count(cleanedWord) > 0) {
      result = index.at(cleanedWord);
    }
  }

  while (ss >> rawWord) {
    char modifier = rawWord.front();
    string cleanedWord = cleanToken(rawWord);

    set<string> currentSet;
    if (index.count(cleanedWord) > 0) {
      currentSet = index.at(cleanedWord);
    }

    set<string> tempResult;

    if (modifier == '+') {
      set_intersection(result.begin(), result.end(), currentSet.begin(),
                       currentSet.end(),
                       inserter(tempResult, tempResult.begin()));
      result = tempResult;
    } else if (modifier == '-') {
      set_difference(result.begin(), result.end(), currentSet.begin(),
                     currentSet.end(),
                     inserter(tempResult, tempResult.begin()));
      result = tempResult;
    } else {
      set_union(result.begin(), result.end(), currentSet.begin(),
                currentSet.end(), inserter(tempResult, tempResult.begin()));
      result = tempResult;
    }
  }

  return result;
  return {};
}

void searchEngine(const string& filename) {
  map<string, set<string>> index;
  int numPages = buildIndex(filename, index);

  if (numPages == 0) {
    cout << "Invalid filename." << endl;
  }

  cout << "Stand by while building index..." << endl;

  int numTerms = index.size();

  cout << "Indexed " << numPages << " pages containing " << numTerms
       << " unique terms" << endl;

  string query;
  while (true) {
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin, query);

    if (query.empty()) {
      cout << "Thank you for searching!" << endl;
      break;
    }

    set<string> matches = findQueryMatches(index, query);
    cout << "Found " << matches.size() << " matching pages" << endl;

    for (const string& url : matches) {
      cout << url << endl;
    }
  }
}
