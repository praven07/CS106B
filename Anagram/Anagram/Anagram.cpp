/*
 * File:Anagram.cpp
 * -----------------
 * This application gets a lexicon file and make all the words in anagram cluster
 * and then prints the largest cluster
 */
#include <iostream>
#include <istream>
#include "simpio.h"
#include "map.h"
#include "set.h"
using namespace std;


/* Prototype */
void openFile(ifstream &file);
void readFile(ifstream &file, Map<string,Set<string> > &list);
void printList(Map<string,Set<string> > &list);
string createKey(string word);
void swap(char &ch1, char &ch2);


/* Main */
int main()
{
	ifstream infile;
	Map<string,Set<string> > list;
	openFile(infile);
	readFile(infile,list);
	printList(list);
	return 0;
}


/*
 * This function takes a file input stream object by reference
 * and get a user input for filename and the tries to open the given
 * file and if file does not exists it puts an error message
 */
void openFile(ifstream &file)
{
	while(true){
		string filename = getLine("Enter a filename: ");
		file.open(filename.c_str());
		if(!file.fail()){
			break;
		}else{
			error("File does not Exists");
			file.clear();
		}
	}
}


/*
 * This function reads in each line from the file and make a anagram key of
 * that word and stores it in the list
 */
void readFile(ifstream &file, Map<string,Set<string> > &list)
{
	while(true){
		string word;
		getline(file,word);
		if(word == "")break;
		string key = createKey(word);
		list[key].add(word);
	}
}


/*
 * It creates a sorted string in alpha order and return a string
 */
string createKey(string word)
{
	for(int i=0; i<word.size(); i++){
		int minIndex = i;
		for(int j=i+1; j<word.size(); j++){
			if(word[j] < word[minIndex]){
				minIndex = j;
			}
		}
		swap(word[i],word[minIndex]);
	}
	return word;
}


/*
 * It swaps the given two char by reference
 */
void swap(char &ch1, char &ch2)
{
	char temp = ch1;
	ch1 = ch2;
	ch2 = temp;
}


/*
 * It prints out the maxSize and the words of the largest
 * anagram cluster list
 */
void printList(Map<string,Set<string> > &list)
{
	int maxSize = 0;
	string word = "";
	Set<string> maxList;
	foreach(string key in list){
		if(list.get(key).size() > maxSize){
			maxSize = list.get(key).size();
			maxList = list.get(key);
			word = key;
		}
	}

	cout << "Largest Anagram Cluster Key: " << word << endl;
	cout << "Cluster size: " << maxSize << endl;
	foreach(string word in maxList){
		cout << word << endl;
	}
}
