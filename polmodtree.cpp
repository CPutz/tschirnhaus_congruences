#include "polmodtree.hpp"
#include <iostream>
#include <sstream>


Node::Node(long coeff, long p) { 
	this->coeff = coeff;
	this->p = p;
}

Node::Node(std::vector<Node *> children, long p) {
	this->children = children;
	this->p = p;
}

Node::~Node() {
	for (Node *c : this->children) {
		delete c;
	}
}

long Node::get_coeff() {
	return this->coeff;
}

void Node::eval(long val) {
	std::vector<Node *>::iterator it = this->children.begin();
	this->coeff = (*it)->get_coeff();

	//use Horner scheme on children
	while (++it != this->children.end()) {
		Node *c = *it;
		this->coeff = this->coeff * val;
		if (c != nullptr) {
			this->coeff += c->get_coeff();
		}
		this->coeff = (this->coeff % this->p);
	}
}

void Node::print() {
	printf("%ld\n", this->coeff);
}


PolModTree::PolModTree(Node *head, std::vector<std::vector<Node *>> nodes, long p) {
	this->head = head;
	this->nodes = nodes;
	this->p = p;
}

PolModTree::PolModTree(std::fstream &fs, long p) {
	this->p = p;
	std::string line;
	std::getline(fs, line);
	std::istringstream ss(line);
	this->head = tree_read(0, ss);
}

Node* PolModTree::tree_read(int level, std::istringstream &ss) {
	Node *n;
	char c;
	ss.get(c);

	if (this->nodes.size() < level + 1) {
		this->nodes.push_back(std::vector<Node*>());
	}

	if (ss.peek() == '[') {
		std::vector<Node*> children;
		do {
			children.push_back(tree_read(level + 1, ss));
		} while (ss.peek() != ']');
		ss.get(c);
		n = new Node(children, this->p);
	} else {
		std::string v;
		std::getline(ss, v, ']');
		if (!v.empty()) {
			long c;
			std::istringstream(v) >> c;
			n = new Node(c, this->p);
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

long PolModTree::get_val() {
	return this->head->get_coeff();
}

void PolModTree::eval(int level, long val) {
	//evaluate all nodes at the current level
	for (Node *n : this->nodes[level]) {
		n->eval(val);
	}
}

void PolModTree::print() {
	this->head->print();
}