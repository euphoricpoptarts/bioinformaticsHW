#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <utility>

using pair = std::pair<std::string, std::string>;

bool is_complement(char a, char b) {
	if (a == 'C' && b == 'G') {
		return true;
	}
	else if (a == 'G' && b == 'C') {
		return true;
	}
	else if (a == 'A' && b == 'T') {
		return true;
	}
	else if (a == 'T' && b == 'A') {
		return true;
	}
	return false;
}

std::list<pair> get_data() {
	std::ifstream f("rosalind_revp.txt");
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

int main() {
	std::list<pair> data = get_data();
	if (data.size() != 1) {
		std::cerr << "Expecting exactly one data entry! Found " << data.size() << "!" << std::endl;
		return 1;
	}
	std::string dna = data.begin()->second;
	std::ofstream out("palindromes.txt");
	if (!out.is_open()) {
		std::cerr << "Couldn't open output file!" << std::endl;
		return 1;
	}
	for (int i = 0; i < dna.length() - 1; i++) {
		//a palindrome of length n can't be a palindrome unless it also has inner palindromes
		//so we start with the inner palindromes and expand outwards
		//also only even-length complement-palindromes exist
		for (int j = 0; j < 6; j++) {
			if ((i - j < 0) || (i + j + 1 >= dna.length())) {
				//break loop
				j = 7;
			}
			else if (is_complement(dna.at(i - j), dna.at(i + j + 1))) {
				int pal_size = 2 * (j + 1);
				if (pal_size >= 4) {
					// of course it's one-indexed, why not
					out << (i - j + 1) << " " << pal_size << std::endl;
				}
			}
			else {
				//break loop
				j = 7;
			}
		}
	}
	out.close();
	return 0;
}