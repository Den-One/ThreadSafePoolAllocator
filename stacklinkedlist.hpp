#pragma once

/**
 * @file stacklinkedlist.hpp
 * @brief Thread-safe pool allocator, supports C++17 standard and higher.
 */

 /// @namespace Pool Allocator namespace.
namespace pa {

    /**
     * @class StackLinkedList
     * @brief Saves the invariant of memory Block sequence.
     */
    class StackLinkedList final {
    public:
        /**
         * @class Node
         * @brief It's a simple node with a pointer of its another exemplar.
         */
        struct Node {
            Node* next = nullptr;
        };

    private:
        /// The start of the whole sequence.
        Node* _head = nullptr;

    public:
        /// @brief Initializes the head of the list like a 'nullptr'.
        StackLinkedList();

        /// @brief List can't be copy constructed.
        StackLinkedList(const StackLinkedList& stackLinkedList) = delete;

        /// @brief List can't be move constructed.
        StackLinkedList(StackLinkedList&& stackLinkedList) noexcept = delete;

        /// @brief List can't be copied.
        StackLinkedList* operator=(const StackLinkedList& rhs) = delete;
        
        /// @brief List can't be moved.
        StackLinkedList* operator=(StackLinkedList&& rhs) noexcept = delete;
        
        ~StackLinkedList();

        /**
         * @brief Change the head to the previously putted pointer.
         * @param newNode Takes a new address to make it the head of the list.
         */
        void push(Node* newNode);

        /**
         * @brief Change the head to the previously putted pointer.
         * @return Returns the top element of the stack.
         */
        Node* pop();
    };
} // ditail namespace