#pragma once
#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
using namespace std;

template <class T>
class Tree {
	struct BNode;
	BNode* root;

public:
	Tree();
	Tree(Tree&); //copy constructor
	void duplicateTree(BNode*, BNode*&);
	Tree operator=(Tree);
	void insertR(T, BNode*);
	void insertR(T); //wrapper function
	void insertI(T);
	void visit(BNode*);
	bool searchR(T, BNode*);
	bool searchR(T);  //wrapper function
	bool searchI(T);
	void deleteR(T, BNode*&);
	void deleteNode(BNode*&);
	void deleteR(T); //wrapper function
	void deleteI(T);
	void inOrder(); //wrapper
	void inOrder(BNode*);
	void inOrder(int); //wrapper
	void inOrder(int, BNode*);
	void preOrder(); //wrapper
	void preOrder(BNode*);
	void postOrder(); //wrapper
	void postOrder(BNode*);
	T getPredecessor(BNode*);
	T getSuccessor(BNode*);
	void destroy(BNode*);
	~Tree();
	//functions necessary for specialized class
	void updateNode(T&);
	void searchAndPrint(T&);
	void treeToArray(vector<T>&);
	void treeToArray(BNode*, vector<T>&);
	void createBalancedTree(vector<T>&, BNode*&);
	void createBalancedTree(vector<T>&);
	//friend functions
};

//BNODE CLASS
template <class T>
struct Tree<T>::BNode {
	T data;
	BNode* right;
	BNode* left;

	BNode(T d) {
		data = d;
		right = left = nullptr;
	}
};

//BST CLASS
template<class T>
inline Tree<T>::Tree()
{
	root = nullptr;
}

template<class T>
inline Tree<T>::Tree(Tree& copy)
{
	duplicateTree(this->root, copy.root);
}

template<class T>
void Tree<T>::duplicateTree(BNode* curr, BNode*& copy)
{
	//as the nodes needs to be present before adding the children
	if (curr != nullptr) {
		copy = new BNode(curr->data);
		duplicateTree(curr->left, copy->left);
		duplicateTree(curr->right, copy->right);
	}
}

template<class T>
inline Tree<T> Tree<T>::operator=(Tree copy)
{
	if (this != &copy) {
		duplicateTree(root, copy.root);
	}
	return *this;
}

template<class T>
inline void Tree<T>::insertR(T d, BNode* node)
{
	if (node == nullptr) {
		node = new BNode(d);
	}
	else {
		if (d >= node->data) {
			insertR(d, node->right);
		}
		else {
			insertR(d, node->left);
		}
	}
}

template<class T>
inline void Tree<T>::insertR(T d)
{
	insertR(d, root);
}

template<class T>
inline void Tree<T>::insertI(T d)
{
	//previous pointer required so the original value updated rather than
	//the value of a pointer
	BNode* curr = root, * prev = nullptr;
	while (curr != nullptr) {
		prev = curr;
		if (d >= curr->data) {
			curr = curr->right;
		}
		else {
			curr = curr->left;
		}
	}
	if (prev == nullptr) {
		root = new BNode(d);
	}
	else if (d >= prev->data) {
		prev->right = new BNode(d);
	}
	else {
		prev->left = new BNode(d);
	}
}

template<class T>
inline bool Tree<T>::searchR(T d, BNode* node)
{
	if (node != nullptr) {
		if (d == node->data) {
			return true;
		}
		else if (d > node->data) {
			return searchR(d, node->right);
		}
		else {
			return searchR(d, node->left);
		}
	}
	else {
		return false;
	}
}

template<class T>
inline bool Tree<T>::searchR(T d)
{
	return searchR(d, root);
}

template<class T>
inline bool Tree<T>::searchI(T d)
{
	BNode* node = root;
	while (node != nullptr) {
		if (d == node->data) {
			return true;
		}
		else if (d > node->data) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}
	return false;
}

template<class T>
inline void Tree<T>::deleteR(T d, BNode*& node)
{
	if (node != nullptr) {
		if (d > node->data) {
			deleteR(d, node->right);
		}
		else if (d < node->data) {
			deleteR(d, node->left);
		}
		else {
			deleteNode(node);
		}
	}
}

template<class T>
inline void Tree<T>::deleteNode(BNode*& node)
{
	//as we are using a BST with duplication so successor approach need to be utilized
	BNode* tmp = node;
	//the first two conditions cover the first two cases of deletion
	if (node->left == nullptr) {
		node = node->right;
		delete tmp;
	}
	else if (node->right == nullptr) {
		node = node->left;
		delete tmp;
	}
	//third case of deletion
	else {
		node->data = getSuccessor(node->right); //successor approach for BST with duplication
		deleteR(node->data, node->right); //deelte the predeseccor/succesor*(in this case)
	}
}

template<class T>
inline void Tree<T>::deleteR(T d)
{
	deleteR(d, root);
}

template<class T>
inline void Tree<T>::deleteI(T d)
{
	BNode* parent = nullptr, *child = root;
	while (d != child->data && child != nullptr) {
		parent = child;
		if (d > child->data) {
			child = child->right;
		}
		else if (d < child->data) {
			child = child->left;
		}
	}
	if (child != nullptr) {
		if (child == root) {
			deleteNode(root);
		}
		else if (parent->left == child) {
			deleteNode(parent->left); //like in iterative insert we send the parent->left/right as arguement so original values updated rather than the temoorary child pointer
		}
		else if (parent->right == child) {
			deleteNode(parent->right);
		}
	}
}

template<class T>
inline void Tree<T>::inOrder()
{
	inOrder(root);
}

template<class T>
inline void Tree<T>::inOrder(BNode* node)
{
	//produces the output in ascending order like a sorted array
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->data << " , ";
		inOrder(node->right);
	}
}

template<class T>
inline void Tree<T>::inOrder(int a)
{
	inOrder(0, root);
}

template<class T>
inline void Tree<T>::inOrder(int a, BNode* node)
{
	//int is just a dummy parameter
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->data;
		inOrder(node->right);
	}
}

template<class T>
inline void Tree<T>::preOrder()
{
	preOrder(root);
}

template<class T>
inline void Tree<T>::preOrder(BNode* node)
{
	if (node != nullptr) {
		cout << node->data << " , ";
		preOrder(node->left);
		preOrder(node->right);
	}
}

template<class T>
inline void Tree<T>::postOrder()
{
	postOrder(root);
}

template<class T>
inline void Tree<T>::postOrder(BNode* node)
{
	if (node != nullptr) {
		postOrder(node->left);
		postOrder(node->right);
		cout << node->data << " , ";
	}
}

template<class T>
inline T Tree<T>::getPredecessor(BNode* tmp)
{
	while (tmp->right != nullptr) {
		tmp = tmp->right;
	}
	return tmp->data;
}

template<class T>
inline T Tree<T>::getSuccessor(BNode* tmp)
{
	while (tmp->left != nullptr) {
		tmp = tmp->left;
	}
	return tmp->data;
}

template<class T>
inline void Tree<T>::destroy(BNode* node)
{
	//uses post-order traversal as left and right tree deleted before the node itself
	if (node != nullptr) {
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}

template<class T>
inline Tree<T>::~Tree()
{
	destroy(root);
}

template<class T>
inline void Tree<T>::updateNode(T&)
{
}

template<class T>
inline void Tree<T>::searchAndPrint(T&)
{
}

template<class T>
inline void Tree<T>::visit(BNode* node)
{
}

template<class T>
inline void Tree<T>::treeToArray(vector<T>& v1)
{
	return treeToArray(root, v1);
}

template<class T>
inline void Tree<T>::treeToArray(BNode* node, vector<T>& v1)
{
	if (node != nullptr) {
	    treeToArray(node->left, v1);
		v1.push_back(node->data);
		treeToArray(node->right, v1);
	}
}

template<class T>
inline void Tree<T>::createBalancedTree(vector<T>&, BNode*&)
{
}

template<class T>
inline void Tree<T>::createBalancedTree(vector<T>&)
{
}
