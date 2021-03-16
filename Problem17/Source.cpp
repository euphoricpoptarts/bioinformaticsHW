#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_set>
#include <unordered_map>

using set = std::unordered_set<std::string>;
using pair = std::pair<std::string, std::string>;
using map = std::unordered_map<std::string, std::list<std::string>>;
using list = std::list<std::string>;

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
    std::ifstream f("rosalind_idk.txt");
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

std::list<pair> make_graph_adjacencies(std::list<std::string> data) {
    std::list<pair> graph;
    for (std::string kmer : data) {
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

std::list<std::string> traverse(std::string start, map& graph);

std::list<std::string> split(std::string start, map& graph) {
    list& x = graph.at(start);
    x.sort();
    std::list<std::string> divergence;
    map g_copy = graph;
    list y = x;
    std::string last = "";
    for(auto xi = x.begin(); xi != x.end(); xi++){
        std::string next = *xi;
        if (next == last) continue;
        last = next;
        auto yi = y.begin();
        std::advance(yi, std::distance(x.begin(), xi));
        y.erase(yi);
        g_copy.at(start) = y;
        y = x;
        std::list<std::string> l = traverse(next, g_copy);
        g_copy = graph;
        for (auto s : l) {
            divergence.push_back(start.back() + s);
        }
    }
    return divergence;
}

std::list<std::string> traverse(std::string start, map& graph) {
    list& x = graph.at(start);
    if (x.size() == 1) {
        std::string next = x.front();
        x.pop_front();
        std::list<std::string> l = traverse(next, graph);
        for (auto& s : l) {
            s = start.back() + s;
        }
        return l;
    }
    else if (x.size() == 0) {
        std::list<std::string> l;
        std::string out = "";
        out += start.back();
        l.push_back(out);
        return l;
    }
    else {
        list l = split(start, graph);
        return l;
    }
}

int main() {
    std::list<std::string> data = get_data();
    std::string output = *data.begin();
    std::string start = output.substr(1);
    std::string to_remove = output.substr(0, output.size() - 1);
    output = output.substr(0, output.size() - 1);
    data.pop_front();
    std::list<pair> adjs = make_graph_adjacencies(data);
    map graph = make_graph(adjs);
    std::string current = start;
    bool breakout = false;
    for (auto& p : graph) {
        list& l = p.second;
        for (auto it = l.begin(); it != l.end(); it++) {
            if (*it == to_remove) {
                l.erase(it);
                breakout = true;
                break;
            }
        }
        if (breakout) break;
    }
    to_remove = "TC";
    breakout = false;
    for (auto& p : graph) {
        list& l = p.second;
        for (auto it = l.begin(); it != l.end(); it++) {
            if (*it == to_remove) {
                l.erase(it);
                breakout = true;
                break;
            }
        }
        if (breakout) break;
    }
    list l = traverse(current, graph);
    l.sort();
    std::ofstream out("output.txt");
    for (auto s : l) {
        std::cout << output << s << std::endl;
    }
    out.close();
    return 0;
}