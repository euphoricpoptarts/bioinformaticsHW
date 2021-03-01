#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>
#include <limits>
#include <exception>

using pair = std::pair<std::string, std::string>;

//divide by 2 to prevent underflow
const int neg_inf = std::numeric_limits<int>::min() / 2;

std::list<pair> get_data() {
    std::ifstream f("rosalind_laff.txt");
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

int getOffset(int rows, int cols, int r, int c, int m) {
    return m * rows * cols + r * cols + c;
}

int* getPos(int* arr, int rows, int cols, int r, int c, int m) {
    return (arr + getOffset(rows, cols, r, c, m));
}

void align_strings(std::string s1, std::string s2, int* align_score, int* align_path) {
    int* cost_matrix = align_matrix();
    int rows = s1.length() + 1;
    int cols = s2.length() + 1;
    int gap_start = -11;
    int gap_extend = -1;
    //need to set things to zero instead of -inf because 0 is the minimum value for local alignments
    *getPos(align_score, rows, cols, 0, 0, 0) = 0;
    *getPos(align_path, rows, cols, 0, 0, 0) = getOffset(rows, cols, 0, 0, 0);
    *getPos(align_score, rows, cols, 0, 0, 1) = neg_inf;
    *getPos(align_path, rows, cols, 0, 0, 1) = getOffset(rows, cols, 0, 0, 1);
    *getPos(align_score, rows, cols, 0, 0, 2) = neg_inf;
    *getPos(align_path, rows, cols, 0, 0, 2) = getOffset(rows, cols, 0, 0, 2);
    for (int i = 1; i < rows; i++) {
        *getPos(align_score, rows, cols, i, 0, 0) = 0;//neg_inf;
        *getPos(align_path, rows, cols, i, 0, 0) = getOffset(rows, cols, i, 0, 0);
        //alignment between i characters of s1 and 0 characters of s2 ending in gap of s1
        *getPos(align_score, rows, cols, i, 0, 1) = neg_inf;
        *getPos(align_path, rows, cols, i, 0, 1) = getOffset(rows, cols, i, 0, 1);
        //alignment between i characters of s1 and 0 characters of s2 ending in gap of s2
        *getPos(align_score, rows, cols, i, 0, 2) = gap_start + (i-1)*gap_extend;
        *getPos(align_path, rows, cols, i, 0, 2) = getOffset(rows, cols, i, 0, 2);
    }
    for (int i = 1; i < cols; i++) {
        *getPos(align_score, rows, cols, 0, i, 0) = 0;//neg_inf;
        *getPos(align_path, rows, cols, 0, i, 0) = getOffset(rows, cols, 0, i, 0);
        //alignment between 0 characters of s1 and i characters of s2 ending in gap of s1
        *getPos(align_score, rows, cols, 0, i, 1) = gap_start + (i-1)*gap_extend;
        *getPos(align_path, rows, cols, 0, i, 1) = getOffset(rows, cols, 0, i, 1);
        //alignment between 0 characters of s1 and i characters of s2 ending in gap of s2
        *getPos(align_score, rows, cols, 0, i, 2) = neg_inf;
        *getPos(align_path, rows, cols, 0, i, 2) = getOffset(rows, cols, 0, i, 2);
    }
    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            char a = s1.at(i - 1);
            char b = s2.at(j - 1);
            int update[4];
            int align_cost = cost_matrix[a * 256 + b];

            //M
            update[0] = *getPos(align_score, rows, cols, i - 1, j - 1, 0) + align_cost;
            update[1] = *getPos(align_score, rows, cols, i - 1, j - 1, 1) + align_cost;
            update[2] = *getPos(align_score, rows, cols, i - 1, j - 1, 2) + align_cost;
            update[3] = 0;
            int backtrace[4] = { 
                getOffset(rows, cols, i - 1, j - 1, 0),
                getOffset(rows, cols, i - 1, j - 1, 1),
                getOffset(rows, cols, i - 1, j - 1, 2),
                getOffset(rows, cols, i, j, 0),
            };
            auto max = std::max_element(update, update + 4);
            int argmax = std::distance(update, max);
            int best = *max;
            *getPos(align_score, rows, cols, i, j, 0) = best;
            //don't really care about ties
            *getPos(align_path, rows, cols, i, j, 0) = backtrace[argmax];

            //X
            update[0] = *getPos(align_score, rows, cols, i, j - 1, 0) + gap_start + gap_extend;
            update[1] = *getPos(align_score, rows, cols, i, j - 1, 1) + gap_extend;
            update[2] = *getPos(align_score, rows, cols, i, j - 1, 2) + gap_start + gap_extend;
            update[3] = 0;
            backtrace[0] = getOffset(rows, cols, i, j - 1, 0);
            backtrace[1] = getOffset(rows, cols, i, j - 1, 1);
            backtrace[2] = getOffset(rows, cols, i, j - 1, 2);
            backtrace[3] = getOffset(rows, cols, i, j, 1);
            max = std::max_element(update, update + 3);
            argmax = std::distance(update, max);
            best = *max;
            *getPos(align_score, rows, cols, i, j, 1) = best;
            *getPos(align_path, rows, cols, i, j, 1) = backtrace[argmax];

            //Y
            update[0] = *getPos(align_score, rows, cols, i - 1, j, 0) + gap_start + gap_extend;
            update[1] = *getPos(align_score, rows, cols, i - 1, j, 1) + gap_start + gap_extend;
            update[2] = *getPos(align_score, rows, cols, i - 1, j, 2) + gap_extend;
            update[3] = 0;
            backtrace[0] = getOffset(rows, cols, i - 1, j, 0);
            backtrace[1] = getOffset(rows, cols, i - 1, j, 1);
            backtrace[2] = getOffset(rows, cols, i - 1, j, 2);
            backtrace[3] = getOffset(rows, cols, i, j, 2);
            max = std::max_element(update, update + 3);
            argmax = std::distance(update, max);
            best = *max;
            *getPos(align_score, rows, cols, i, j, 2) = best;
            *getPos(align_path, rows, cols, i, j, 2) = backtrace[argmax];
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
    std::cout << "Rows: " << rows << "; Cols: " << cols << std::endl;
    //times 3 for the 3 matrices X, Y, and M
    int* align_score, *align_path;
    try {
        align_score = new int[3 * rows * cols];
        align_path = new int[3 * rows * cols];
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    align_strings(s1, s2, align_score, align_path);
    auto max = std::max_element(align_score, align_score + 3 * rows * cols);
    int best = *max;
    int argmax = std::distance(align_score, max);
    int current = argmax;
    int next = align_path[argmax];
    while (next != current) {
        current = next;
        next = align_path[next];
    }
    int s1_start = (current % (rows*cols)) / cols;
    int s1_end = (argmax % (rows*cols)) / cols;
    int s2_start = (current % (rows * cols)) % cols;
    int s2_end = (argmax % (rows * cols)) % cols;
    std::cout << best << std::endl;
    //for (int c = 0; c < 3; c++) {
    //    std::cout << c << std::endl;
    //    for (int i = 0; i < rows; i++) {
    //        for (int j = 0; j < cols; j++) {
    //            std::cout << *getPos(align_score, rows, cols, i, j, c) << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}
    std::cout << s1.substr(s1_start, s1_end - s1_start) << std::endl;
    std::cout << s2.substr(s2_start, s2_end - s2_start) << std::endl;
    delete[] align_score;
    delete[] align_path;
}