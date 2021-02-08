#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>

std::string get_input() {
	std::ifstream f("rosalind_prot.txt");
	std::string data;
	if (f.is_open()) {
		//only one line of data expected
		f >> data;
		f.close();
	}
	else {
		std::cerr << "Could not find file!" << std::endl;
		return "";
	}
	return data;
}

class tree_node {
public:
	//very important to use pointers to avoid expensive deep copies
	std::unordered_map<std::string, tree_node*> children;
	tree_node* suffix_link = nullptr;

	~tree_node() {
		for (auto pair : children) {
			delete pair.second;
		}
		children.clear();
	}
};

char traverse(tree_node* x, std::string output, std::string input, int trie_depth) {
	//0 is a placeholder character before any characters have been processed
	char preceding = 0;
	bool all_match = true;
	for (auto pair : x->children) {
		std::string name = pair.first;
		//make sure to add length of name, because depth in tree is different from depth in trie
		char child_preceding = traverse(pair.second, output + name, input, trie_depth + name.length());
		//if any children are left diverse, or if all children don't have the same left preceding char
		//then we know that this node is also left diverse
		if (preceding && preceding != child_preceding && child_preceding != 1) {
			all_match = false;
		}
		else if (!preceding) {
			preceding = child_preceding;
		}
	}
	if (x->children.size() == 0) {
		if (trie_depth == input.length()) {
			//2 is a dummy character to indicate the character before start of string
			return 2;
		}
		//the depth of the suffix tells us where to find the character preceding it
		return input.at(input.length() - trie_depth - 1);
	}
	else {
		//output if this node is left diverse
		if (!all_match && output.length() >= 20) {
			std::cout << output << std::endl;
		}
		if (all_match) {
			//if not left diverse, return left preceding character
			return preceding;
		}
		else {
			//else return a dummy character to indicate that this is left diverse
			return 1;
		}
	}
}

tree_node* insert(tree_node* x, std::string next) {
	if (x->children.count(next) == 0) {
		tree_node* node = new tree_node();
		if (x->suffix_link != nullptr) {
			node->suffix_link = insert(x->suffix_link, next);
		}
		else {
			node->suffix_link = x;
		}
		x->children.insert({ next, node });
		return node;
	}
	else {
		return x->children.find(next)->second;
	}
}

//collapses all child paths recursively
void collapse_tree(tree_node* x) {
	std::unordered_map<std::string, tree_node*> children;
	for (auto pair : x->children) {
		std::string name = pair.first;
		collapse_tree(pair.second);
		tree_node* y = pair.second;
		if (y->children.size() == 1) {
			//replace y with its only child z
			//also concatenate the keys!
			auto z = y->children.begin();
			children.insert({ name + z->first, z->second });
		}
		else {
			//keep y
			children.insert(pair);
		}
	}
	x->children = children;
}

tree_node* construct_tree(std::string input) {
	tree_node * root = new tree_node();
	tree_node * deepest_suffix = root;
	for (int i = 0; i < input.length(); i++) {
		std::string y;
		y.push_back(input.at(i));
		deepest_suffix = insert(deepest_suffix, y);
	}
	collapse_tree(root);
	return root;
}

int main() {
	std::string input = "TAGAGATAGAATGGGTCCAGAGTTTTGTAATTTCCATGGGTCCAGAGTTTTGTAATTTATTATATAGAGATAGAATGGGTCCAGAGTTTTGTAATTTCCATGGGTCCAGAGTTTTGTAATTTAT$";
	if (input == "") {
		return 1;
	}
	tree_node* x = construct_tree(input);
	traverse(x, "", input, 0);
}