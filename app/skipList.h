/* Copyright 2019 Calin Dragos 315 CA */

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#define MAX_LEVEL 6

unsigned int seed = time(NULL);

float frand() {
    return (float) rand_r(&seed) / RAND_MAX;
}

int generateRandomLevel() {
    const float P = 0.5;
    static bool first = true;
    if (first) {
        srand((unsigned)time(NULL));
        first = false;
    }
    int lvl = (int)(log(frand()) / log(1.-P));
    return lvl < MAX_LEVEL ? lvl : MAX_LEVEL;
}

template <typename T>
struct skipNode {
    T value;
    skipNode **forward;

    skipNode(int level, T value) {
        forward = new skipNode<T> * [level + 1];
        memset(forward, 0, sizeof(skipNode<T>*) * (level + 1));
        this->value = value;
    }

    ~skipNode() {
        delete [] forward;
    }
};

template <typename T>
class skipList {
 public:
    skipNode<T> *header;
    T value;
    int level;
    const float P = 0.5;
    void Display(std::ofstream&);
    skipList() {
        header = new skipNode<T>(MAX_LEVEL, value);
        level = 0;
    }

    ~skipList() {
        skipNode<T>* node = header;
        while (node->forward[0]) {
            skipNode<T>* aux = node;
            node = node->forward[0];
            delete aux;
        }
        delete node;
    }

    void addElement(T value) {
        skipNode<T> *current = header;
        skipNode<T> *update[MAX_LEVEL + 1];
        memset(update, 0, sizeof(skipNode<T>*) * (MAX_LEVEL + 1));
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        current = current->forward[0];
        if (current == NULL || !(current->value == value)) {
            int rLvl = generateRandomLevel();
            if (rLvl > level) {
                for (int i = level + 1; i <= rLvl; i++) {
                    update[i] = header;
                }
                level = rLvl;
            }
            current = new skipNode<T>(rLvl, value);
             for (int i = 0; i <= rLvl; i++) {
                current->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = current;
            }
        }
    }

    void removeElement(T value) {
        skipNode<T> *current = header;
        skipNode<T> *update[MAX_LEVEL + 1];
        memset (update, 0, sizeof(skipNode<T>*) * (MAX_LEVEL + 1));
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] != NULL &&
                current->forward[i]->value < value) {
                current = current->forward[i];
            }
            update[i] = current;
        }
        current = current->forward[0];
        if (current->value == value) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) {
                    break;
                }
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (level > 0 && header->forward[level] == NULL) {
                level--;
            }
        }
    }

    T getElementByIndex(int index) {
        skipNode<T> *current = header;
        for (int i = 0; i < index; i++) {
            current = current->forward[0];
        }
        return current->value;
    }

    int size() {
        skipNode<T> *current = header;
        int nElems = 0;
        while (current->forward[0] != NULL) {
            current = current->forward[0];
            nElems++;
        }
        return nElems;
    }
};

#endif  //  SKIPLIST_H_

