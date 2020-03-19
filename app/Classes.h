/* Copyright 2019 Calin Dragos 315 CA */

#ifndef CLASSES_H_
#define CLASSES_H_

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>

class Driver {
 public:
	std::string name;
	std::string location;
	float rating;
	int nRides;
	int distance;
	int status;
	int index;

	Driver() {}

	Driver(std::string name, std::string location, float rating, int nRides,
		int distance, int status, int index) {
		this->name = name;
		this->location = location;
		this->rating = rating;
		this->nRides = nRides;
		this->distance = distance;
		this->status = status;
		this->index = index;
	}

	~Driver() {}

	Driver(const Driver& other) {
		this->name = other.name;
		this->location = other.location;
		this->rating = other.rating;
		this->nRides = other.nRides;
		this->distance = other.distance;
		this->status = other.status;
		this->index = other.index;
	}

	void operator=(const Driver& other) {
		this->name = other.name;
		this->location = other.location;
		this->rating = other.rating;
		this->nRides = other.nRides;
		this->distance = other.distance;
		this->status = other.status;
		this->index = other.index;
	}

	/* iau cu ! pentru ca ma intereseaza ca skiplist-ul sa ordoneze
	descrescator */
	bool operator<(const Driver& other) {
		return !(this->rating < other.rating ||
			(this->rating == other.rating && this->name > other.name));
	}

	bool operator==(const Driver& other) {
		return (this->name == other.name);
	}

    friend std::ostream& operator<< (std::ostream &outfile,
    	const Driver& drive);
};

std::ostream& operator<< (std::ostream &outfile, const Driver& drive) {
    outfile << drive.name << ": " << drive.location << " " <<
    	drive.rating << " " << drive.nRides << " " << drive.distance
    	<< " ";
    return outfile;
}

/* necesar pentru top_dist */
class Driver2 {
 public:
	std::string name;
	int distance;

	Driver2() {}

	Driver2(std::string name, int distance) {
		this->name = name;
		this->distance = distance;
	}

	~Driver2() {}

	Driver2(const Driver2& other) {
		this->name = other.name;
		this->distance = other.distance;
	}

	void operator=(const Driver2& other) {
		this->name = other.name;
		this->distance = other.distance;
	}

	bool operator<(const Driver2& other) {
		return !(this->distance < other.distance ||
			(this->distance == other.distance && this->name > other.name));
	}

	bool operator==(const Driver2& other) {
		return (this->name == other.name);
	}
};


/* necesar pentru top_rides */
class Driver3 {
 public:
	std::string name;
	int nRides;

	Driver3() {}

	Driver3(std::string name, int nRides) {
		this->name = name;
		this->nRides = nRides;
	}

	~Driver3() {}

	Driver3(const Driver3& other) {
		this->name = other.name;
		this->nRides = other.nRides;
	}

	void operator=(const Driver3& other) {
		this->name = other.name;
		this->nRides = other.nRides;
	}

	bool operator<(const Driver3& other) {
		return !(this->nRides < other.nRides ||
			(this->nRides == other.nRides && this->name > other.name));
	}

	bool operator==(const Driver3& other) {
		return (this->name == other.name);
	}
};

/* pentru ordonarea de la task5 */
class Intersection {
 public:
	std::string name;
	int distance;

	/* nu mai trebuie sa iau negat, fiindca cea mai mica distanta inseamna
	cel mai mult combustibil ramas */
	bool operator<(const Intersection& other) {
		return (this->distance < other.distance ||
			(this->distance == other.distance && this->name < other.name));
	}

	bool operator==(const Intersection& other) {
		return (this->name == other.name);
	}
};

#endif  //  CLASSES_H_
