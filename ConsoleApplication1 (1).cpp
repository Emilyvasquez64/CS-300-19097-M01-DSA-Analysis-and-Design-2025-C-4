// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Emily Vasquez
//CS-300-19097-M01 DSA: Analysis and Design 2025
//7-1 Submit Project Two
//Course Instructor: Prof. Farley

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>
//#include <unistd.h>

using namespace std;

// Trim whitespace from both ends of a string, this will prevent errors if user enters "1 " or " 1" instead of "1"
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, last - first + 1);
}

// Structure to represent a course, this is how the CSV file is structured and should be read
struct Course {
    string number;
    string name;
    vector<string> prerequisites;
};

// Function to load courses from a file into the program
map<string, Course> loadCourses(const string& filename) {
    map<string, Course> courses;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Couldn't open file " << filename << endl;
        perror("Reason");
        return courses;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Course course;
        vector<string> tokens;

        // Split line by commas
        while (getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // At least course number and course title required
        if (tokens.size() >= 2) {
            course.number = trim(tokens[0]);
            course.name = trim(tokens[1]);

            // Remaining tokens are prerequisites
            for (size_t i = 2; i < tokens.size(); i++) {
                course.prerequisites.push_back(trim(tokens[i]));
            }

            courses[course.number] = course;
        }
    }

    file.close();
    cout << "Courses loaded successfully." << endl;
    return courses;
}

// Function to print sorted list of all courses
void printCourseList(const map<string, Course>& courses) {
    if (courses.empty()) {
        cout << "Error: No data loaded yet. Please load a file first." << endl;
        return;
    }

    vector<string> courseNumbers;
    for (const auto& pair : courses) {
        courseNumbers.push_back(pair.first);
    }

    sort(courseNumbers.begin(), courseNumbers.end());

    cout << "\nCourse List:" << endl;
    for (const string& num : courseNumbers) {
        cout << num << ", " << courses.at(num).name << endl;
    }
}


// Function to print information about a specific course
void printCourseInfo(const map<string, Course>& courses, const string& courseNum) {
    auto it = courses.find(courseNum);
    if (it == courses.end()) {
        cout << "Error: Course " << courseNum << " not found." << endl;
        return;
    }

    const Course& course = it->second;
    cout << "\n" << course.number << ", " << course.name << endl;

    // Filter out empty prerequisite strings
    vector<string> validPrereqs;
    for (const auto& prereq : course.prerequisites) {
        if (!prereq.empty()) {
            validPrereqs.push_back(prereq);
        }
    }

    if (validPrereqs.empty()) {
        cout << "Prerequisites: N/A" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < validPrereqs.size(); i++) {
            const string& prereq = validPrereqs[i];
            cout << prereq;

            auto preIt = courses.find(prereq);
            if (preIt != courses.end()) {
                cout << " (" << preIt->second.name << ")";
            }

            if (i < validPrereqs.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

int main() {
    // Print current working directory
    //char cwd[10000];
   // if (_getcwd(cwd, sizeof(cwd)) != nullptr) {
     //   cout << "Current Working Directory: " << cwd << endl;
   // }
   // else {
    //    perror("getcwd() error");
   //s }

    map<string, Course> courses;
    int choice = 0;
    string filename;
    string courseNum;

    while (choice != 9) {
        cout << "\nWelcome to the course planner." << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << "What would you like to do? ";

        cin >> choice;
        if (cin.fail()) {
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            cout << "Input error: please enter a number from the menu." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard leftover newline

        switch (choice) {
        case 1:
            cout << "Enter the file name: ";
            getline(cin, filename);
            courses = loadCourses(filename);
            break;

        case 2:
            printCourseList(courses);
            break;

        case 3:
            cout << "What course do you want to know about? (e.g., CSCI101): ";
            getline(cin, courseNum);

            // Convert input to uppercase for case-insensitive matching
            transform(courseNum.begin(), courseNum.end(), courseNum.begin(),
                [](unsigned char c) { return toupper(c); });

            printCourseInfo(courses, courseNum);
            break;

        case 9:
            cout << "Thank you for using the course planner! Goodbye!" << endl;
            break;

        default:
            cout << choice << " is not a valid option. Please select an option from the menu." << endl;
            break;
        }
    }

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
