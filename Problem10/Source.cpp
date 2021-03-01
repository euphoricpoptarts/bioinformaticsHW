#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>

using pair = std::pair<std::string, std::string>;

std::list<pair> get_data() {
    std::ifstream f("rosalind_glob.txt");
    std::list<pair> data;
    if (f.is_open()) {
        bool begin = true;
        std::string line;
        std::string id = "";
        std::string dna = "";
        while (f >> line) {
            if (line.at(0) == '>') {
                if (!begin) {
                    //obviously can't do this at beginning of file
                    data.push_back(std::make_pair(id, dna));
                }
                id = line.substr(1);
                dna = "";
            }
            else {
                dna += line;
            }
            begin = false;
        }
        if (!f.eof()) {
            std::cerr << "Error occurred while reading!" << std::endl;
            data.clear();
        }
        else if (!begin) {
            //final data entry is not created by above loop
            data.push_back(std::make_pair(id, dna));
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

int main() {
    std::list<pair> data = get_data();

    std::string s1, s2;
    s1 = data.begin()->second;
    s2 = (++data.begin())->second;
    int rows = s1.length() + 1;
    int cols = s2.length() + 1;
    int* align_score = new int[rows * cols];
    int* align_path = new int[rows * cols];
    align_strings(s1, s2, align_score, align_path);
    int best = align_score[rows * cols - 1];
    std::cout << best << std::endl;
    delete[] align_score;
    delete[] align_path;
}