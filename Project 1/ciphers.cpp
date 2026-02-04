#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "include/caesar_dec.h"
#include "include/caesar_enc.h"
#include "include/subst_dec.h"
#include "include/subst_enc.h"
#include "utils.h"

using namespace std;

// Initialize random number generator in .cpp file for ODR reasons
std::mt19937 Random::rng;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function declarations go at the top of the file so we can call them
// anywhere in our program, such as in main or in other functions.
// Most other function declarations are in the included header
// files.

// When you add a new helper function, make sure to declare it up here!

/**
 * Print instructions for using the program.
 */
void printMenu();
void decryptSubstCipherFileCommand(const QuadgramScorer& scorer);
int main() {
  Random::seed(time(NULL));
  string command;
  vector<string> dictionary;
  ifstream dictFile("dictionary.txt");
  string word;

  while (dictFile >> word) {
    dictionary.push_back(word);
  }
  vector<string> quadgrams;
  vector<int> counts;
  ifstream quadFile("english_quadgrams.txt");
  string qLine;

  while (getline(quadFile, qLine)) {
    size_t commaPos = qLine.find(',');
    if (commaPos != string::npos) {
      quadgrams.push_back(qLine.substr(0, commaPos));
      counts.push_back(stoi(qLine.substr(commaPos + 1)));
    }
  }
  quadFile.close();

  QuadgramScorer scorer(quadgrams, counts);

  cout << "Welcome to Ciphers!" << endl;
  cout << "-------------------" << endl;
  cout << endl;

  do {
    printMenu();
    cout << endl << "Enter a command (case does not matter): ";

    getline(cin, command);
    cout << endl;

    if (command == "R" || command == "r") {
      string seed_str;
      cout << "Enter a non-negative integer to seed the random number "
              "generator: ";
      getline(cin, seed_str);
      Random::seed(stoi(seed_str));
    } else if (command == "A" || command == "a") {
      applyRandSubstCipherCommand();
    } else if (command == "C" || command == "c") {
      caesarEncryptCommand();
    } else if (command == "D" || command == "d") {
      caesarDecryptCommand(dictionary);
    } else if (command == "E" || command == "e") {
      computeEnglishnessCommand(scorer);
    } else if (command == "S" || command == "s") {
      decryptSubstCipherCommand(scorer);
    } else if (command == "F" || command == "f") {
      decryptSubstCipherFileCommand(scorer);
    }

    cout << endl;

  } while (!(command == "x" || command == "X") && !cin.eof());

  return 0;
}

void printMenu() {
  cout << "Ciphers Menu" << endl;
  cout << "------------" << endl;
  cout << "C - Encrypt with Caesar Cipher" << endl;
  cout << "D - Decrypt Caesar Cipher" << endl;
  cout << "E - Compute English-ness Score" << endl;
  cout << "A - Apply Random Substitution Cipher" << endl;
  cout << "S - Decrypt Substitution Cipher from Console" << endl;
  cout << "F - Decrypt Substitution Cipher from File" << endl;
  cout << "R - Set Random Seed for Testing" << endl;
  cout << "X - Exit Program" << endl;
}

// "#pragma region" and "#pragma endregion" group related functions in this file
// to tell VSCode that these are "foldable". You might have noticed the little
// down arrow next to functions or loops, and that you can click it to collapse
// those bodies. This lets us do the same thing for arbitrary chunks!
#pragma region CaesarEnc

char rot(char c, int amount) {
  // TODO: student
  int pos = ALPHABET.find(c);
  pos = (pos + amount) % 26;
  return ALPHABET[pos];
  return 'A';
}

string rot(const string& line, int amount) {
  string result = "";

  for (char c : line) {
    if (isalpha(c)) {
      c = toupper(c);
      result += rot(c, amount);
    } else if (isspace(c)) {
      result += c;
    }
  }
  return result;
}

void caesarEncryptCommand() {
  // TODO: student

  string text;
  string amountStr;
  cout << "Enter the text to encrypt: ";
  getline(cin, text);

  cout << "Enter the number of characters to rotate by: ";
  getline(cin, amountStr);

  int amount = stoi(amountStr);

  string encrypted = rot(text, amount);
  cout << "Encrypted text: " << encrypted << endl;
}

#pragma endregion CaesarEnc

#pragma region CaesarDec

void rot(vector<string>& strings, int amount) {
  // TODO: student
  for (string& s : strings) {
    s = rot(s, amount);
  }
}

string clean(const string& s) {
  string cleaned = "";

  for (char c : s) {
    if (c >= 'A' && c <= 'Z') {
      cleaned += c;
    } else if (c >= 'a' && c <= 'z') {
      cleaned += (c - 32);
    }
  }

  return cleaned;
}

vector<string> splitBySpaces(const string& s) {
  // TODO: student
  vector<string> words;
  stringstream ss(s);
  string temp;

  while (ss >> temp) {
    words.push_back(clean(temp));
  }
  return words;
}

string joinWithSpaces(const vector<string>& words) {
  // TODO: student
  string result;
  for (size_t i = 0; i < words.size(); i++) {
    result += words[i];
    if (i + 1 < words.size()) {
      result += " ";
    }
  }
  return result;
}

int numWordsIn(const vector<string>& words, const vector<string>& dict) {
  int count = 0;

  for (const string& w : words) {
    for (const string& d : dict) {
      if (w == d) {
        count++;
        break;
      }
    }
  }

  return count;
}

void caesarDecryptCommand(const vector<string>& dict) {
  // TODO: student
  cout << "Enter the text to Caesar decrypt: ";
  string input;
  getline(cin, input);

  vector<string> originalWords = splitBySpaces(input);
  bool found = false;

  for (int shift = 0; shift < 26; shift++) {
    vector<string> rotated = originalWords;
    rot(rotated, shift);

    int validWords = numWordsIn(rotated, dict);
    if (validWords > rotated.size() / 2) {
      cout << joinWithSpaces(rotated) << endl;
      found = true;
    }
  }

  if (!found) {
    cout << "No good decryptions found" << endl;
  }
}

#pragma endregion CaesarDec

#pragma region SubstEnc

string applySubstCipher(const vector<char>& cipher, const string& s) {
  // TODO: student
  string result = "";
  for (char c : s) {
    if (isalpha(c)) {
      int index = 0;
      if (c >= 'a' && c <= 'z') {
        index = c - 'a';
      } else {
        index = c - 'A';
      }
      result += cipher[index];
    } else {
      result += c;
    }
  }
  return result;
}

void applyRandSubstCipherCommand() {
  // TODO: student
  cout << "Enter the text to encrypt: ";
  string text;
  getline(cin, text);

  vector<char> cipher = genRandomSubstCipher();

  string encrypted = applySubstCipher(cipher, text);
  cout << "Encrypted text: " << encrypted << endl;
}

#pragma endregion SubstEnc

#pragma region SubstDec

double scoreString(const QuadgramScorer& scorer, const string& s) {
  // TODO: student
  string cleanStr = "";
  for (char c : s) {
    if (isalpha(c)) {
      cleanStr += toupper(c);
    }
  }

  if (cleanStr.length() < 4) {
    return 0.0;
  }

  double totalScore = 0.0;
  for (size_t i = 0; i <= cleanStr.length() - 4; i++) {
    string quad = cleanStr.substr(i, 4);
    totalScore += scorer.getScore(quad);
  }
  return totalScore;
}

void computeEnglishnessCommand(const QuadgramScorer& scorer) {
  // TODO: student
  cout << "Enter a string for englishness scoring: ";
  string text;
  getline(cin, text);

  double score = scoreString(scorer, text);
  cout << "English-ness score: " << score << endl;
}

vector<char> hillClimb(const QuadgramScorer& scorer, const string& ciphertext) {
  // TODO: student
  vector<char> currentKey = genRandomSubstCipher();
  string decrypted = applySubstCipher(currentKey, ciphertext);
  double currentScore = scoreString(scorer, decrypted);

  int noImprovementCount = 0;

  while (noImprovementCount < 1000) {
    vector<char> nextKey = currentKey;

    int index1 = Random::randInt(25);
    int index2 = Random::randInt(25);

    while (index1 == index2) {
      index2 = Random::randInt(25);
    }

    char temp = nextKey[index1];
    nextKey[index1] = nextKey[index2];
    nextKey[index2] = temp;

    string nextDecrypted = applySubstCipher(nextKey, ciphertext);
    double nextScore = scoreString(scorer, nextDecrypted);

    if (nextScore > currentScore) {
      currentKey = nextKey;
      currentScore = nextScore;
      noImprovementCount = 0;
    } else {
      noImprovementCount++;
    }
  }
  return currentKey;
}

vector<char> decryptSubstCipher(const QuadgramScorer& scorer,
                                const string& ciphertext) {
  // TODO: student
  vector<char> bestKey;
  double bestScore = -1000000000.0;

  for (int i = 0; i < 25; i++) {
    vector<char> candidateKey = hillClimb(scorer, ciphertext);
    string decrypted = applySubstCipher(candidateKey, ciphertext);
    double candidateScore = scoreString(scorer, decrypted);

    if (candidateScore > bestScore) {
      bestScore = candidateScore;
      bestKey = candidateKey;
    }
  }
  return bestKey;
}

void decryptSubstCipherCommand(const QuadgramScorer& scorer) {
  // TODO: student
  cout << "Enter a string for substitution decryption: ";
  string input;
  getline(cin, input);

  vector<char> bestKey = decryptSubstCipher(scorer, input);
  string result = applySubstCipher(bestKey, input);
  cout << "Decrypted text: " << result << endl;
}
void decryptSubstCipherFileCommand(const QuadgramScorer& scorer) {
  string inputFilename, outputFilename;
  cout << "Enter a filename for input: ";
  getline(cin, inputFilename);
  cout << "Enter a filename for output: ";
  getline(cin, outputFilename);

  ifstream inFile(inputFilename);
  if (!inFile.is_open()) {
    cout << "Error opening input file." << endl;
    return;
  }
  stringstream buffer;
  buffer << inFile.rdbuf();
  string ciphertext = buffer.str();
  inFile.close();
  vector<char> bestKey = decryptSubstCipher(scorer, ciphertext);
  string plaintext = applySubstCipher(bestKey, ciphertext);

  ofstream outFile(outputFilename);
  if (outFile.is_open()) {
    outFile << plaintext;
    outFile.close();
  }
}
#pragma endregion SubstDec
