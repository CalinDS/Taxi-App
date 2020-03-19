/* Copyright 2019 Calin Dragos 315 CA */

#ifndef HASHTABLE_H__
#define HASHTABLE_H__

#include <iostream>
#include <list>
#include <string>

/* diferite functii de hash, am ramas pana la urma doar la ultima */
int int_hash(int number) {
    return number % 7;
}

int char_hash(std::string str) {
    return str.length() + 8497;
}

int smart_hash(std::string str) {
    int hash = 5381;
    int c;
    int l = str.length();
    for (int i = 0; i < l; i++) {
    c = str.at(i);
    hash = ((hash << 5) + hash) + c;
    }
    return abs(hash);
}

/* fiecare componenta e definita de cheie si valoare */
template <typename Tkey, typename Tvalue> struct info {
    Tkey key;
    Tvalue value;
};

template <typename Tkey, typename Tvalue> class Hashtable {
 private:
    std::list<struct info<Tkey, Tvalue>> *H;
    int size;
    int capacity;
    int (*hash)(Tkey);

 public:
    ~Hashtable() {
        delete[] H;
    }

    /* practic am spart constructorul in 2 functii de set pentru ca nu pot
    initializa in clasa solver */
    void setCapacity(int capacity) {
        this->capacity = capacity;
        H = new std::list<struct info<Tkey, Tvalue>>[capacity]();
    }

    void setHashFunction(int (*h)(Tkey)) {
        this->hash = h;
    }

    /*functia de adaugare a valorii si de asociere cu cheia */
    void put(Tkey key, Tvalue value) {
        bool check = false;
        int index = hash(key) % capacity;
        for (auto it = H[index].begin(); it != H[index].end(); it++) {
            if (it -> key == key){
                it->value = value;
                check = true;
            }
        }
        if (!check){
            size++;
            struct info<Tkey, Tvalue> newEntry;
            newEntry.key = key;
            newEntry.value = value;
            H[index].push_back(newEntry);
        }
    }

    /* returneaza valoarea data de o anumita cheie */
    Tvalue get(Tkey key) {
        int index = hash(key) % capacity;
        for (auto it = H[index].begin(); it != H[index].end(); it++){
            if (it->key == key) {
                return it->value;
            }
        }
        /* in cazul in care nu exista cheia respectiva, returnez ceva,
        functia nefiind void. */
        return  H[index].begin()->value;
    }

    bool has_key(Tkey key) {
        int index = hash(key) % capacity;
        for (auto it = H[index].begin(); it != H[index].end(); it++){
            if (it->key == key) {
                return true;
            }
        }
        return false;
    }
};

#endif  //  HASHTABLE_H__
