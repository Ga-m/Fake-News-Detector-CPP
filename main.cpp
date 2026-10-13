#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

using namespace std;

// Convert text to lowercase
string toLowerCase(string text)
{
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char c)
              {
                  return static_cast<char>(tolower(c));
              });

    return text;
}

// Remove spaces from the beginning and end
string trim(string text)
{
    size_t start = text.find_first_not_of(" \t\r\n");

    if (start == string::npos)
    {
        return "";
    }

    size_t end = text.find_last_not_of(" \t\r\n");

    return text.substr(start, end - start + 1);
}

// Count how many keywords appear in the text
int countKeywords(const string &text,
                  const vector<string> &keywords)
{
    int count = 0;

    for (const string &word : keywords)
    {
        if (text.find(word) != string::npos)
        {
            count++;
        }
    }

    return count;
}

// Save analysis results to a history file
void saveHistory(const string &news,
                 const string &result,
                 int fakeScore,
                 int reliableScore)
{
    ofstream file("data/history.txt", ios::app);

    if (!file.is_open())
    {
        cout << "\nWarning: History file could not be opened.\n";
        cout << "Please check whether the data folder exists.\n";
        return;
    }

    file << "News: " << news << "\n";
    file << "Prediction: " << result << "\n";
    file << "Fake indicators: " << fakeScore << "\n";
    file << "Reliable indicators: " << reliableScore << "\n";
    file << "----------------------------------------\n";

    file.close();
}

// Analyze news using simple keyword rules
void analyzeNews()
{
    string news;

    vector<string> fakeKeywords =
    {
        "shocking",
        "secret",
        "miracle",
        "100% guaranteed",
        "forward this",
        "share immediately",
        "you won't believe",
        "breaking!!!",
        "free money",
        "click here",
        "unbelievable",
        "no one is talking about"
    };

    vector<string> reliableKeywords =
    {
        "according to",
        "official statement",
        "research paper",
        "published study",
        "government website",
        "press release",
        "data shows",
        "court records",
        "verified report",
        "official report"
    };

    cout << "\n========== NEWS ANALYSIS ==========\n";
    cout << "Enter the news text:\n> ";

    getline(cin, news);

    if (trim(news).empty())
    {
        cout << "\nError: News text cannot be empty.\n";
        return;
    }

    string lowerNews = toLowerCase(news);

    int fakeScore = countKeywords(lowerNews, fakeKeywords);
    int reliableScore = countKeywords(lowerNews, reliableKeywords);

    string result;

    if (fakeScore > reliableScore && fakeScore > 0)
    {
        result = "POTENTIALLY MISLEADING";
    }
    else if (reliableScore > fakeScore && reliableScore > 0)
    {
        result = "MORE SOURCE-LIKE";
    }
    else
    {
        result = "UNCERTAIN";
    }

    cout << "\n========== ANALYSIS RESULT ==========\n";
    cout << "News: " << news << "\n";
    cout << "Suspicious keyword score: " << fakeScore << "\n";
    cout << "Source-related keyword score: " << reliableScore << "\n";
    cout << "Result: " << result << "\n";

    cout << "\nNote: This is a keyword-based prediction, not a verified fact.\n";

    saveHistory(news, result, fakeScore, reliableScore);

    cout << "=====================================\n";
}

// Display the sample dataset
void viewDataset()
{
    ifstream file("data/sample_news.csv");

    if (!file.is_open())
    {
        cout << "\nError: Could not open data/sample_news.csv\n";
        cout << "Check the project folder and CSV file name.\n";
        return;
    }

    string line;

    cout << "\n========== SAMPLE DATASET ==========\n";

    while (getline(file, line))
    {
        cout << line << "\n";
    }

    file.close();

    cout << "====================================\n";
}

// Evaluate predictions against the labelled dataset
void evaluateDataset()
{
    ifstream file("data/sample_news.csv");

    if (!file.is_open())
    {
        cout << "\nError: Dataset file not found.\n";
        return;
    }

    string line;

    int total = 0;
    int correct = 0;
    int skipped = 0;

    vector<string> fakeKeywords =
    {
        "shocking",
        "secret",
        "miracle",
        "100% guaranteed",
        "forward this",
        "share immediately",
        "you won't believe",
        "breaking!!!",
        "free money",
        "click here",
        "unbelievable",
        "no one is talking about"
    };

    vector<string> reliableKeywords =
    {
        "according to",
        "official statement",
        "research paper",
        "published study",
        "government website",
        "press release",
        "data shows",
        "court records",
        "verified report",
        "official report"
    };

    // Skip the CSV header
    if (!getline(file, line))
    {
        cout << "\nDataset is empty.\n";
        return;
    }

    while (getline(file, line))
    {
        if (trim(line).empty())
        {
            continue;
        }

        // Expected format: news text,label
        // The last comma separates the label from the text.
        size_t commaPosition = line.find_last_of(',');

        if (commaPosition == string::npos)
        {
            skipped++;
            continue;
        }

        string news = trim(line.substr(0, commaPosition));
        string actualLabel =
            toLowerCase(trim(line.substr(commaPosition + 1)));

        if (news.empty() ||
            (actualLabel != "fake" && actualLabel != "real"))
        {
            skipped++;
            continue;
        }

        string lowerNews = toLowerCase(news);

        int fakeScore = countKeywords(lowerNews, fakeKeywords);
        int reliableScore = countKeywords(lowerNews, reliableKeywords);

        string prediction;

        if (fakeScore > reliableScore && fakeScore > 0)
        {
            prediction = "fake";
        }
        else if (reliableScore > fakeScore && reliableScore > 0)
        {
            prediction = "real";
        }
        else
        {
            skipped++;
            continue;
        }

        total++;

        if (prediction == actualLabel)
        {
            correct++;
        }
    }

    file.close();

    cout << "\n========== DATASET EVALUATION ==========\n";
    cout << "Evaluated predictions: " << total << "\n";
    cout << "Correct predictions: " << correct << "\n";
    cout << "Skipped / uncertain rows: " << skipped << "\n";

    if (total > 0)
    {
        double accuracy = (correct * 100.0) / total;

        cout << fixed << setprecision(2);
        cout << "Accuracy on evaluated rows: "
             << accuracy << "%\n";
    }
    else
    {
        cout << "Accuracy cannot be calculated.\n";
        cout << "Add suitable labelled examples to the dataset.\n";
    }

    cout << "\nThis is a small rule-based evaluation, not an ML benchmark.\n";
    cout << "========================================\n";
}

// Display saved analysis history
void viewHistory()
{
    ifstream file("data/history.txt");

    if (!file.is_open())
    {
        cout << "\nNo history found yet.\n";
        cout << "Analyze some news first.\n";
        return;
    }

    string line;

    cout << "\n========== ANALYSIS HISTORY ==========\n";

    while (getline(file, line))
    {
        cout << line << "\n";
    }

    file.close();

    cout << "======================================\n";
}

// Display project information
void projectInformation()
{
    cout << "\n========== PROJECT INFORMATION ==========\n";
    cout << "Project Name : Fake News Detector\n";
    cout << "Language     : C++\n";
    cout << "Technique    : Keyword-based rules\n";
    cout << "Dataset      : CSV file\n";
    cout << "Storage      : Text file for history\n";

    cout << "\nFeatures:\n";
    cout << "1. News text analysis\n";
    cout << "2. Sample dataset display\n";
    cout << "3. Dataset evaluation\n";
    cout << "4. Save analysis history\n";
    cout << "5. View previous analysis\n";

    cout << "\nLimitation:\n";
    cout << "The program does not understand news context.\n";
    cout << "Its predictions must not be treated as verified facts.\n";
    cout << "=========================================\n";
}

// Main function
int main()
{
    string choice;

    cout << "========================================\n";
    cout << "       FAKE NEWS DETECTOR IN C++\n";
    cout << "========================================\n";

    while (true)
    {
        cout << "\n--------------- MAIN MENU ---------------\n";
        cout << "1. Analyze News\n";
        cout << "2. View Sample Dataset\n";
        cout << "3. Project Information\n";
        cout << "4. Evaluate Dataset\n";
        cout << "5. View Analysis History\n";
        cout << "0. Exit\n";
        cout << "-----------------------------------------\n";
        cout << "Enter your choice: ";

        getline(cin, choice);

        if (choice == "1")
        {
            analyzeNews();
        }
        else if (choice == "2")
        {
            viewDataset();
        }
        else if (choice == "3")
        {
            projectInformation();
        }
        else if (choice == "4")
        {
            evaluateDataset();
        }
        else if (choice == "5")
        {
            viewHistory();
        }
        else if (choice == "0")
        {
            cout << "\nThank you for using Fake News Detector!\n";
            break;
        }
        else
        {
            cout << "\nInvalid choice! Enter a number from 0 to 5.\n";
        }
    }

    return 0;
}