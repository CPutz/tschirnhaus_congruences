#include "polmodtree.hpp"
#include "mathextra.hpp"
#include <sstream>
#include <iostream>


PolModTreeNode::PolModTreeNode(int coeff, int modulus) { 
	this->coeff = mod(coeff, modulus);
	this->modulus = modulus;
}

PolModTreeNode::PolModTreeNode(std::vector<PolModTreeNode *> children,
	int modulus) {
	this->children = children;
	this->modulus = modulus;
}

PolModTreeNode::~PolModTreeNode() {
	for (PolModTreeNode *c : this->children) {
		delete c;
	}
}

int PolModTreeNode::get_coeff() {
	return this->coeff;
}

void PolModTreeNode::eval(int val) {
	std::vector<PolModTreeNode *>::iterator it = this->children.begin();
	this->coeff = (*it)->get_coeff();

	//use Horner scheme on children
	while (++it != this->children.end()) {
		PolModTreeNode *c = *it;
		this->coeff *= val;
		if (c != nullptr) {
			this->coeff += c->get_coeff();
		}
		this->coeff = mod(this->coeff, this->modulus);
	}
}


PolModTree::PolModTree(std::fstream &fs, int modulus) {
	//go to star of file
	fs.clear();
	fs.seekg(0, std::ios::beg);

	this->modulus = modulus;
	std::string line;
	std::getline(fs, line);
	std::istringstream ss(line);
	this->depth = 0;
	this->head = tree_read(0, ss);
}

PolModTreeNode* PolModTree::tree_read(int level, std::istringstream &ss) {
	if (this->depth < level) {
		this->depth = level;
	}

	PolModTreeNode *n;
	char c;
	ss.get(c);

	if (this->nodes.size() < level + 1) {
		this->nodes.push_back(std::vector<PolModTreeNode*>());
	}

	if (ss.peek() == '[') {
		std::vector<PolModTreeNode*> children;
		do {
			children.push_back(tree_read(level + 1, ss));
		} while (ss.peek() != ']');
		ss.get(c);
		n = new PolModTreeNode(children, this->modulus);
	} else {
		std::string v;
		std::getline(ss, v, ']');
		if (!v.empty()) {
			int c;
			std::istringstream(v) >> c;
			n = new PolModTreeNode(c, this->modulus);
		} else {
			n = nullptr;
		}
	}

	if (n != nullptr) {
		this->nodes[level].push_back(n);
	}

	return n;
}

PolModTree::~PolModTree() {
	delete head;
}

int PolModTree::get_val() {
	//TODO: lame
	if (!this->head) {
		return 0;
	}
	return this->head->get_coeff();
}

int PolModTree::get_depth() {
	return this->depth;
}

void PolModTree::eval(int level, int val) {  
	//evaluate all nodes at the current level
	for (PolModTreeNode *n : this->nodes[level]) {
		n->eval(val);
	}
}