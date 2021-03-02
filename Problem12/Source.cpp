#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>

using pair = std::pair<std::string, std::string>;

std::list<std::string> get_data() {
    std::ifstream f("rosalind_ba5l.txt");
    std::list<std::string> data;
    if (f.is_open()) {
        bool begin = true;
        std::string line;
        std::string id = "";
        std::string dna = "";
        while (f >> line) {
            data.push_back(line);
        }
        if (!f.eof()) {
            std::cerr << "Error occurred while reading!" << std::endl;
            data.clear();
        }
        f.close();
    }
    else {
        std::cerr << "Could not find file!" << std::endl;
    }
    return data;
}

std::string align_string = "\
   A  C  D  E  F  G  H  I  K  L  M  N  P  Q  R  S  T  V  W  Y\
A  4  0 -2 -1 -2  0 -2 -1 -1 -1 -1 -2 -1 -1 -1  1  0  0 -3 -2\
C  0  9 -3 -4 -2 -3 -3 -1 -3 -1 -1 -3 -3 -3 -3 -1 -1 -1 -2 -2\
D -2 -3  6  2 -3 -1 -1 -3 -1 -4 -3  1 -1  0 -2  0 -1 -3 -4 -3\
E -1 -4  2  5 -3 -2  0 -3  1 -3 -2  0 -1  2  0  0 -1 -2 -3 -2\
F -2 -2 -3 -3  6 -3 -1  0 -3  0  0 -3 -4 -3 -3 -2 -2 -1  1  3\
G  0 -3 -1 -2 -3  6 -2 -4 -2 -4 -3  0 -2 -2 -2  0 -2 -3 -2 -3\
H -2 -3 -1  0 -1 -2  8 -3 -1 -3 -2  1 -2  0  0 -1 -2 -3 -2  2\
I -1 -1 -3 -3  0 -4 -3  4 -3  2  1 -3 -3 -3 -3 -2 -1  3 -3 -1\
K -1 -3 -1  1 -3 -2 -1 -3  5 -2 -1  0 -1  1  2  0 -1 -2 -3 -2\
L -1 -1 -4 -3  0 -4 -3  2 -2  4  2 -3 -3 -2 -2 -2 -1  1 -2 -1\
M -1 -1 -3 -2  0 -3 -2  1 -1  2  5 -2 -2  0 -1 -1 -1  1 -1 -1\
N -2 -3  1  0 -3  0  1 -3  0 -3 -2  6 -2  0  0  1  0 -3 -4 -2\
P -1 -3 -1 -1 -4 -2 -2 -3 -1 -3 -2 -2  7 -1 -2 -1 -1 -2 -4 -3\
Q -1 -3  0  2 -3 -2  0 -3  1 -2  0  0 -1  5  1  0 -1 -2 -2 -1\
R -1 -3 -2  0 -3 -2  0 -3  2 -2 -1  0 -2  1  5 -1 -1 -3 -3 -2\
S  1 -1  0  0 -2  0 -1 -2  0 -2 -1  1 -1  0 -1  4  1 -2 -3 -2\
T  0 -1 -1 -1 -2 -2 -2 -1 -1 -1 -1  0 -1 -1 -1  1  5  0 -2 -2\
V  0 -1 -3 -2 -1 -3 -3  3 -2  1  1 -3 -2 -2 -3 -2  0  4 -3 -1\
W -3 -2 -4 -3  1 -2 -2 -3 -3 -2 -1 -4 -4 -2 -3 -3 -2 -3 11  2\
Y -2 -2 -3 -2  3 -3  2 -1 -2 -1 -1 -2 -3 -1 -2 -2 -2 -1  2  7";

int* align_matrix() {
    //hopefully won't need to initialize whole matrix
    int* matrix = new int[256 * 256];
    std::stringstream data;
    data << align_string;
    char cols[20];
    for (int i = 0; i < 20; i++) {
        data >> cols[i];
    }
    for (int i = 0; i < 20; i++) {
        char row;
        data >> row;
        for (int j = 0; j < 20; j++) {
            char col = cols[j];
            int score = 0;
            data >> score;
            matrix[row * 256 + col] = score;
        }
    }
    return matrix;
}

void align_strings(std::string s1, std::string s2, int* align_score, int* align_path) {
    int* cost_matrix = align_matrix();
    int rows = s1.length() + 1;
    int cols = s2.length() + 1;
    int gap = -5;
    align_score[0] = 0;
    align_path[0] = 0;
    for (int i = 1; i < rows; i++) {
        align_score[i * cols] = i * gap;
        align_path[i * cols] = (i - 1) * cols;
    }
    for (int i = 1; i < cols; i++) {
        align_score[i] = i * gap;
        align_path[i] = i - 1;
    }
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            int update[3];
            update[0] = align_score[(i - 1) * cols + j] + gap;
            update[1] = align_score[i * cols + (j - 1)] + gap;
            char a = s1.at(i - 1);
            char b = s2.at(j - 1);
            update[2] = align_score[(i - 1) * cols + (j - 1)] + cost_matrix[a * 256 + b];
            int backtrace[3] = { (i - 1) * cols + j, i * cols + (j - 1), (i - 1) * cols + (j - 1) };
            auto max = std::max_element(update, update + 3);
            int argmax = std::distance(update, max);
            int best = *max;
            align_score[i * cols + j] = best;
            //don't really care about ties
            align_path[i * cols + j] = backtrace[argmax];
        }
    }
    delete[] cost_matrix;
}

int* align_string_scores(std::string s1, std::string s2) {
    int* cost_matrix = align_matrix();
    int rows = s1.length() + 1;
    int cols = s2.length() + 1;
    int gap = -5;
    int* prev_score = new int[cols];
    int* next_score = new int[cols];
    prev_score[0] = 0;
    for (int i = 1; i < cols; i++) {
        prev_score[i] = i * gap;
    }
    for (int i = 1; i < rows; i++) {
        next_score[0] = i * gap;
        for (int j = 1; j < cols; j++) {
            int update[3];
            update[0] = prev_score[j] + gap;
            update[1] = next_score[(j - 1)] + gap;
            char a = s1.at(i - 1);
            char b = s2.at(j - 1);
            update[2] = prev_score[j - 1] + cost_matrix[a * 256 + b];
            auto max = std::max_element(update, update + 3);
            int best = *max;
            next_score[j] = best;
        }
        for (int j = 0; j < cols; j++) {
            prev_score[j] = next_score[j];
        }
    }
    delete[] cost_matrix;
    delete[] prev_score;
    return next_score;
}

pair align_string_alignment(std::string s1, std::string s2, int& score) {
    int rows = s1.length() + 1;
    int cols = s2.length() + 1;
    int* align_scores = new int[rows * cols];
    int* align_paths = new int[rows * cols];
    align_strings(s1, s2, align_scores, align_paths);
    int current = rows * cols - 1;
    int next = align_paths[current];
    std::string alignment1 = "";
    std::string alignment2 = "";
    auto s1_iter = s1.rbegin();
    auto s2_iter = s2.rbegin();
    while (next != current) {
        //if the column index decreased add a character to the alignment
        //otherwise add a '-'
        if ((next % cols) < (current % cols)) {
            alignment2 = *(s2_iter++) + alignment2;
        }
        else {
            alignment2 = '-' + alignment2;
        }
        if ((next / cols) < (current / cols)) {
            alignment1 = *(s1_iter++) + alignment1;
        }
        else {
            alignment1 = '-' + alignment1;
        }
        current = next;
        next = align_paths[current];
    }
    score = align_scores[rows * cols - 1];
    delete[] align_scores;
    delete[] align_paths;
    return { alignment1, alignment2 };
}

pair hirschberg(std::string s1, std::string s2, int& score) {
    if (s1.length() <= 2 || s2.length() <= 2) {
        return align_string_alignment(s1, s2, score);
    }
    int midX = s1.length() / 2;
    std::string s1_left = s1.substr(0, midX);
    std::string s1_right = s1.substr(midX);
    int* align_left = align_string_scores(s1_left, s2);
    std::string s1_right_rev = std::string(s1_right.rbegin(), s1_right.rend());
    std::string s2_rev = std::string(s2.rbegin(), s2.rend());
    int* align_right = align_string_scores(s1_right_rev, s2_rev);
    std::reverse(align_right, align_right + s2.length() + 1);
    for (int i = 0; i < s2.length() + 1; i++) {
        align_left[i] = align_left[i] + align_right[i];
    }
    auto max = std::max_element(align_left, align_left + s2.length() + 1);
    int ymid = std::distance(align_left, max);
    score = *max;
    delete[] align_left;
    delete[] align_right;
    std::string s2_left = s2.substr(0, ymid);
    std::string s2_right = s2.substr(ymid);
    int dummy_score1 = 0, dummy_score2 = 0;
    pair p1 = hirschberg(s1_left, s2_left, dummy_score1);
    pair p2 = hirschberg(s1_right, s2_right, dummy_score2);
    return { p1.first + p2.first, p1.second + p2.second };
}

void sanity(std::string s1, std::string s2) {
    int score = 0;
    int gap = -5;
    int* cost_matrix = align_matrix();
    for (int i = 0; i < s1.length(); i++) {
        char a = s1.at(i);
        char b = s2.at(i);
        if (a == '-' || b == '-') {
            score += gap;
        }
        else {
            score += cost_matrix[a * 256 + b];
        }
    }
    std::cout << score << std::endl;
}

int main() {
    std::list<std::string> data = get_data();

    std::string s1, s2;
    s1 = *data.begin();
    s2 = *(++data.begin());
    
    int score = 0;
    pair alignment = hirschberg(s1, s2, score);
    std::cout << score << std::endl;
    std::cout << alignment.first << std::endl;
    std::cout << alignment.second << std::endl;
}