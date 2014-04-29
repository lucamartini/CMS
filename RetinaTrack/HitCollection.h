/*
 * HitCollection.h
 *
 *  Created on: 21/gen/2014
 *      Author: lucamartini
 */

#ifndef HITCOLLECTION_H_
#define HITCOLLECTION_H_

#include <vector>
#include <string>
#include <iostream>

#include "TGraph.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TPaveStats.h"

#include "CommonFuncs.h"

using namespace std;

struct hit {
	double x;
	double y;
};

class HitCollection {
public:
	HitCollection(string name_);
	virtual ~HitCollection();
	HitCollection( const HitCollection& other );

	string getName() {return name;};
	void setName(string name_) {name = name_;};
	unsigned int size() {return hitCollection.size();};
	double highestX();
	double highestY();
	double lowestX();
	double lowestY();
	unsigned int getClosestHit();
	unsigned int getFarthestHit();
	hit * getHit(unsigned int i) {return &hitCollection.at(i);};
	void addHit(hit hit) {hitCollection.push_back(hit);};
	void printHits();

	vector <hit> * getHitCollectionRef() {return &hitCollection;};
	pqPoint drawHits(bool fit, bool draw = true, bool rz = false);

private:
	string name;
	vector <hit> hitCollection;
};

#endif /* HITCOLLECTION_H_ */
