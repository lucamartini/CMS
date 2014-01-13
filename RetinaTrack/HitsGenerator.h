/*
 * HitsGenerator.h
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#ifndef HITSGENERATOR_H_
#define HITSGENERATOR_H_

#include <vector>
#include <iostream>

#include "TRandom3.h"

#include "CommonFuncs.h"


using namespace std;

class HitsGenerator {
public:
	HitsGenerator();
	virtual ~HitsGenerator();

//	unsigned int setHits();
	unsigned int cleanHits(double ymin, double ymax, double xmin, double xmax);

	void addHit(double x, double y);
	void addCircle(double R_gen, double phi0_gen, bool random_leg = false);

	vector <hit> getHitCollection() {return hitCollection;};

private:
	void setGeometry();
	vector < double > barrel_layer_r;
  vector <hit> hitCollection;
};

#endif /* HITSGENERATOR_H_ */
