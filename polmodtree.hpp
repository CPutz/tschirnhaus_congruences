#ifndef POLMODTREE_HPP
#define POLMODTREE_HPP

#include <vector>
#include <fstream>


class Node {
private:
	long coeff;
	long p;
	std::vector<Node *> children;

public:
	Node(long coeff, long p);
	Node(std::vector<Node *> children, long p);
	~Node();
	long get_coeff();
	void add_coeff(long val);
	void eval(long val);
	void print();
};


class PolTree {
private:
	Node *head;
	std::vector<std::vector<Node *>> nodes;
	Node* tree_read(int level, std::istringstream &ss);
	long p;

public:
	PolTree(Node *head, std::vector<std::vector<Node *>> nodes, long p);
	PolTree(std::fstream &fs, long p);
	~PolTree();
	long get_val();
	void eval(int level, long val);
	void print();
};


#endif // POLMODTREE_HPP
