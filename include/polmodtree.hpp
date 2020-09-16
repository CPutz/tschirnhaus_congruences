#ifndef POLMODTREE_HPP
#define POLMODTREE_HPP

#include <vector>
#include <fstream>


class PolModTreeNode {
private:
	int coeff;
	int modulus;
	std::vector<PolModTreeNode *> children;

public:
	PolModTreeNode(int coeff, int modulus);
	PolModTreeNode(std::vector<PolModTreeNode *> children, int modulus);
	~PolModTreeNode();
	int get_coeff();
	void eval(int val);
};


class PolModTree {
private:
	int modulus;
	int depth;
	PolModTreeNode *head;
	std::vector<std::vector<PolModTreeNode *>> nodes;
	PolModTreeNode* tree_read(int level, std::istringstream &ss);

public:
	PolModTree(std::fstream &fs, int modulus);
	~PolModTree();
	int get_val();
	int get_depth();
	void eval(int level, int val);
};

#endif // POLMODTREE_HPP
