#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_set>

using set = std::unordered_set<std::string>;
using pair = std::pair<std::string, std::string>;

std::string rcomp(std::string data) {
    std::string output;
    for (auto c = data.rbegin(); c != data.rend(); ++c) {
        char x = *c;
        if (x == 'A') {
            output += 'T';
        }
        else if (x == 'C') {
            output += 'G';
        }
        else if (x == 'G') {
            output += 'C';
        }
        else if (x == 'T') {
            output += 'A';
        }
    }
    return output;
}

std::list<std::string> get_data() {
    std::ifstream f("rosalind_dbru.txt");
    std::list<std::string> data;
    set uniques;
    if (f.is_open()) {
        bool begin = true;
        std::string line;
        std::string id = "";
        std::string dna = "";
        while (f >> line) {
            std::string rc = rcomp(line);
            if (uniques.count(line) == 0) {
                uniques.insert(line);
                data.push_back(line);
            }
            if (uniques.count(rc) == 0) {
                uniques.insert(rc);
                data.push_back(rc);
            }
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

std::list<pair> make_graph(std::list<std::string> data) {
    std::list<pair> graph;
    for (std::string kmer : data) {
        std::string first = kmer.substr(0, kmer.size() - 1);
        std::string second = kmer.substr(1);
        graph.push_back(std::make_pair(first, second));
    }
    return graph;
}

int main() {
    std::list<std::string> data = get_data();
    std::list<pair> graph = make_graph(data);
    std::ofstream out("out.txt");
    for (pair p : graph) {
        out << "(" << p.first << ", " << p.second << ")" << std::endl;
    }
    out.close();
    return 0;
}