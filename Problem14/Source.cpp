#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <utility>
#include <unordered_set>

using pair = std::pair<std::string, std::string>;
using set = std::unordered_set<std::string>;

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
	std::ifstream f("rosalind_corr.txt");
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

set get_corrects(std::list<pair> data) {
	set candidates;
	set corrects;
	for (pair p : data) {
		std::string s = p.second;
		std::string r = rcomp(s);
		if (candidates.count(s) > 0 && candidates.count(r) > 0) {
			corrects.insert(s);
			corrects.insert(r);
		}
		candidates.insert(s);
		candidates.insert(r);
	}
	return corrects;
}

int hamming(std::string s1, std::string s2) {
	int dist = 0;
	for (int i = 0; i < s1.length(); i++) {
		if (s1.at(i) != s2.at(i)) {
			dist++;
		}
	}
	return dist;
}

int main() {
	std::list<pair> data = get_data();
	set corrects = get_corrects(data);
	std::list<std::string> correct_s;
	for (auto s : corrects) {
		correct_s.push_back(s);
	}
	std::ofstream out("output.txt");
	for (pair p : data) {
		std::string s1 = p.second;
		if (corrects.count(s1) == 0) {
			for (auto s2 : correct_s) {
				if (hamming(s1, s2) == 1) {
					out << s1 << "->" << s2 << std::endl;
				}
			}
		}
	}
	out.close();
	return 0;
}