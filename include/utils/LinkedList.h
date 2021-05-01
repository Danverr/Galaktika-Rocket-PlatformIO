#pragma once

#include <inttypes.h>

template<typename T>
struct LinkedListNode {
    T data;
    LinkedListNode<T>* next;

    LinkedListNode(T _data, LinkedListNode<T>* _next) : data(_data), next(_next) {}
};

template<typename T>
class LinkedList {
private:
    LinkedListNode<T>* first;
    LinkedListNode<T>* last;
    uint32_t count;
public:
    LinkedList();
    LinkedList(T[], uint32_t);
    void add(T value);
    void remove(uint32_t ind);
    void removeByValue(T value);  // Need tests
    void clear();
    T& at(uint32_t ind);  // reworked T -> T&
    bool find(T value);
    T* toArray();
    void forEach(void (*callback)(T& value));  // New
    void forEach(bool (*callback)(T& value));  // New
    LinkedListNode<T>* getFirst() { return first; }  // New
    uint32_t size() { return count; }
    ~LinkedList();
};

///////////////////////////
//// Realisation
///////////////////////////

template <typename T>
LinkedList<T>::LinkedList() {
    first = nullptr;
    last = nullptr;
    count = 0;
}

template <typename T>
LinkedList<T>::LinkedList(T arr[], uint32_t arrSize) {
    first = nullptr;
    last = nullptr;
    count = 0;
    for (int i = 0; i < arrSize; ++i) {
        add(arr[i]);
    }
}

template <typename T>
void LinkedList<T>::add(T value) {
    LinkedListNode<T>* node = new LinkedListNode<T>(value, nullptr);
    if (last) {
        last->next = node;
    } else {
        first = node;
    }
    last = node;
    count++;
}

template <typename T>
void LinkedList<T>::remove(uint32_t ind) {
    LinkedListNode<T>* node = first;
    LinkedListNode<T>* lastNode = nullptr;
    if (ind >= count) {
        return;
    }

    for (uint32_t i = 0; i < ind; ++i) {
        lastNode = node;
        node = node->next;
    }

    if (node == first) {
        first = node->next;
    } else {
        lastNode->next = node->next;
    }

    delete node;
    count--;
}

template <typename T>
void LinkedList<T>::removeByValue(T value) {
    LinkedListNode<T>* node = first;
    LinkedListNode<T>* lastNode = nullptr;

    for (uint32_t i = 0; i < count; ++i) {
        if (node->data == value) {
            if (node == first) {
                first = node->next;
            } else {
                lastNode->next = node->next;
            }

            delete node;
            count--;
        }

        lastNode = node;
        node = node->next;
    }
}

template <typename T>
void LinkedList<T>::clear() {
    count = 0;
    LinkedListNode<T>* node = first;
    LinkedListNode<T>* lastNode = nullptr;

    while (node) {
        lastNode = node;
        node = node->next;
        delete lastNode;
    }
}

template <typename T>
T& LinkedList<T>::at(uint32_t ind) {
    LinkedListNode<T>* node = first;

    for (uint32_t i = 0; i < ind; ++i) {
        node = node->next;
    }

    return node->data;
}

template <typename T>
bool LinkedList<T>::find(T value) {
    LinkedListNode<T>* node = first;

    while (node) {
        if (node->data == value) {
            return true;
        }
        node = node->next;
    }
    return false;
}

template <typename T>
T* LinkedList<T>::toArray() {
    LinkedListNode<T>* node = first;
    T* array = new T[count];

    for (uint32_t i = 0; i < count; ++i) {
        array[i] = node->data;
        node = node->next;
    }

    return array;
}

template <typename T>
void LinkedList<T>::forEach(void (*callback)(T& value)) {
    LinkedListNode<T>* node = first;

    while (node) {
        callback(node->data);
        node = node->next;
    }
}

template <typename T>
void LinkedList<T>::forEach(bool (*callback)(T& value)) {
    LinkedListNode<T>* node = first;

    while (node) {
        if (callback(node->data)) {
            break;
        }
        node = node->next;
    }
}

template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}
