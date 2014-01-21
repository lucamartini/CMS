/*
 * HitsGenerator.h
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#ifndef HITSGENERATOR_H_
#define HITSGENERATOR_H_


#include <iostream>

#include "TRandom3.h"

#include "CommonFuncs.h"

#include "LayerGeometry.h"
#include "HitCollection.h"

//using namespace std;

enum geometry {plain, cylinder};



class HitsGenerator {
public:
	HitsGenerator(LayerGeometry LG_, geometry geo_, string name_);
	virtual ~HitsGenerator();

	unsigned int cleanHitsXY(double ymin, double ymax, double xmin, double xmax);
	unsigned int cleanHitsRPhi(double rmin, double rmax, double phimin, double phimax);

	void addHit(double x, double y);
	void addHit(double x);
	void addCircle(double R_gen, double phi0_gen, bool random_leg = false);
	void addLine(double phi0_gen, double b = 0.);

	inline void print_hit(hit hit) { std::cout << "x = " << hit.x << "; y = " << hit.x << std::endl;}

	HitCollection getHitCollection() {return hitCollection;};

private:

	HitCollection hitCollection;
  LayerGeometry LG;
  geometry geo;
  string name;

};

#endif /* HITSGENERATOR_H_ */
