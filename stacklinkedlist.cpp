#include "stacklinkedlist.hpp"

pa::StackLinkedList::StackLinkedList() : _head{ nullptr } {}

pa::StackLinkedList::~StackLinkedList() {};

void pa::StackLinkedList::push(Node* newNode) {
    newNode->next = _head;
    _head = newNode;
}

pa::StackLinkedList::Node* pa::StackLinkedList::pop() {
    Node* top = _head;
    _head = _head->next;
    return top;
}