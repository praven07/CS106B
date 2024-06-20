#include <iostream>
#include <stdlib.h>
#include "simpio.h"
#include "vector.h"
using namespace std;



Vector<int> generateNumbers(int start, int end);
bool find(Vector<int> &list, int value);


int main(){

	int start = 70000000;
	int end = 99999999;
	int numSteps = 1;
	cout << "loading...";
	Vector<int> list = generateNumbers(start,end);
	system("cls");
	while(true){
		int findValue = getInteger("Enter a value to find: ");
		if(findValue == 0)break;	
		if(find(list,findValue)){
			cout << "Found: " << findValue << endl;
		}else{
			cout << "Cannot find the give number" << endl;
		}
	}
	return 0;
}

Vector<int> generateNumbers(int start, int end){

	Vector<int> list;
	while(start != end){
		list.add(start);
		start++;
	}
	return list;
}



bool find(Vector<int> &list, int value){
	
	for(int i=0; i<list.size(); i++){
		if(list[i] == value)return true; 
	}
	return false;
}
