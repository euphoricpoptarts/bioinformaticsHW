#include <iostream>
#include <fstream>
#include <string>

int main() {
	std::ifstream f("rosalind_hamm.txt");
	std::string s1, s2;
	if (f.is_open()) {
		//only two lines of data expected
		f >> s1 >> s2;
		f.close();
	}
	else {
		std::cerr << "Could not find file!" << std::endl;
		return 1;
	}
	if (s1.length() != s2.length()) {
		std::cerr << "The two strings have different lengths!" << std::endl;
	}
	unsigned int diff = 0;
	for (unsigned int i = 0; i < s1.length(); ++i) {
		if (s1.at(i) != s2.at(i)) {
			++diff;
		}
	}
	std::cout << diff << std::endl;
	return 0;
}