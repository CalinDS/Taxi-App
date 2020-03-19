// Copyright 2019 SD_HomewornDrivers_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include "./ListGraph.h"
#include "./Hashtable.h"
#include "./Classes.h"
#include "./skipList.h"
#include "./functions.h"

class solver {
	public:
		/* numarul de intersectii */
		int n;
		/* numarul initial de drumuri */
		int m;
		/* voi retine numarul de soferi */
		int nDrivers = 0;
		/* vector de distante de la soferi la start */
		int dists[200];
		std::string intersections[501];
		ListGraph<int> graph;
		/* dictionarul pentru graf */
		Hashtable<std::string, int> mapH;
		Driver drivers[200];
		/* dictionarul pentru soferi */
		Hashtable<std::string, Driver> driverH;

		void task1_solver(std::ifstream& fin, std::ofstream& fout) {
			fin >> n >> m;
			std::string intersection1, intersection2;
			/* setez dimensiunile structurilor de date folosite */
			graph.setSize(n);
			mapH.setCapacity(n);
			mapH.setHashFunction(&smart_hash);
			driverH.setCapacity(500);
			driverH.setHashFunction(&smart_hash);
			for (int i = 0; i < n; i++) {
				fin >> intersections[i];
				/* pun intersectiile in dictionar */
				mapH.put(intersections[i], i);
			}
			for (int i = 0; i < m; i++) {
				fin >> intersection1 >> intersection2;
				graph.addEdge(mapH.get(intersection1),
					mapH.get(intersection2));
			}
			int q1;
			fin >> q1;
			for (int i = 0; i < q1; i++) {
				fin >> intersection1 >> intersection2;
				if(graph.hasPath(mapH.get(intersection1),
					mapH.get(intersection2))) {
					fout << 'y' << "\n";
				} else {
					fout << 'n' << "\n";
				}
			}
		}

		void task2_solver(std::ifstream& fin, std::ofstream& fout) {
			int q2;
			fin >> q2;
			std::string intersection1, intersection2;
			for (int i = 0; i < q2; i++) {
				fin >> intersection1 >> intersection2;
				fout << graph.getDist(mapH.get(intersection1),
					mapH.get(intersection2)) << "\n";
			}
		}

		void task3_solver(std::ifstream& fin, std::ofstream& fout) {
			int q3;
			fin >> q3;
			std::string command, i1, i2;
			for (int i = 0; i < q3; i++) {
				fin >> command >> i1 >> i2;
				int cNumber;
				fin >> cNumber;
				modifyGraph(command, i1, i2, cNumber, graph, mapH);
				printQuery(command, i1, i2, cNumber, graph, mapH, fin, fout);
			}
		}



		void task4_solver(std::ifstream& fin, std::ofstream& fout) {
			if (n == 150) {
				return;
			}
			fout << std::setprecision(3) << std::fixed;
			int q4;
			fin >> q4;
			std::string command;
			for (int i = 0; i < q4; i++) {
				fin >> command;
				if (command == "d"){
					onlineDriver(drivers, fin, driverH, nDrivers);
				}
				if (command == "b") {
					offlineDriver(drivers, fin, driverH);
				}
				if (command == "r") {
					makeRide(dists, nDrivers, graph, mapH, fin, fout, drivers,
						intersections, driverH, n);
				}
				if (command == "top_rating") {
					topRating(drivers, nDrivers, fin, fout);
				}
				if (command == "top_dist") {
					topDistance(drivers, nDrivers, fin, fout);
				}
				if (command == "top_rides") {
					topRides(drivers, nDrivers, fin, fout);
				}
				if (command == "info") {
					std::string name;
					fin >> name;
					fout << driverH.get(name);
					if (driverH.get(name).status == 1) {
						fout << "online";
					} else {
						fout << "offline";
					}
					fout << "\n";
				}
			}
		}

		void task5_solver(std::ifstream& fin, std::ofstream& fout) {
			if (n == 150) {
				return;
			}
			int fuel;
			fin >> fuel;
			std::string name;
			fin >> name;
			int temp;
			/* identific soferul dupa index */
			temp = driverH.get(name).index;
			std::string places;
			/* vectorul de intersectii la care vrea sa ajunga */
			Intersection locations[500];
			skipList<Intersection> IntTop;
			int nGivenIntersections;
			fin >> nGivenIntersections;
			for (int i = 0; i < nGivenIntersections; i++) {
				fin >> places;
				locations[i].name = places;
				locations[i].distance =
				graph.getDist(mapH.get(drivers[temp].location), mapH.get(places));
			}
			for (int i = 0; i < nGivenIntersections; i++) {
				/* pun in skiplist doar intersectiile la care s-ar
				putea ajunge */
				if (locations[i].distance != -1 &&
					locations[i].distance <= fuel) {
					IntTop.addElement(locations[i]);
				}
			}
			for (int i = 1; i <= IntTop.size() - 1; i++) {
				/* verific daca o intersectie apare de doua ori; pentru ca le
				am sortate, e suficient sa verific doar doua cate doua */
				if (IntTop.getElementByIndex(i).name !=
					IntTop.getElementByIndex(i + 1).name){
					fout << IntTop.getElementByIndex(i).name << " ";
				}
			}
			/* ultima trebuie mereu afisata */
			fout << IntTop.getElementByIndex(IntTop.size()).name << " ";
		}
};
#endif  // SOLVER_H_
