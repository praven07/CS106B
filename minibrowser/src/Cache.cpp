#define CacheCPP_Included
#include "Cache.h"
using namespace std;

template <typename ValueType>
Cache<ValueType>::Cache(int maxSize) {

    this->maxSize = maxSize;
    size = 0;
    head = nullptr;
    tail = nullptr;
}

template <typename ValueType>
Cache<ValueType>::~Cache() {
    deleteNodes(head);
}

template <typename ValueType>
void Cache<ValueType>::put(const std::string& key, const ValueType& value) {

    if (size == maxSize) {
        evict();
    }

    Node* newNode = new Node();
    newNode->key = key;
    newNode->value = value;

    if (isEmpty()) {
        newNode->prev = nullptr;
        newNode->next = nullptr;
        head = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
    }

    tail = newNode;
    pointers.add(key, newNode);
    size++;
    print(head);
}

template <typename ValueType>
bool Cache<ValueType>::containsKey(const std::string& key) const {

    return pointers.containsKey(key);
}

template <typename ValueType>
ValueType Cache<ValueType>::get(const std::string& key) {

    if (!containsKey(key)) {
        error("The provided key is not valid.");
    }

    Node* node = pointers.get(key);
    ValueType value = node->value;

    moveToEnd(node);

    print(head);

    return value;
}

template <typename ValueType>
void Cache<ValueType>::moveToEnd(Node* &node) {

    if (node->prev != nullptr && node->next != nullptr) {
        Node* leftNode = node->prev;
        Node* rightNode = node->next;

        leftNode->next = rightNode;
        rightNode->prev = leftNode;

        node->prev = tail;
        node->next = nullptr;
        tail->next = node;
    } else if (node == head) {
        head = node->next;
        head->prev = nullptr;
        tail->next = node;
        node->prev = tail;
        node->next = nullptr;
    }

    tail = node;
}

template <typename ValueType>
void Cache<ValueType>::evict() {

    Node* trash = head;
    head = head->next;
    head->prev = nullptr;

    pointers.remove(trash->key);
    delete trash;
    size--;
}

template <typename ValueType>
void Cache<ValueType>::deleteNodes(Node* &node) {

    if (node != nullptr) {
        deleteNodes(node->next);

        delete node;
    }
}

template <typename ValueType>
bool Cache<ValueType>::isEmpty() {

    return size == 0;
}

template <typename ValueType>
void Cache<ValueType>::print(Node* &node) {

    if (node == nullptr) {
        cout << endl;
    } else {
        cout << " -> " << node->key;
        print(node->next);
    }
}
