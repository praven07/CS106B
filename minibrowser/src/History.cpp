#include "History.h"
#include "error.h"
using namespace std;

History::History() {
    front = nullptr;
    curr = nullptr;
}

History::~History() {

    deleteNodes(front);
}

bool History::hasNext() const {

    return !isEmpty() && curr->next != nullptr;
}

bool History::hasPrevious() const {

    return !isEmpty() && curr->prev != nullptr;
}

string History::next() {

    if (!hasNext()) {
        error("There is no next page.");
    }

    curr = curr->next;
    return curr->title;
}

string History::previous() {

    if (!hasPrevious()) {
        error("There is no previous page.");
    }

    curr = curr->prev;
    return curr->title;
}

void History::goTo(const string& page) {


    Node* newNode = new Node();
    newNode->title = page;
    newNode->prev = nullptr;
    newNode->next = nullptr;

    if (hasNext()) {
        deleteNodes(curr->next);
    }


    if (isEmpty()) {
        front = newNode;
    } else {
        newNode->prev = curr;
        curr->next = newNode;
    }

    curr = newNode;
}

bool History::isEmpty() const {

    return curr == nullptr;
}

/**
 * Deletes the nodes starting from the given node to the end of the list.
 * @brief History::deleteNodes
 * @param node
 */
void History::deleteNodes(Node* &node) {

    if (node != nullptr) {

        deleteNodes(node->next);
        delete node;
        node = nullptr;
    }
}


