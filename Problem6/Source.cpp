#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

std::unordered_map<std::string, char> codon_table = {
{"UUU", 'F'},      {"CUU", 'L'},      {"AUU", 'I'},      {"GUU", 'V'},
{"UUC", 'F'},      {"CUC", 'L'},      {"AUC", 'I'},      {"GUC", 'V'},
{"UUA", 'L'},      {"CUA", 'L'},      {"AUA", 'I'},      {"GUA", 'V'},
{"UUG", 'L'},      {"CUG", 'L'},      {"AUG", 'M'},      {"GUG", 'V'},
{"UCU", 'S'},      {"CCU", 'P'},      {"ACU", 'T'},      {"GCU", 'A'},
{"UCC", 'S'},      {"CCC", 'P'},      {"ACC", 'T'},      {"GCC", 'A'},
{"UCA", 'S'},      {"CCA", 'P'},      {"ACA", 'T'},      {"GCA", 'A'},
{"UCG", 'S'},      {"CCG", 'P'},      {"ACG", 'T'},      {"GCG", 'A'},
{"UAU", 'Y'},      {"CAU", 'H'},      {"AAU", 'N'},      {"GAU", 'D'},
{"UAC", 'Y'},      {"CAC", 'H'},      {"AAC", 'N'},      {"GAC", 'D'},
{"UAA", 'Z'},      {"CAA", 'Q'},      {"AAA", 'K'},      {"GAA", 'E'},
{"UAG", 'Z'},      {"CAG", 'Q'},      {"AAG", 'K'},      {"GAG", 'E'},
{"UGU", 'C'},      {"CGU", 'R'},      {"AGU", 'S'},      {"GGU", 'G'},
{"UGC", 'C'},      {"CGC", 'R'},      {"AGC", 'S'},      {"GGC", 'G'},
{"UGA", 'Z'},      {"CGA", 'R'},      {"AGA", 'R'},      {"GGA", 'G'},
{"UGG", 'W'},      {"CGG", 'R'},      {"AGG", 'R'},      {"GGG", 'G'}
};

int main() {
	std::ifstream f("rosalind_prot.txt");
	std::string data;
	if (f.is_open()) {
		//only one line of data expected
		f >> data;
		f.close();
	}
	else {
		std::cerr << "Could not find file!" << std::endl;
		return 1;
	}
	std::string output;
	for (auto c = data.begin(); c != data.end(); c += 3) {
		std::string codon = std::string(c, c + 3);
		char x = codon_table.at(codon);
		if (x != 'Z') {
			output += x;
		}
	}
	std::cout << output << std::endl;
	return 0;
}