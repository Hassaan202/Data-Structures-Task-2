#pragma once
#include "BST.h"

//LIST CLASS
template <class T>
void printList(list<T>& l1){
	typename list<T>::iterator it = l1.begin();
	while (it != l1.end()) {
		cout << *it << endl;
		it++;
	}
	cout << endl;
}

template <class T>
ostream& operator<<(ostream& out, list<T> l1) {
	printList(l1);
	return out;
}

//OCCURENCE DATA STRUCTURE
struct OccurenceData {
	int chapterNum;
	int pageNum;
	int lineNum;

	friend ostream& operator<<(ostream&, const OccurenceData&);
};

ostream& operator<<(ostream& out, const OccurenceData& od)
{
    out << "Ch " << od.chapterNum << "; Pg " << od.pageNum << "; Ln " << od.lineNum; //no endl as list print already has it
	return out;
}

//CLASS WORD DATA
class WordData {
	friend class Tree<WordData>;
	string key;
	list<OccurenceData> wordOccurences;
public:
	WordData(string s = "") {
		key = s;
	}
	void updateKey(string s) {
		key = s;
	}
	bool operator<(WordData& w) {
		return (key < w.key);
	}
	bool operator>(WordData& w) {
		return (key > w.key);
	}
	bool operator==(WordData& w) {
		return (key == w.key);
	}
	void updateList(WordData& w) {
		while (!w.wordOccurences.empty()) {
			wordOccurences.push_back(w.wordOccurences.back());
			w.wordOccurences.pop_back();
		}
	}
	friend ostream& operator<<(ostream&, const WordData&);
	friend void populateSpecializedTree(Tree<WordData>&, string, Tree<string>&);
};

ostream& operator<<(ostream& out, const WordData& w)
{
	out << "Word: " << w.key << endl;
	out << "Occurences:\n" << w.wordOccurences;
	return out;
}


//SPECIALIZED BST CLASS
template <>
inline Tree<WordData>::Tree()
{
	root = nullptr;
}

template <>
inline Tree<WordData>::Tree(Tree& copy)
{
	duplicateTree(this->root, copy.root);
}

template <>
inline Tree<WordData> Tree<WordData>::operator=(Tree copy)
{
	if (this != &copy) {
		duplicateTree(root, copy.root);
	}
	return *this;
}

template <>
inline void Tree<WordData>::insertR(WordData d, BNode* node)
{
	if (node == nullptr) {
		node = new BNode(d);
	}
	else {
		if (d.key >= node->data.key) {
			insertR(d, node->right);
		}
		else {
			insertR(d, node->left);
		}
	}
}

template <>
inline void Tree<WordData>::insertR(WordData d)
{
	insertR(d, root);
}

template <>
inline void Tree<WordData>::insertI(WordData d)
{
	//previous pointer required so the original value updated rather than
	//the value of a pointer
	BNode* curr = root, * prev = nullptr;
	while (curr != nullptr) {
		prev = curr;
		if (d.key >= curr->data.key) {
			curr = curr->right;
		}
		else {
			curr = curr->left;
		}
	}
	if (prev == nullptr) {
		root = new BNode(d);
	}
	else if (d.key >= prev->data.key) {
		prev->right = new BNode(d);
	}
	else {
		prev->left = new BNode(d);
	}
}

template <>
void Tree<WordData>::updateNode(WordData& w1) {
	BNode* node = root;
	while (node != nullptr) {
		if (w1.key == node->data.key) {
			while (!w1.wordOccurences.empty()){
				node->data.wordOccurences.push_back(w1.wordOccurences.back()); //adds the last element of the list
			w1.wordOccurences.pop_back();
		    }
			return;
	    }
		else if (w1.key > node->data.key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}
}

template<>
inline void Tree<WordData>::searchAndPrint(WordData& w1)
{
	BNode* node = root;
	while (node != nullptr) {
		if (w1.key == node->data.key) {
			cout << node->data;
			return;
		}
		else if (w1.key > node->data.key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}
	if (node == nullptr) {
		cout << "Unable to find the word!\n";
	}
}

template<>
inline void Tree<WordData>::createBalancedTree(vector<WordData>& v, BNode*& node)
{
	if (v.size() == 0) {   //base case
		return;
	}
	else if (v.size() == 1) {   //base case
		node = new BNode(v[0]);
		return;
	}
	else {  //general case
		int mid = (v.size() - 1) / 2;
		node = new BNode(v[mid]);
		vector<WordData> leftVector, rightVector;
		//stores the left half of the array
		for (int i = 0; i < mid; i++) {
			leftVector.push_back(v[i]);
		}
		//stores the right half of the array
		for (int i = mid + 1; i < v.size(); i++) {
			rightVector.push_back(v[i]);
		}
		createBalancedTree(leftVector, node->left);
		createBalancedTree(rightVector, node->right);
	}
}

template<>
inline void Tree<WordData>::createBalancedTree(vector<WordData>& v1)
{
	createBalancedTree(v1, root);
}


template <>
inline void Tree<WordData>::visit(BNode* node)
{
}

template <>
inline bool Tree<WordData>::searchR(WordData d, BNode* node)
{
	if (node != nullptr) {
		if (d.key == node->data.key) {
			return true;
		}
		else if (d.key > node->data.key) {
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

template <>
inline bool Tree<WordData>::searchR(WordData d)
{
	return searchR(d, root);
}

template <>
inline bool Tree<WordData>::searchI(WordData d)
{
	BNode* node = root;
	while (node != nullptr) {
		if (d.key == node->data.key) {
			return true;
		}
		else if (d.key > node->data.key) {
			node = node->right;
		}
		else {
			node = node->left;
		}
	}
	return false;
}

template <>
inline void Tree<WordData>::deleteR(WordData d, BNode*& node)
{
	if (node != nullptr) {
		if (d.key > node->data.key) {
			deleteR(d, node->right);
		}
		else if (d.key < node->data.key) {
			deleteR(d, node->left);
		}
		else {
			deleteNode(node);
		}
	}
}

template <>
inline void Tree<WordData>::deleteR(WordData d)
{
	deleteR(d, root);
}

template <>
inline void Tree<WordData>::deleteI(WordData d)
{
	BNode* parent = nullptr, *child = root;
	while (d.key != child->data.key && child != nullptr) {
		parent = child;
		if (d.key > child->data.key) {
			child = child->right;
		}
		else if (d.key < child->data.key) {
			child = child->right;
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

template <>
inline WordData Tree<WordData>::getPredecessor(BNode* tmp)
{
	while (tmp->right != nullptr) {
		tmp = tmp->right;
	}
	return tmp->data;
}

template <>
inline WordData Tree<WordData>::getSuccessor(BNode* tmp)
{
	while (tmp->left != nullptr) {
		tmp = tmp->left;
	}
	return tmp->data;
}