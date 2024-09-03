#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <thread>
#include <termcolor.hpp>
#include <list>
#include <sstream> 


using namespace std;
using namespace std::chrono;


class Result;
class Typetest;

class Sentence {
private:
    string content;

public:
    Sentence(string content) : content(content) {}

    string getContent() const {
        return content;
    }

    int getLength() const {
        return content.length();
    }

    int wordCount() const {
        int count = 0;
        stringstream ss(content);
        string word;
        while (ss >> word) {
            ++count;
        }
        return count;
    }
};

class Result {
public:
    double timeTaken;
    double wpm;
    double accuracy;

    Result(double timeTaken = 0, double wpm = 0, double accuracy = 0)
        : timeTaken(timeTaken), wpm(wpm), accuracy(accuracy) {}


    Result operator+(const Result& other) const {
        return Result(timeTaken + other.timeTaken,
            wpm + other.wpm,
            accuracy + other.accuracy);
    }


    Result operator/(int scalar) const {
        return Result(timeTaken / scalar,
            wpm / scalar,
            accuracy / scalar);
    }

    void print() const {
        cout << "Time taken: " << timeTaken << " seconds" << endl;
        cout << "WPM: " << wpm << endl;
        cout << "Accuracy: " << accuracy << "%" << endl;
    }
};

class Typetest {
private:
    list<Result> results;  
    vector<Sentence> sentences;

public:
    Typetest() {
        sentences = populateSentences();
    }

    void run() {
        string a;
        char again_check = 'Y';
        cout << "Welcome to typetest!" << endl;

        while (again_check != 'N') {
            Sentence sentence = pickSentence();

            cout << "Press enter key when ready..." << endl;
            getline(cin, a);
            countdown();

            Result result = attempt(sentence);
            results.push_back(result);

            cout << "Would you like to try again? Y/N" << endl;
            std::cin >> again_check;
            getline(std::cin, a);  
        }

        Result averageResult = calculateAverage();
        cout << "Average stats per attempt:" << endl;
        averageResult.print();

        cout << "Press Enter to exit..." << endl;
        getline(cin, a);
    }

private:
    vector<Sentence> populateSentences() {
        vector<Sentence> sentences = {
            Sentence("The quick brown fox jumps over the lazy dog"),
            Sentence("She sells seashells by the seashore"),
            Sentence("The rain in Spain stays mainly in the plain"),
            Sentence("A watched pot never boils"),
            Sentence("Actions speak louder than words"),
            Sentence("Better late than never, but never late is better"),
            Sentence("The early bird catches the worm"),
            Sentence("Practice makes perfect, so keep typing daily"),
            Sentence("Rome wasn't built in a day, but it was built"),
            Sentence("Every cloud has a silver lining"),
            Sentence("You can't judge a book by its cover"),
            Sentence("The pen is mightier than the sword"),
            Sentence("Where there's smoke, there's fire"),
            Sentence("A journey of a thousand miles begins with a single step"),
            Sentence("Two wrongs don't make a right"),
            Sentence("Honesty is the best policy, always and forever"),
            Sentence("The grass is always greener on the other side"),
            Sentence("Fortune favors the bold and the brave"),
            Sentence("A picture is worth a thousand words"),
            Sentence("Don't count your chickens before they hatch")
        };
        return sentences;
    }

    Sentence pickSentence() {
        srand((unsigned)time(NULL));
        int random = rand() % sentences.size();
        return sentences[random];
    }

    void countdown() {
        for (int i = 3; i > 0; --i) {
            cout << i << endl;
            this_thread::sleep_for(1s);
        }
        cout << termcolor::green << "GO!" << termcolor::reset << endl;
    }

    Result attempt(Sentence sentence) {
        double inaccuracies = 0;

        cout << "Your line is..." << endl << sentence.getContent() << endl;

        auto start = high_resolution_clock::now();

        string attempt;
        getline(cin, attempt);

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);

        int outputWordCount = sentence.wordCount();
        int attemptWordCount = countWords(attempt);

        cout << endl << "Your attempt:" << endl;
        int iter = 0;
        for (; (iter < attempt.length()) && (iter < sentence.getLength()); iter++) {
            if (attempt[iter] != sentence.getContent()[iter]) {
                inaccuracies++;
                if (attempt[iter] == ' ') {
                    cout << termcolor::red << "_" << termcolor::reset;
                }
                else {
                    cout << termcolor::red << attempt[iter] << termcolor::reset;
                }
            }
            else {
                cout << termcolor::green << attempt[iter] << termcolor::reset;
            }
        }

        if (attempt.length() > sentence.getLength()) {
            inaccuracies += attempt.length() - sentence.getLength();
            for (; iter < attempt.length(); iter++) {
                cout << termcolor::red << attempt[iter] << termcolor::reset;
            }
        }
        else if (sentence.getLength() > attempt.length()) {
            inaccuracies += sentence.getLength() - attempt.length();
            for (; iter < sentence.getLength(); iter++) {
                cout << termcolor::red << "_" << termcolor::reset;
            }
        }

        cout << endl << endl << "Stats:" << endl;

        double timeTaken = duration.count() / 1000.0;
        double wpm = (attemptWordCount * 60000.0) / duration.count();
        double accuracy = 100.0 * (static_cast<double>(sentence.getLength() - inaccuracies) / sentence.getLength());

        Result result(timeTaken, wpm, accuracy);
        result.print();

        return result;
    }

    int countWords(const string& str) const {
        int count = 0;
        stringstream ss(str);
        string word;
        while (ss >> word) {
            ++count;
        }
        return count;
    }

    Result calculateAverage() const {
        if (results.empty()) return Result();

        Result total;
        for (const auto& result : results) {
            total = total + result;
        }
        return total / results.size();
    }
};

int main() {
    Typetest typetest;
    typetest.run();
    return 0;
}
