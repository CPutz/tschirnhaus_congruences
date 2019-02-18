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
	void eval(long val);
	void print();
};


class PolModTree {
private:
	Node *head;
	std::vector<std::vector<Node *>> nodes;
	Node* tree_read(int level, std::istringstream &ss);
	long p;

public:
	PolModTree(Node *head, std::vector<std::vector<Node *>> nodes, long p);
	PolModTree(std::fstream &fs, long p);
	~PolModTree();
	long get_val();
	void eval(int level, long val);
	void print();
};


#endif // POLMODTREE_HPP
