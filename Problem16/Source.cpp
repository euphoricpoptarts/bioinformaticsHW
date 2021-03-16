#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_set>
#include <unordered_map>

using pair = std::pair<std::string, std::string>;
using set = std::unordered_set<pair>;
using map = std::unordered_map<pair, std::list<pair>>;

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

std::list<pair> get_data() {
    std::ifstream f("rosalind_pcov.txt");
    std::list<pair> data;
    set uniques;
    if (f.is_open()) {
        bool begin = true;
        std::string line;
        std::string id = "";
        std::string dna = "";
        while (f >> line) {
            std::string r = rcomp(line);
            pair p = std::make_pair(line, r);
            if (line.compare(r) < 0) {
                p = std::make_pair(r, line);
            }
            if (uniques.count(p) == 0) {
                uniques.insert(p);
                data.push_back(p);
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

std::list<pair> make_graph_adjacencies(std::list<pair> data) {
    std::list<pair> graph;
    for (pair kmer : data) {
        std::string first = kmer.substr(0, kmer.size() - 1);
        std::string second = kmer.substr(1);
        graph.push_back(std::make_pair(first, second));
    }
    return graph;
}

map make_graph(std::list<pair> adjs) {
    map graph;
    for (pair adj : adjs) {
        std::string idx = adj.first;
        if (graph.count(idx) == 0) {
            std::list<std::string> l;
            l.push_back(adj.second);
            graph.insert(std::make_pair(idx, l));
        }
        else {
            auto lookup = graph.find(idx);
            lookup->second.push_back(adj.second);
        }
    }
    return graph;
}

int main() {
    std::list<pair> data = get_data();
    std::list<pair> adjs = make_graph_adjacencies(data);
    map graph = make_graph(adjs);
    std::string start = graph.begin()->first;
    std::string output = "";
    std::string current = start;
    do {
        current = *graph.at(current).begin();
        output += current.back();
    } while (current != start);
    std::ofstream out("output.txt");
    out << output << std::endl;
    out.close();
    return 0;
}