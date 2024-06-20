#include <iostream>
#include <fstream>
#include "console.h"
#include "lexicon.h"
#include "queue.h"
#include "stack.h"
using namespace std;


string wordLadder(string start, string end, Lexicon &dict);
string formatLadder(Stack<string> &ladder);


int main() {

    Lexicon dict("EnglishWords.dat");

    string start = "cat";
    string dest = "pen";


    string ladder = wordLadder(start, dest, dict);

    cout << ladder << endl;

    return 0;
}

string wordLadder(string start, string dest, Lexicon &dict) {


    Lexicon visitedWords;
    Queue<Stack<string> > queue;

    Stack<string> initialLadder;
    initialLadder.push(start);
    queue.enqueue(initialLadder);

    while(!queue.isEmpty()) {

        Stack<string> topLadder = queue.dequeue();

        if (topLadder.peek() == dest) {
            return formatLadder(topLadder);
        }

        for(int i = 0; i < topLadder.peek().size(); i++) {
            for(int j = 0; j < 26; j++) {

               string topWord = topLadder.peek();

               topWord[i] = ('a' + (topWord[i] - 'a' + j) % 26);

                if (!visitedWords.contains(topWord) && dict.contains(topWord)) {
                    Stack<string> newCopy = topLadder;
                    newCopy.push(topWord);
                    visitedWords.add(topWord);
                    queue.enqueue(newCopy);
                }
            }
        }
    }

    return "No match found!";
}


string formatLadder(Stack<string> &ladder) {

    return ladder.toString();
}
