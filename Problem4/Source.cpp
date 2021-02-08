#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <utility>

using pair = std::pair<std::string, std::string>;

double gc_content(const pair& p) {
	const std::string dna = p.second;
	unsigned int gc_count = 0;
	for (auto c : dna) {
		if (c == 'C' || c == 'G') gc_count++;
	}
	return 100.0 * static_cast<double>(gc_count) / static_cast<double>(dna.length());
}

int main() {
	std::ifstream f("rosalind_gc.txt");
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
		}
		else if (!begin) {
			//final data entry is not created by above loop
			data.push_back(std::make_pair(id, dna));
		}
		f.close();
	}
	else {
		std::cerr << "Could not find file!" << std::endl;
		return 1;
	}

	double max_gc = 0.0;
	std::string max_id;
	for (auto p : data) {
		double gc = gc_content(p);
		if (gc >= max_gc) {
			max_gc = gc;
			max_id = p.first;
		}
	}
	std::cout << max_id << std::endl << max_gc << std::endl;
	return 0;
}