/* Copyright 2019 Calin Dragos 315 CA */

#ifndef FUNCTIONS_H__
#define FUNCTIONS_H__
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include "./ListGraph.h"
#include "./Hashtable.h"
#include "./Classes.h"
#include "./skipList.h"

/* functia cu care aflu distanta minima (diferita de -1) */
int getMin(int v[], int n) {
	int i = 0, min;
	while (v[i] < 0) {
		i++;
	}
	min = v[i];
	for (int j = i + 1; j < n; j++) {
		if (v[j] < min && v[j] >= 0) {
			min = v[j];
		}
	}
	return min;
}


void modifyGraph(std::string command, std::string i1, std::string i2,
 	int cNumber, ListGraph<int> &graph, Hashtable<std::string, int> &mapH) {
	if (command == "c" && cNumber == 0) {
		if (!graph.hasEdge(mapH.get(i1), mapH.get(i2)))
		graph.addEdge(mapH.get(i1), mapH.get(i2));
	}
	/* nu imi trebuie contor pentru ca if-urile sunt idependente */
	if (command == "c" && cNumber == 1) {
		if (graph.hasEdge(mapH.get(i2), mapH.get(i1)) &&
			!graph.hasEdge(mapH.get(i1), mapH.get(i2))) {
			graph.removeEdge(mapH.get(i2), mapH.get(i1));
		}
		if (graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			!graph.hasEdge(mapH.get(i2), mapH.get(i1))) {
			graph.removeEdge(mapH.get(i1), mapH.get(i2));
		}
		if (graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			graph.hasEdge(mapH.get(i2), mapH.get(i1))) {
			graph.removeEdge(mapH.get(i1), mapH.get(i2));
			graph.removeEdge(mapH.get(i2), mapH.get(i1));
		}
	}
	if (command == "c" && cNumber == 2) {
		if (graph.hasEdge(mapH.get(i2), mapH.get(i1)) &&
			!graph.hasEdge(mapH.get(i1), mapH.get(i2))) {
			graph.addEdge(mapH.get(i1), mapH.get(i2));
		}
		if (graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			!graph.hasEdge(mapH.get(i2), mapH.get(i1))) {
			graph.addEdge(mapH.get(i2), mapH.get(i1));
		}
		if (!graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			!graph.hasEdge(mapH.get(i2), mapH.get(i1))) {
			graph.addEdge(mapH.get(i2), mapH.get(i1));
			graph.addEdge(mapH.get(i1), mapH.get(i2));
		}
	}
	if (command == "c" && cNumber == 3) {
		/* imi trebuie un contor pentru ca altfel daca se intra in primul if
		se va intra sigur si in al doilea si nu vreau asta */
		int count = 0;
		if (graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			!graph.hasEdge(mapH.get(i2), mapH.get(i1))) {
			graph.addEdge(mapH.get(i2), mapH.get(i1));
			graph.removeEdge(mapH.get(i1), mapH.get(i2));
			count++;
		}
		if (!graph.hasEdge(mapH.get(i1), mapH.get(i2)) &&
			graph.hasEdge(mapH.get(i2), mapH.get(i1)) && count == 0) {
			graph.addEdge(mapH.get(i1), mapH.get(i2));
			graph.removeEdge(mapH.get(i2), mapH.get(i1));
		}
	}
}

void printQuery(std::string command, std::string i1, std::string i2,
 	int cNumber, ListGraph<int> &graph, Hashtable<std::string, int> &mapH,
 	std::ifstream& fin, std::ofstream& fout) {
	if (command == "q" && cNumber == 0) {
		if(graph.hasPath(mapH.get(i1), mapH.get(i2))) {
			fout << 'y' << "\n";
		} else {
			fout << 'n' << "\n";
		}
	}
	if (command == "q" && cNumber == 1) {
		fout << graph.getDist(mapH.get(i1), mapH.get(i2)) << "\n";
	}
	if (command == "q" && cNumber == 2) {
		std::string via;
		fin >> via;
		fout << graph.bigDist(mapH.get(i1), mapH.get(via), mapH.get(i2));
		fout << "\n";
	}
}

void onlineDriver(Driver drivers[], std::ifstream& fin,
	Hashtable<std::string, Driver> &driverH, int &nDrivers) {
	std::string name, place;
	fin >> name >> place;
	/* daca este un nume nou, bag soferul in dictionar */
	if (!driverH.has_key(name)) {
		drivers[nDrivers].name = name;
		drivers[nDrivers].location = place;
		drivers[nDrivers].status = 1;
		drivers[nDrivers].index = nDrivers;
		drivers[nDrivers].rating = 0;
		driverH.put(drivers[nDrivers].name, drivers[nDrivers]);
		nDrivers++;
	/* daca nu, doar actualizez pozitia */
	} else {
		int aux;
		aux = driverH.get(name).index;
		drivers[aux].status = 1;
		drivers[aux].location = place;
		driverH.put(drivers[aux].name, drivers[aux]);
	}
}

/* soferul devine offline */
void offlineDriver(Driver drivers[], std::ifstream& fin,
	Hashtable<std::string, Driver> &driverH) {
	std::string name;
	fin >> name;
	int aux;
	aux = driverH.get(name).index;
	drivers[aux].status = 0;
	driverH.put(drivers[aux].name, drivers[aux]);
}

void makeRide(int dists[], int &nDrivers, ListGraph<int> &graph,
	Hashtable<std::string, int> &mapH, std::ifstream& fin,
	std::ofstream& fout, Driver drivers[], std::string intersections[],
	Hashtable<std::string, Driver> &driverH, int n) {
	std::string start, end;
	fin >> start;
	fin >> end;
	float grade;
	fin >> grade;
	skipList<Driver> GetBestDriver;
	int check1 = 0;
	int check2 = 0;
	/* toate distantele de la soferi la start sunt -1 */
	for (int i = 0; i < 200; i++) {
		dists[i] = -1;
	}
	/* obtin noile distante (daca e cazul) */
	for (int i = 0; i < nDrivers; i++) {
		if (drivers[i].status == 1 &&
			graph.getDist(mapH.get(drivers[i].location), mapH.get(start)) != -1) {
			dists[i] = graph.getDist(mapH.get(drivers[i].location), mapH.get(start));
			check2++;
		}
	}
	if (check2 != 0) {
		/* daca exista soferi disponibil verific daca destinatia e disponibila,
		si daca nu e, caut printre vecini potentiale noi destinatii */
		if (graph.getDist(mapH.get(start), mapH.get(end)) != -1) {
			check1++;
		}
		if (graph.getDist(mapH.get(start), mapH.get(end)) == -1) {
			for (int i = 0; i < n; i++) {
				if (graph.getDist(mapH.get(end), i) == 1 &&
					graph.hasPath(mapH.get(start), i)) {
					check1++;
					end = intersections[i];
					break;
				}
			}
		}
		if (check1 == 0) {
			fout << "Destinatie inaccesibila" << "\n";
		} else {
			/* pun in skiplist soferii care sunt la distanta minima de start */
			for (int i = 0; i < nDrivers; i++) {
				if (drivers[i].status == 1 &&
					graph.getDist(mapH.get(drivers[i].location), mapH.get(start))
					== getMin(dists, nDrivers)) {
					GetBestDriver.addElement(drivers[i]);
				}
			}
			/* il identific pe soferul potrivit dupa index si modific
			statisticile */
			int temp;
			temp = GetBestDriver.getElementByIndex(1).index;
			drivers[temp].distance = drivers[temp].distance +
				graph.bigDist(mapH.get(drivers[temp].location),
					mapH.get(start), mapH.get(end));
			drivers[temp].location = end;
			drivers[temp].nRides = drivers[temp].nRides + 1;
			drivers[temp].rating = (drivers[temp].rating *
				(drivers[temp].nRides - 1) + grade) / drivers[temp].nRides;
			driverH.put(drivers[temp].name, drivers[temp]);
		}
	} else {
		fout << "Soferi indisponibili" << "\n";
	}
}

void topRating(Driver drivers[], int nDrivers,
	std::ifstream& fin, std::ofstream& fout) {
	int nForPrint;
	fin >> nForPrint;
	int nPrint;
	/* relatia de "<" de mai sus e portrivita si aici */
	skipList<Driver> RatingTop;
	/* stabilesc daca ii afisez pe toti sau doar nForPrint din ei */
	if (nForPrint > nDrivers) {
		nPrint = nDrivers;
	} else {
		nPrint = nForPrint;
	}
	for (int i = 0; i < nDrivers; i++) {
		RatingTop.addElement(drivers[i]);
	}
	for (int i = 1; i <= nPrint; i++) {
		fout << RatingTop.getElementByIndex(i).name << ":";
		fout << RatingTop.getElementByIndex(i).rating << " ";
	}
	fout << "\n";
}

void topDistance(Driver drivers[], int nDrivers,
	std::ifstream& fin, std::ofstream& fout) {
	int nForPrint;
	fin >> nForPrint;
	int nPrint;
	Driver2 drivers2[200];
	for (int i = 0; i < nDrivers; i++) {
		drivers2[i].name = drivers[i].name;
		drivers2[i].distance = drivers[i].distance;
	}
	skipList<Driver2> DistanceTop;
	if (nForPrint > nDrivers) {
		nPrint = nDrivers;
	} else {
		nPrint = nForPrint;
	}
	for (int i = 0; i < nDrivers; i++) {
		DistanceTop.addElement(drivers2[i]);
	}
	for (int i = 1; i <= nPrint; i++) {
		fout << DistanceTop.getElementByIndex(i).name << ":";
		fout << DistanceTop.getElementByIndex(i).distance << " ";
	}
	fout << "\n";
}

void topRides(Driver drivers[], int nDrivers,
	std::ifstream& fin, std::ofstream& fout) {
	int nForPrint;
	fin >> nForPrint;
	int nPrint;
	Driver3 drivers3[200];
	for (int i = 0; i < nDrivers; i++) {
		drivers3[i].name = drivers[i].name;
		drivers3[i].nRides = drivers[i].nRides;
	}
	skipList<Driver3> RidesTop;
	if (nForPrint > nDrivers) {
		nPrint = nDrivers;
	} else {
		nPrint = nForPrint;
	}
	for (int i = 0; i < nDrivers; i++) {
		RidesTop.addElement(drivers3[i]);
	}
	for (int i = 1; i <= nPrint; i++) {
		fout << RidesTop.getElementByIndex(i).name << ":";
		fout << RidesTop.getElementByIndex(i).nRides << " ";
	}
	fout << "\n";
}

#endif  //  FUNCTIONS_H__
