#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_set>
#include <unordered_map>

class node;

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator () (std::pair<T1, T2> const& pair) const
    {
        std::size_t h1 = std::hash<T1>()(pair.first);
        std::size_t h2 = std::hash<T2>()(pair.second);

        return h1 ^ h2;
    }
};

using pair = std::pair<std::string, std::string>;
using set = std::unordered_set<pair, pair_hash>;
using sset = std::unordered_set<std::string>;
using map = std::unordered_map<std::string, std::string>;

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
    std::ifstream f("rosalind_gasm.txt");
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

class node {
public:
    std::string left, left_r, right, right_r;
    node(std::string l, std::string lr, std::string r, std::string rr) : left(l), left_r(lr), right(r), right_r(rr) {}

    bool operator=(const node& y) {
        if (this->left == y.left && this->left_r == y.left_r && this->right == y.right && this->right_r == y.right_r) {
            return true;
        }
        return false;
    }

    bool operator!=(const node& y) {
        if (this->left == y.left && this->left_r == y.left_r && this->right == y.right && this->right_r == y.right_r) {
            return false;
        }
        return true;
    }

    std::string get_next(std::string current) {
        if (current == left) {
            return right;
        }
        else if (current == left_r) {
            return right_r;
        }
        else if (current == right) {
            return left;
        }
        else if (current == right_r) {
            return left_r;
        }
        return left_r;
    }
};

map make_graph_adjacencies(std::list<pair> data, int k, bool& success) {
    map graph;
    for (pair kmer : data) {
        std::string forward = kmer.first;
        std::string reverse = kmer.second;
        std::string f_last, r_last;
        for (int i = 0; i + k <= forward.length(); i++) {
            std::string f = forward.substr(i, k);
            std::string r = reverse.substr(i, k);
            if (i > 0) {
                //we expect there to be a graph containing exactly two cycles
                //this means we should not have overlapping edges
                //if there are overlapping edges, this means k is not correct for this problem
                if (graph.count(f_last) != 0) {
                    if (graph.at(f_last).compare(f) != 0) {
                        success = false;
                        return graph;
                    }
                }
                if (graph.count(r_last) != 0) {
                    if (graph.at(r_last).compare(r) != 0) {
                        success = false;
                        return graph;
                    }
                }
                graph.insert(std::make_pair(f_last, f));
                graph.insert(std::make_pair(r_last, r));
            }

            f_last = f;
            r_last = r;
        }
        //std::string l = forward.substr(0, forward.size() - 1);
        //std::string r = forward.substr(1);
        //std::string lr = reverse.substr(0, reverse.size() - 1);
        //std::string rr = reverse.substr(1);
        //node n(l, lr, r, rr);
        //graph.push_back(n);
    }
    success = true;
    return graph;
}

//map make_graph(std::list<node> adjs) {
//    map graph;
//    for (node adj : adjs) {
//        //std::string idx = adj.left;
//        graph.insert(std::make_pair(adj.left, adj));
//        graph.insert(std::make_pair(adj.left_r, adj));
//        graph.insert(std::make_pair(adj.right, adj));
//        graph.insert(std::make_pair(adj.right_r, adj));
//        /*if (graph.count(idx) == 0) {
//            graph.insert(std::make_pair(idx, adj));
//        }
//        else {
//            auto lookup = graph.find(idx);
//            lookup->second.push_back(adj.second);
//        }*/
//    }
//    return graph;
//}

int main() {
    std::list<pair> data = get_data();
    bool success = false;
    map graph;
    int k = 2;
    do {
        graph = make_graph_adjacencies(data, k++, success);
    } while (!success);
    std::string start = graph.begin()->first;
    std::string output = "";
    std::string current = start;
    do {
        current = graph.at(current);
        output += current.back();
    } while (current != start);
    std::ofstream out("output.txt");
    out << output << std::endl;
    out.close();
    return 0;
}