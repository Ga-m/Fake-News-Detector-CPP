Fake News Detector in C++

A beginner-friendly C++ project that analyzes news text using keyword-based rules and classifies it as potentially misleading or more source-like.

Features

* Analyze news text
* Detect suspicious and source-related keywords
* View a sample dataset
* Evaluate predictions on labeled sample data
* Save and view analysis history

Technologies Used

* C++
* Standard C++ Library
* CSV dataset

How to Run

Compile the program using GCC:

g++ -std=c++17 main.cpp -o fake_news_detector

Run on Windows:

fake_news_detector.exe

Dataset

The sample_news.csv file contains sample news statements labeled as REAL or FAKE.

Important Note

This is an educational, rule-based prototype. It does not use a trained machine-learning model and cannot independently verify whether a news story is true.

Author

Gaurav Kumar
