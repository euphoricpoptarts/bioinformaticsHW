#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::ifstream f("rosalind_revc.txt");
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
	std::cout << output << std::endl;
	return 0;
}