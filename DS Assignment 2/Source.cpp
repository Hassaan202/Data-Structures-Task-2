#include "SpecializedClass.h"

void populateStopWordsTree(Tree<string>& t1, string fileName) {
	string currWord;
	ifstream fin(fileName);
	if (!fin) {
		cout << "File 1 has not succesfully opened!\n";
	}
	else {
		while (!fin.eof()) {
			fin >> currWord;
			t1.insertI(currWord);
		}
		fin.close();
	}
}

vector<string> tokenize(const string str, const string delimeters)
{
	vector<string> tokenized;
	bool delimeterFound = false;
	int wordLength = 0;
	for (int i = 0; i < str.length(); i++) {
		wordLength++;
		int j = 0;
		while (j < delimeters.length()) {
			if (str[i] == delimeters[j]) {
				delimeterFound = true;
			}
			j++;
		}
		if (delimeterFound) {
			tokenized.push_back(str.substr(i + 1 - wordLength, wordLength - 1));
			delimeterFound = false;
			//check if the next is also a delimeter(assumption is that at max two delimeters appear simultaneously)
			int j = 0;
			while (j < delimeters.length()) {
				if (str[i+1] == delimeters[j]) {
					delimeterFound = true;
				}
				j++;
			}
			if (delimeterFound) {
				i++;
			}
			delimeterFound = false;
			wordLength = 0;
		}
	}
	return tokenized;
}

void populateSpecializedTree(Tree<WordData>& t1, string fileName1, Tree<string>& stopWords) {
	string line;
	ifstream fin;
	fin.open(fileName1, ios::in);
	if (!fin) {
		cout << "File (fin) has not succesfully opened!\n";
	}
	else {
		int chapterNo = 0, pageNo = 0, lineNo = 0;
		while (getline(fin, line)) {
			lineNo++; //as each new line read
			if (line.substr(0, 8) == "#Chapter") {
				chapterNo = (line[9] - '0'); //updates the chapter no
			}
			else if (line.substr(0, 5) == "#Page") {
				pageNo = (line[6] - '0'); //updates the page no
			}
			else {
				//tokeization of the line
				vector<string> wordList = tokenize(line, ",. \n();#");
				for (int i = 0; i < wordList.size(); i++) {
					transform(wordList[i].begin(), wordList[i].end(), wordList[i].begin(), ::tolower); //converts each word to lower case for standardization when storing (part of regex library)
					if (!stopWords.searchI(wordList[i])) {
						//if not found in the stopword tree then insert in the specialized tree
						WordData tmp;
						tmp.key = wordList[i];
						OccurenceData currentOccurence;
						currentOccurence.chapterNum = chapterNo;
						currentOccurence.pageNum = pageNo;
						currentOccurence.lineNum = lineNo;
						tmp.wordOccurences.push_back(currentOccurence);
						if (t1.searchI(tmp)) {
							//if already present then update the occurence linked list only
							t1.updateNode(tmp);
						}
						else {
							//if word not already present then add it
							t1.insertI(tmp);
						}
					 }

				}
			}
		}
	}
	fin.close();
}

void updateOccurences(WordData& w1, vector<WordData>& dest) {
	for (int i = 0; i < dest.size(); i++) {
		if (w1 == dest[i]) {
			dest[i].updateList(w1);
			return;
		}
	}
}

void mergeSortVectors(vector<WordData>& v1, vector<WordData>& v2, vector<WordData>& m) {
	int i = 0, j = 0, k = 0;
	//add the frist manually
	if (v1[i] < v2[j]) {
		m.push_back(v1[i++]);
	}
	else {
		m.push_back(v2[j++]);
	}
	k++;
	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j]) {
			if (m[k - 1] == v1[i]) {
				updateOccurences(v1[i++], m);
			}
			else {
				m.push_back(v1[i++]);
				k++;
			}			
		}
		else {
			if (m[k - 1] == v2[j]) {
				updateOccurences(v2[j++], m);
			}
			else {
				m.push_back(v2[j++]);
				k++;
			}
		}
	}
	while (i < v1.size()) {
		m.push_back(v1[i++]);
		k++;
	}
	while (j < v2.size()) {
		m.push_back(v2[j++]);
		k++;
	}
}


void mergeSortVectorsForCommon(vector<WordData>& v1, vector<WordData>& v2, vector<WordData>& m) {
	int i = 0, j = 0, k = 0;
	while (i < v1.size() && j < v2.size()) {
		if (v1[i] < v2[j]) {
			i++;
		}
		else if (v1[i] > v2[j]) {
			j++;
		}
		else {
			//if the same elements then add to the array
			m.push_back(v1[i++]);
			updateOccurences(v2[j++], m);
			k++;
		}
	}
}

void mergeSpecializedTrees(Tree<WordData>& t1, Tree<WordData>& t2, Tree<WordData>& merged) {
	vector<WordData> v1, v2, mergeVector;
	t1.treeToArray(v1);
	/*for (int i = 0; i < v1.size(); i++) {
		cout << v1[i] << endl;
	}*/
	t2.treeToArray(v2);
	//for (int i = 0; i < v2.size(); i++) {
	//	cout << v2[i] << endl;
	//}
	
	//merge sort the vectors in O(n1+n2) time and also joins the occurences of the common words
	mergeSortVectors(v1, v2, mergeVector);
	//for (int i = 0; i < mergeVector.size(); i++) {
	//cout << mergeVector[i] << endl;
 //   } 
	//Creating a balanced specialized tree
	merged.createBalancedTree(mergeVector);
}

void findCommonWords(Tree<WordData>& t1, Tree<WordData>& t2, Tree<WordData>& common) {
	vector<WordData> v1, v2, commonVector;
	t1.treeToArray(v1);
	//for (int i = 0; i < v1.size(); i++) {
	//	cout << v1[i] << endl;
	//}
	t2.treeToArray(v2);
	//for (int i = 0; i < v2.size(); i++) {
	//	cout << v2[i] << endl;
	//}
	mergeSortVectorsForCommon(v1, v2, commonVector);
	common.createBalancedTree(commonVector);
}

int main(){
	//Populating stop words tree
	Tree<string> stopWords;
	populateStopWordsTree(stopWords, "stopWords.txt");
	/*stopWords.inOrder();*/

	//populating input words tree
	Tree<WordData> words1;
	populateSpecializedTree(words1, "input2.txt", stopWords);
	/*words1.inOrder(100);*/

	//searching a word and printing its occurences
	string searchWord;
	cout << "Enter the word to find the details: ";
	cin >> searchWord;
	WordData searchWordData(searchWord);
	words1.searchAndPrint(searchWordData);

	//Deleting a word from the tree
	string deleteWord;
	cout << "Enter the word to delete: ";
	cin >> deleteWord;
	WordData deleteWordData(deleteWord);
	words1.deleteR(deleteWordData);

	//to verify if the words has been deleted or not
	cout << "Enter the word to find the details: ";
	cin >> searchWord;
	searchWordData.updateKey(searchWord);
	words1.searchAndPrint(searchWordData);

	system("pause");
	//merge two specialized BST trees
	cout << "\n\nMERGED WORDS:\n";
	Tree<WordData> words2, mergeTree;
	populateSpecializedTree(words2, "input3.txt", stopWords);
	/*words2.inOrder(100);*/
	mergeSpecializedTrees(words1, words2, mergeTree);
	mergeTree.inOrder();

	system("pause");
	//finding common words
	cout << "\n\nCOMMON WORDS:\n";
	Tree<WordData> commonWordsTree;
	findCommonWords(words1, words2, commonWordsTree);
	commonWordsTree.inOrder();

	return 0;
}