// CS 300 Project Two
// Brian Minor
// ABCU Advising Assistance Program
// Data Structure Used: Binary Search Tree

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// ---------------- COURSE STRUCT ----------------

struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// ---------------- BST NODE ----------------

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(const Course& c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// ---------------- BINARY SEARCH TREE ----------------

class BinarySearchTree {
private:
    Node* root;

    // Insert helper (recursive)
    Node* insertNode(Node* node, const Course& course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        }
        else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        }
        else {
            cout << "Duplicate course number found: "
                << course.courseNumber << endl;
        }

        return node;
    }

    // In-order traversal helper
    void inOrder(Node* node) const {
        if (node == nullptr) return;

        inOrder(node->left);
        cout << node->course.courseNumber << ", "
            << node->course.title << endl;
        inOrder(node->right);
    }

    // Search helper
    Course* searchNode(Node* node, const string& key) const {
        if (node == nullptr) return nullptr;

        if (key == node->course.courseNumber) {
            return &(node->course);
        }

        if (key < node->course.courseNumber) {
            return searchNode(node->left, key);
        }
        return searchNode(node->right, key);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(const Course& course) {
        root = insertNode(root, course);
    }

    void PrintAllCourses() const {
        cout << "Here is a sample schedule:" << endl;
        inOrder(root);
    }

    Course* Search(const string& key) const {
        return searchNode(root, key);
    }
};

// ---------------- HELPER FUNCTIONS ----------------

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

// ---------------- FILE LOAD ----------------

bool loadCourses(const string& filename, BinarySearchTree& bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    vector<Course> tempCourses;
    vector<string> allNumbers;

    string line;

    // First pass — parse
    while (getline(file, line)) {
        if (trim(line).empty()) continue;

        stringstream ss(line);
        vector<string> tokens;
        string token;

        while (getline(ss, token, ',')) {
            tokens.push_back(trim(token));
        }

        if (tokens.size() < 2) {
            cout << "Format error in file." << endl;
            return false;
        }

        Course c;
        c.courseNumber = toUpper(tokens[0]);
        c.title = tokens[1];

        for (size_t i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                c.prerequisites.push_back(toUpper(tokens[i]));
            }
        }

        tempCourses.push_back(c);
        allNumbers.push_back(c.courseNumber);
    }

    // Second pass — validate prerequisites
    for (const Course& c : tempCourses) {
        for (const string& p : c.prerequisites) {
            if (find(allNumbers.begin(), allNumbers.end(), p)
                == allNumbers.end()) {
                cout << "Invalid prerequisite: " << p << endl;
                return false;
            }
        }
    }

    // Insert into BST
    for (const Course& c : tempCourses) {
        bst.Insert(c);
    }

    cout << "Data loaded successfully." << endl;
    return true;
}

// ---------------- PRINT COURSE DETAILS ----------------

void printCourseDetails(BinarySearchTree& bst, const string& number) {
    string key = toUpper(trim(number));
    Course* c = bst.Search(key);

    if (c == nullptr) {
        cout << "Course not found." << endl;
        return;
    }

    cout << c->courseNumber << ", " << c->title << endl;

    if (c->prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";

    for (size_t i = 0; i < c->prerequisites.size(); i++) {
        Course* pre = bst.Search(c->prerequisites[i]);
        if (pre != nullptr) {
            cout << pre->courseNumber << ", " << pre->title;
        }
        else {
            cout << c->prerequisites[i];
        }

        if (i + 1 < c->prerequisites.size()) cout << " | ";
    }
    cout << endl;
}

// ---------------- MAIN MENU ----------------

int main() {
    BinarySearchTree bst;
    bool loaded = false;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        cout << endl;
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit" << endl;
        cout << "What would you like to do? ";

        string input;
        getline(cin, input);

        if (input == "1") {
            cout << "Enter file name: ";
            string file;
            getline(cin, file);
            loaded = loadCourses(file, bst);
        }
        else if (input == "2") {
            if (!loaded) {
                cout << "Please load data first." << endl;
            }
            else {
                bst.PrintAllCourses();
            }
        }
        else if (input == "3") {
            if (!loaded) {
                cout << "Please load data first." << endl;
            }
            else {
                cout << "Enter course number: ";
                string num;
                getline(cin, num);
                printCourseDetails(bst, num);
            }
        }
        else if (input == "9") {
            cout << "Thank you for using the course planner!" << endl;
            break;
        }
        else {
            cout << input << " is not a valid option." << endl;
        }
    }

    return 0;
}
