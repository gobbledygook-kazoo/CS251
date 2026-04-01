#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>

#include "bstset.h"
#include "include/worldle.h"

using namespace std;

/**
 * Given a filename that contains a list of words (one word per line),
 * returns a set that contains all the words in the file.
 */
BSTSet<string> load_words(const string& filename) {
  BSTSet<string> words;
    ifstream file(filename);

    if (!file.is_open()) {
        return words;
    }

    string word;
    while (file >> word) {
        words.insert(word);
    }

    file.close();
    return words;
  return BSTSet<string>{};
}

/**
 * Given a `guess` and the potential `winning_word`, computes the pattern
 * that Wordle would give us, as a string in which each character
 * corresponds to the character position on the original string and the
 * colored feedback is output in numbers as:
 *
 * - 0 - Gray
 * - 1 - Yellow
 * - 2 - Green
 *
 * For example:
 * - `get_pattern("RAISE", "SCORE") == "10012"`
 * - `get_pattern("MERRY", "CORER") == "01210"`
 */
string get_pattern(const string& guess, const string& winning_word) {
    string pattern = "00000";
    
    bool guess_used[5] = {false, false, false, false, false};
    bool winning_used[5] = {false, false, false, false, false};

    for (int i = 0; i < 5; ++i) {
        if (guess[i] == winning_word[i]) {
            pattern[i] = '2';
            guess_used[i] = true;
            winning_used[i] = true;
        }
    }

    for (int i = 0; i < 5; ++i) {
        if (guess_used[i]) continue;

        for (int j = 0; j < 5; ++j) {
            if (!winning_used[j] && guess[i] == winning_word[j]) {
                pattern[i] = '1';
                winning_used[j] = true;
                break; 
            }
        }
    }

    return pattern;
}

/**
 * Given the current `remaining_words`, keep only the words that conform
 * to the given `feedback`, based on the given `guess`.
 *
 * For example:
 * If `remaining_words` contains {"BASIC", "SONIC", "TONIC", "STOIC"}, then:
 *
 * - `filter_words(remaining_words, "BASIC", "00122") will erase `"BASIC"` and
 * `"TONIC"` from `remaining_words`.
 *
 * In this case, the `feedback` says that the word should have an 'S' (not in
 * the middle position), therefore we filter out "TONIC". It also says that it
 * doesn't have an 'A' nor a 'B', so we filter out "BASIC". Given the
 * `feedback`, "STOIC" and "SONIC" are still possible winning words that
 * remain in `remaining_words`.
 */
void filter_words(BSTSet<string>& remaining_words, const string& guess,
                  const string& feedback) {
  vector<string> to_remove;

    remaining_words.begin();
    string word;
    while (remaining_words.next(word)) {
        if (get_pattern(guess, word) != feedback) {
            to_remove.push_back(word);
        }
    }

    for (const string& w : to_remove) {
        remaining_words.erase(w);
    }
}

/**
 * Given a set of words that are still playable, returns
 * the word that maximizes the entropy.
 */
string recommend(BSTSet<string>& remaining_words) {
  if (remaining_words.empty()) {
    return "";
  }

  if (remaining_words.size() == 1) {
    remaining_words.begin();
    string final_word;
    remaining_words.next(final_word);
    return final_word;
  }

  string best_word = "";
  double max_entropy = -1.0;

  remaining_words.begin();
  string current_word;

  while (remaining_words.next(current_word)) {
    double current_entropy = entropy(current_word, remaining_words);

    if (current_entropy > max_entropy) {
      max_entropy = current_entropy;
      best_word = current_word;
    }
  }

  return best_word;
}


//###############################################################################


/**
 * Computes the entropy of a candidate `guess`, given the
 * `remaining_words` that are still available to play.
 *
 * This implementation will be given to the students.
 */
double entropy(const string& guess, const BSTSet<string>& remaining_words) {
  unordered_map<string, int> pattern_counts;
  BSTSet<string> copy_remaining_words = remaining_words;

  copy_remaining_words.begin();
  string word;
  while (copy_remaining_words.next(word)) {
    string pattern = get_pattern(guess, word);
    pattern_counts[pattern]++;
  }

  double entropy = 0.0;
  double total = remaining_words.size();

  for (auto [pattern, count] : pattern_counts) {
    double p = (double)count / total;
    entropy -= p * log2(p);
  }

  return entropy;
}

/**
 * Given a set of words will continually recommend one based on the
 * principle of maximizing entropy.
 *
 * After each recommendation, user is responsible to enter the feedback
 * they got from playing a Wordle word in a Wordle game engine.
 *
 * Will stop once the user notifies that they won the Wordle game.
 * 
 * This implementation will be given to the students.
 */
void recommender(BSTSet<string>& remaining_words) {
  string user_feedback = "00000";
  int num_guesses = 0;
  string winning_messages[6] = {"Genius",   "Magnificent", "Impressive",
                                "Splendid", "Great",       "Phew"};

  while (!remaining_words.empty()) {
    string guess = recommend(remaining_words);
    cout << "Recommended guess: " << guess << endl;
    cout << "Enter feedback (2 for green, 1 for yellow, 0 for gray): ";
    string feedback;
    cin >> feedback;
    num_guesses++;

    if (feedback == "22222") {
      if (num_guesses <= 6) {
        cout << winning_messages[num_guesses - 1] << "! You won in "
             << num_guesses << " guesses!" << endl;
      } else {
        cout << "Sorry I made you lose :(" << endl;
      }
      break;
    }

    filter_words(remaining_words, guess, feedback);
    cout << "Remaining words: " << remaining_words.size() << endl;
    cout << "------------------------------------\n" << endl;
  }

  if (remaining_words.empty()) {
    cout << "Wait, we ran out of words!?!? This shouldn't have happened... SMH."
         << endl;
  }
}