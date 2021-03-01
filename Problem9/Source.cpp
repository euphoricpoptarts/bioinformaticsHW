#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <list>

std::string get_input() {
	std::ifstream f("rosalind_suff.txt");
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
		//first delete children recursively
		for (auto pair : children) {
			delete pair.second;
		}
		children.clear();
	}
};

void traverse(tree_node* x, std::ofstream& out_f) {
	for (auto pair : x->children) {
		out_f << pair.first << std::endl;
		traverse(pair.second, out_f);
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
			//if we don't clear children then z will be deleted in y's destructor
			y->children.clear();
			delete y;
		}
		else {
			//keep y
			children.insert(pair);
		}
	}
	x->children = children;
}

tree_node* construct_tree(std::string input) {
	tree_node* root = new tree_node();
	tree_node* deepest_suffix = root;
	for (int i = 0; i < input.length(); i++) {
		std::string y;
		y.push_back(input.at(i));
		deepest_suffix = insert(deepest_suffix, y);
	}
	collapse_tree(root);
	return root;
}

int main() {
	std::string input = get_input();
	if (input == "") {
		return 1;
	}
	tree_node* x = construct_tree(input);
	std::ofstream out_file("out.txt");
	if (out_file.is_open()) {
		traverse(x, out_file);
		out_file.close();
	}
	delete x;
}