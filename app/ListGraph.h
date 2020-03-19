/* Copyright 2019 Calin Dragos 315 CA */

#ifndef LISTGRAPH_H__
#define LISTGRAPH_H__

#include <vector>
#include <queue>
#include <string>

template <typename T> struct Node {
    std::vector<T> neighbors;
};

template <typename T> class ListGraph {
 private:
    std::vector<Node<T>> nodes;
    int size;

 public:
	int *distance;

    ~ListGraph() {
    	nodes.clear();
    	delete [] distance;
    }

    /* setter similar cu cel de la dictionar */
    void setSize(int size) {
        this->size = size;
        nodes.resize(size);
        distance = new int[size];
    }

    void addEdge(T src, T dst) {
        nodes[src].neighbors.push_back(dst);
    }

    void removeEdge(T src, T dst) {
        int position = 0;
        for (auto it = nodes[src].neighbors.begin();
            it != nodes[src].neighbors.end(); it++) {
        	if (*it == dst) {
        		break;
        	}
        	position++;
        }
        nodes[src].neighbors.erase(nodes[src].neighbors.begin() + position);
    }

    bool hasEdge(T src, T dst) {
        bool value = false;
        for (auto it = nodes[src].neighbors.begin();
            it != nodes[src].neighbors.end(); it++) {
        	if (*it == dst) {
        		value = true;
        		break;
        	}
        }
        return value;
    }

    /* parcurgere pe latime plecand de la nodul source, ne da distanta minima
    de la acest nod la orice alt nod din graf */
    void BFS(T source) {
    	std::queue<int> q;
    	q.push(source);
        /* intial consideram ca nu exista drum nicaieri, deci distanta -1 */
    	for (int i = 0; i < size; i++) {
    		distance[i] = -1;
    	}
    	distance[source] = 0;
    	while (!q.empty()) {
    		int node = q.front();
    		q.pop();
    		for (auto n = nodes[node].neighbors.begin();
                n != nodes[node].neighbors.end(); n++) {
    			if (distance[*n] == -1) {
    				q.push(*n);
    				distance[*n] = distance[node] + 1;
    			}
    		}
    	}
    }

    /* fac BFS de la nodul din care plec si iau din vectorul de distance
    valoarea corespunzatoare nodului cautat */
    int getDist(T src, T dst) {
        BFS(src);
        return distance[dst];
    }

    /* de mentionat ca hasPath(nod, nod) este true mereu, dar
    hasEdge(nod, nod) e false fara addEgde(nod, nod) */
    bool hasPath(T src, T dst) {
        BFS(src);
        if (distance[dst] != -1) {
            return true;
        } else {
            return false;
        }
    }

    /* pentru drumul minim cand se trece obligatoriu prin alta intersectie */
    int bigDist(T node1, T node2, T node3) {
        int d1, d2;
        BFS(node1);
        d1 = distance[node2];
        BFS(node2);
        d2 = distance[node3];
        if (d1 == -1 || d2 == -1) {
            return -1;
        } else {
            return (d1 + d2);
        }
    }
};

#endif  //  LISTGRAPH_H__
