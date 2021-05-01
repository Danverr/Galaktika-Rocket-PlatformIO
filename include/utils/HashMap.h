#pragma once

#include <inttypes.h>
#include "Arduino.h"

template <class K, class V>
struct HashMapPair {
    K key;
    V value;
    HashMapPair<K, V>* nextPair;

    HashMapPair(K k, V v, HashMapPair<K, V>* next) {
        key = k;
        value = v;
        nextPair = next;
    }
};

template <class K, class V>
class HashMap {
private:
    HashMapPair<K, V>** hashArray;
    uint16_t (*calculateHashCode)(K);
    uint16_t count;
    V standart;
public:
    const uint16_t hashArraySize;

    HashMap(uint16_t (*calcHashCode)(K), uint16_t hashArraySize = 20, V standart = V());
    void insert(K key, V value);
    void remove(K key);
    V at(K key);
    bool find(K key);
    void clear();
    uint16_t size() { return count; }
    ~HashMap();
};

///////////////////////
//// Realisation
///////////////////////

template <class K, class V>
HashMap<K, V>::HashMap(uint16_t (*calcHashCode)(K), uint16_t hashArraySize, V standartValue)
    : hashArraySize(hashArraySize) {
    calculateHashCode = calcHashCode;
    standart = standartValue;
    count = 0;
    hashArray = new HashMapPair<K, V>* [hashArraySize];

    for (int i = 0; i < hashArraySize; ++i) {
        hashArray[i] = nullptr;
    }
}

template <class K, class V>
void HashMap<K, V>::insert(K key, V value) {
    if (!find(key)) {
        Serial.println("FOUND");
        uint16_t ind = calculateHashCode(key) % hashArraySize;
        HashMapPair<K, V>* pair = new HashMapPair<K, V>(key, value, hashArray[ind]);
        Serial.println("PAIR");
        hashArray[ind] = pair;
        Serial.println("HASHARR");
        count++;
    }
}

template <class K, class V>
void HashMap<K, V>::remove(K key) {
    uint16_t ind = calculateHashCode(key) % hashArraySize;
    HashMapPair<K, V>* pair = hashArray[ind];
    HashMapPair<K, V>* lastPair = nullptr;

    while (pair) {
        if (pair->key == key) {
            if (pair == hashArray[ind]) {
                hashArray[ind] = pair->nextPair;
            } else {
                lastPair->nextPair = pair->nextPair;
            }
            delete pair;
            count--;
            return;
        }
        lastPair = pair;
        pair = pair->nextPair;
    }
}

template <class K, class V>
V HashMap<K, V>::at(K key) {
    uint16_t ind = calculateHashCode(key) % hashArraySize;
    HashMapPair<K, V>* pair = hashArray[ind];
    while (pair) {
        if (pair->key == key) {
            return pair->value;
        }
        pair = pair->nextPair;
    }

    return standart;
}

template <class K, class V>
bool HashMap<K, V>::find(K key) {
    uint16_t ind = calculateHashCode(key) % hashArraySize;
    HashMapPair<K, V>* pair = hashArray[ind];

    while (pair) {
        if (pair->key == key) {
            return true;
        }
        pair = pair->nextPair;
    }
    return false;
}

template <class K, class V>
void HashMap<K, V>::clear() {
    for (uint16_t i = 0; i < count; ++i) {
        HashMapPair<K, V>* pair = hashArray[i];
        while (pair) {
            HashMapPair<K, V>* next = pair->nextPair;
            delete pair;
            pair = next;
        }
    }
    delete hashArray;
}

template <class K, class V>
HashMap<K, V>::~HashMap() {
    clear();
}