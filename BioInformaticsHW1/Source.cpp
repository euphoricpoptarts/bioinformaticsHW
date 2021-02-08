#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::ifstream f("rosalind_dna.txt");
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
	int counts[] = { 0, 0, 0, 0 };
	for (auto c = data.begin(); c != data.end(); ++c) {
		char x = *c;
		if (x == 'A') {
			counts[0]++;
		}
		else if (x == 'C') {
			counts[1]++;
		}
		else if (x == 'G') {
			counts[2]++;
		}
		else if (x == 'T') {
			counts[3]++;
		}
	}
	std::cout << counts[0] << " " << counts[1] << " " << counts[2] << " " << counts[3] << std::endl;
	return 0;
}