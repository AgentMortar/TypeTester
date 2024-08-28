// TypeTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <cmath>
#include <thread>
#include <termcolor.hpp>

using namespace std;
using namespace std::chrono;

void countdown() {
    for (int i = 3; i > 0; --i) {
        cout << i << endl;
        this_thread::sleep_for(1s);  // Wait for 1 second
    }
    cout << termcolor::green << "GO!" << termcolor::reset << endl;
}

vector<string> populate_vector() {
    vector<string> sentences = {
        "The quick brown fox jumps over the lazy dog",
        "She sells seashells by the seashore",
        "The rain in Spain stays mainly in the plain",
        "A watched pot never boils",
        "Actions speak louder than words",
        "Better late than never, but never late is better",
        "The early bird catches the worm",
        "Practice makes perfect, so keep typing daily",
        "Rome wasn't built in a day, but it was built",
        "Every cloud has a silver lining",
        "You can't judge a book by its cover",
        "The pen is mightier than the sword",
        "Where there's smoke, there's fire",
        "A journey of a thousand miles begins with a single step",
        "Two wrongs don't make a right",
        "Honesty is the best policy, always and forever",
        "The grass is always greener on the other side",
        "Fortune favors the bold and the brave",
        "A picture is worth a thousand words",
        "Don't count your chickens before they hatch"
    };
    return sentences;
}

string pick_line(vector<string> sentences) {
    int sentences_size = sentences.size();
    srand((unsigned)time(NULL));
    int random = rand() % sentences_size;
    return (sentences[random]);
}

void attempt(string output_line) {
    double inaccuracies = 0;

    cout << "Your line is..." << endl << output_line << endl;

    auto start = high_resolution_clock::now();

    string attempt;
    getline(cin, attempt);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    std::stringstream ss1(output_line);
    std::string word1;
    int outputWordCount = 0;

    while (ss1 >> word1) {
        ++outputWordCount;
    }

    std::stringstream ss2(attempt);
    std::string word2;
    int attemptWordCount = 0;

    while (ss2 >> word2) {
        ++attemptWordCount;
    }

    cout << endl << "Your attempt:" << endl;
    int iter = 0;
    for (; (iter < attempt.length()) && (iter < output_line.length()); iter++) {
        if (attempt[iter] != output_line[iter]) {
            inaccuracies++;
            if (attempt[iter] == ' ') {
                cout << termcolor::red << "_" << termcolor::reset;
            }
            else {
                cout << termcolor::red << attempt[iter] << termcolor::reset;
            }
        }
        else {
            cout << attempt[iter];
        }
    }

    if (attempt.length() > output_line.length()) {
        inaccuracies += attempt.length() - output_line.length();
        for (; iter < attempt.length(); iter++) {
            cout << termcolor::red << attempt[iter] << termcolor::reset;
        }
    }
    else if (output_line.length() > attempt.length()) {
        inaccuracies += output_line.length() - attempt.length();
        for (; iter < output_line.length(); iter++) {
            cout << termcolor::red << "_" << termcolor::reset;
        }
    }

    cout << endl << endl << "Stats:" << endl;

    std::cout << "Time taken: "
        << duration.count() / 1000 << "." << duration.count() % 1000 << " seconds" << std::endl;

    std::cout << "WPM: " << (attemptWordCount * 60000.0) / duration.count() << std::endl;

    double accuracy = 100.0 * (static_cast<double>(output_line.length() - inaccuracies) / output_line.length());

    std::cout << "Accuracy: " << accuracy << "%" << std::endl;
}

int main() {
    vector<string> sentences = populate_vector();
    string output_line = pick_line(sentences);

    cout << "Press enter key when ready..." << endl;
    string a;
    getline(cin, a);
    countdown();

    attempt(output_line);

    cout << "Press Enter to exit..." << endl;
    getline(cin, a);
    return 1;
}
