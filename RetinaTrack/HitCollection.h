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

	unsigned int size() {return hitCollection.size();};
	string getName() {return name;};
	void addHit(hit hit) {hitCollection.push_back(hit);};
	vector <hit> * getHitCollectionRef() {return &hitCollection;};

	void printHits();
	void drawHits(bool fit);

private:
	string name;
	vector <hit> hitCollection;
};

#endif /* HITCOLLECTION_H_ */
