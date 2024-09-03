#include <iostream>
#include <list>
#include <string>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <thread>
#include <termcolor.hpp>

using namespace std::chrono;

///testingggg

class TypingStats {
private:
    double inaccuracies;
    int attemptWordCount;
    int outputWordCount;
    double timeTaken; 
    double wpm;
    double accuracy;

public:
    TypingStats() : inaccuracies(0), attemptWordCount(0), outputWordCount(0), timeTaken(0), wpm(0), accuracy(0) {}

    void calculateStats(const std::string& output_line, const std::string& attempt, std::chrono::milliseconds duration) {
        inaccuracies = 0;
        timeTaken = duration.count() / 1000.0;

        std::stringstream ss1(output_line);
        std::string word1;
        outputWordCount = 0;

        while (ss1 >> word1) {
            ++outputWordCount;
        }

        std::stringstream ss2(attempt);
        std::string word2;
        attemptWordCount = 0;

        while (ss2 >> word2) {
            ++attemptWordCount;
        }

        for (int iter = 0; (iter < attempt.length()) && (iter < output_line.length()); iter++) {
            if (attempt[iter] != output_line[iter]) {
                inaccuracies++;
            }
        }

        if (attempt.length() > output_line.length()) {
            inaccuracies += attempt.length() - output_line.length();
        }
        else if (output_line.length() > attempt.length()) {
            inaccuracies += output_line.length() - attempt.length();
        }

        wpm = (attemptWordCount * 60.0) / timeTaken;
        accuracy = 100.0 * (static_cast<double>(output_line.length() - inaccuracies) / output_line.length());
    }

    void printStats() const {
        std::cout << "Time taken: " << timeTaken << " seconds" << std::endl;
        std::cout << "WPM: " << wpm << std::endl;
        std::cout << "Accuracy: " << accuracy << "%" << std::endl;
    }
};

class SentenceList {
private:
    std::list<std::string> sentences;

public:
    void addSentence(const std::string& sentence) {
        sentences.push_back(sentence);
    }

    std::string getRandomSentence() {
        int size = sentences.size();
        int randomIndex = rand() % size;
        std::cout << "index is: " << randomIndex;
        auto it = std::next(sentences.begin(), randomIndex);
        return *it;
    }

    std::string operator[](int index) {
        auto it = std::next(sentences.begin(), index);
        return *it;
    }

    int size() const {
        return sentences.size();
    }
};

void countdown() {
    for (int i = 3; i > 0; --i) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(1s);  
    }
    std::cout << termcolor::green << "GO!" << termcolor::reset << std::endl;
}

SentenceList populate_list() {
    SentenceList sentences;
    sentences.addSentence("The quick brown fox jumps over the lazy dog");
    sentences.addSentence("She sells seashells by the seashore");
    sentences.addSentence("The rain in Spain stays mainly in the plain");
    sentences.addSentence("A watched pot never boils");
    sentences.addSentence("Actions speak louder than words");
    sentences.addSentence("Better late than never, but never late is better");
    sentences.addSentence("The early bird catches the worm");
    sentences.addSentence("Practice makes perfect, so keep typing daily");
    sentences.addSentence("Rome wasn't built in a day, but it was built");
    sentences.addSentence("Every cloud has a silver lining");
    sentences.addSentence("You can't judge a book by its cover");
    sentences.addSentence("The pen is mightier than the sword");
    sentences.addSentence("Where there's smoke, there's fire");
    sentences.addSentence("A journey of a thousand miles begins with a single step");
    sentences.addSentence("Two wrongs don't make a right");
    sentences.addSentence("Honesty is the best policy, always and forever");
    sentences.addSentence("The grass is always greener on the other side");
    sentences.addSentence("Fortune favors the bold and the brave");
    sentences.addSentence("A picture is worth a thousand words");
    sentences.addSentence("Don't count your chickens before they hatch");
    return sentences;
}


void attempt(const std::string& output_line) {
    std::cout << "Your line is..." << std::endl << output_line << std::endl;

    auto start = high_resolution_clock::now();

    std::string attempt;
    getline(std::cin, attempt);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    TypingStats stats;
    stats.calculateStats(output_line, attempt, duration);

    std::cout << std::endl << "Your attempt:" << std::endl;
    for (int iter = 0; (iter < attempt.length()) && (iter < output_line.length()); iter++) {
        if (attempt[iter] != output_line[iter]) {
            if (attempt[iter] == ' ') {
                std::cout << termcolor::red << "_" << termcolor::reset;
            }
            else {
                std::cout << termcolor::red << attempt[iter] << termcolor::reset;
            }
        }
        else {
            std::cout << termcolor::green << attempt[iter] << termcolor::reset;
        }
    }

    if (attempt.length() > output_line.length()) {
        for (int iter = output_line.length(); iter < attempt.length(); iter++) {
            std::cout << termcolor::red << attempt[iter] << termcolor::reset;
        }
    }
    else if (output_line.length() > attempt.length()) {
        for (int iter = attempt.length(); iter < output_line.length(); iter++) {
            std::cout << termcolor::red << "_" << termcolor::reset;
        }
    }

    std::cout << std::endl << std::endl << "Stats:" << std::endl;
    stats.printStats();
}

int main() {

    srand(time(NULL));
    SentenceList sentences = populate_list();
    std::string output_line = sentences.getRandomSentence();

    std::cout << "Press enter key when ready..." << std::endl;
    std::string a;
    getline(std::cin, a);
    countdown();

    attempt(output_line);

    std::cout << "Press Enter to exit..." << std::endl;
    getline(std::cin, a);
    return 1;
}
