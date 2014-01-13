/*
 * HitsGenerator.cpp
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#include "HitsGenerator.h"

HitsGenerator::HitsGenerator() {
	setGeometry();
}

HitsGenerator::~HitsGenerator() {
}

void HitsGenerator::addHit(double x, double y) {
	hit hit_;
	hit_.x = x;
	hit_.y = y;
	hitCollection.push_back(hit_);
}

void HitsGenerator::setGeometry() {
	barrel_layer_r.push_back(23.0);
	barrel_layer_r.push_back(35.7);
	barrel_layer_r.push_back(50.8);
	barrel_layer_r.push_back(68.6);
	barrel_layer_r.push_back(88.8);
	barrel_layer_r.push_back(108.0);
}

void HitsGenerator::addCircle(double R_gen, double phi0_gen, bool random_leg) {
	TRandom3 rand;

	unsigned int  barrel_layer_size = barrel_layer_r.size();
	bool first = true;
	bool second = true;
	if (random_leg) {
		UInt_t rnd = rand.Integer(2);
		if (rnd == 0) first = false;
		else second = false;
	}
	for (unsigned int i = 0; i < barrel_layer_size; i++) {
		double spread = 0;
//		spread = rand.Gaus(0, 0.01);
		spread = 0;
		double r = barrel_layer_r[i];
		double ratio = barrel_layer_r[i] / (2*R_gen);
		if (ratio < -1. || ratio > 1.) continue;

		double phi;
		double x;
		double y;
		if (first) {
			phi = acos(ratio) + phi0_gen;
			while (phi > 2*pi) phi -= 2*pi;
			while (phi < 0.) phi += 2*pi;
			x = get_x_from_pol(r, phi);
			y = get_y_from_pol(r, phi);
			hit hit_i;
			hit_i.x = x + spread;
			hit_i.y = y;
			hitCollection.push_back(hit_i);
		}

		if (second) {
			phi = -1*acos(ratio) + phi0_gen;
			while (phi > 2*pi) phi -= 2*pi;
			while (phi < 0.) phi += 2*pi;
			x = get_x_from_pol(r, phi);
			y = get_y_from_pol(r, phi);
			hit hit_j;
			hit_j.x = x + spread;
			hit_j.y = y;
			hitCollection.push_back(hit_j);
		}
	}
//	return HitCollection.size();
}

unsigned int HitsGenerator::cleanHits(double ymin, double ymax, double xmin, double xmax) {
	vector <hit> HitCollectionNew;
	unsigned int hits_n = hitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		if (hitCollection[hit_i].y > ymin && hitCollection[hit_i].y <= ymax
				&& hitCollection[hit_i].x > xmin && hitCollection[hit_i].x <= xmax) {
			HitCollectionNew.push_back(hitCollection[hit_i]);
		}
	}
	hitCollection = HitCollectionNew;
	return hitCollection.size();
}
