#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

int main() {
	std::ifstream f("rosalind_rna.txt");
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
	std::replace(data.begin(), data.end(), 'T', 'U');
	std::cout << data << std::endl;
	return 0;
}