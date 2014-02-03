/*
 * HitsGenerator.cpp
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#include "HitsGenerator.h"

HitsGenerator::HitsGenerator(LayerGeometry LG_, geometry geo_, string name_) :
	hitCollection(name_),
	LG(LG_),
	geo(geo_),
	name(name_) {
}

HitsGenerator::~HitsGenerator() {
}

void HitsGenerator::addHit(double x, double y) {
	hit hit_;
	hit_.x = x;
	hit_.y = y;
	hitCollection.addHit(hit_);
}


void HitsGenerator::addCircle(double R_gen, double phi0_gen, bool random_leg) {
	TRandom3 rand;

	unsigned int  barrel_layer_size = LG.get_layer_size();
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
		double x;
		double y;

		if (geo == plain) {

			double r = LG.get_barrel_layers_plain()[i].q;
			double a_gen = get_x_from_pol(R_gen, phi0_gen);
			double b_gen = get_y_from_pol(R_gen, phi0_gen);
			double sqr = a_gen*a_gen - r*r + 2*b_gen*r;
			double x0 = a_gen + sqrt(sqr);
			double x1 = a_gen + sqrt(sqr);
			if (x0 > 0 && x1 > x0) x = x0;
			else x = x1;
			y = r;
		}

		if (geo == cylinder) {
			double r = LG.get_barrel_layers_cylinder()[i].R;
			double ratio = r / (2*R_gen); //
			double phi;
			if (first) {
				if (ratio < -1. || ratio > 1.) continue;
				phi = acos(ratio) + phi0_gen;
				while (phi >= 2*pi) phi -= 2*pi;
				while (phi < 0.) phi += 2*pi;
				x = get_x_from_pol(r, phi);
				y = get_y_from_pol(r, phi);
			}
			if (second) {
				phi = -1*acos(ratio) + phi0_gen;
				while (phi >= 2*pi) phi -= 2*pi;
				while (phi < 0.) phi += 2*pi;
				x = get_x_from_pol(r, phi);
				y = get_y_from_pol(r, phi);
			}
		}

		hit hit_i;
		hit_i.x = x + spread;
		hit_i.y = y;
		hitCollection.addHit(hit_i);
//			print_hit(hit_i);
	}
//	return HitCollection.size();
}

void HitsGenerator::addLine(double phi, double b) {
	unsigned int  barrel_layer_size = LG.get_layer_size();
	for (unsigned int i = 0; i < barrel_layer_size; i++) {

		double x;
		double y;
		if (geo == cylinder) {
			double r = LG.get_barrel_layers_cylinder()[i].R;
//			double m = (r*sin(phi) - b)/(r*cos(phi));
			double sqr = r*r + tan(phi)*tan(phi)*r*r - b*b;
			x = (-tan(phi)*b + sqrt(sqr))*cos(phi)*cos(phi);
			y = tan(phi)*x + b;
		}
		if (geo == plain) {
			y = LG.get_barrel_layers_plain()[i].q;
			double m = tan(phi);
			x = (y-b)/m;
		}

		hit hit_i;
		hit_i.x = x;
		hit_i.y = y;
		hitCollection.addHit(hit_i);
	}
}

unsigned int HitsGenerator::cleanHitsXY(double ymin, double ymax, double xmin, double xmax) {
	HitCollection HitCollectionNew(name);
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	unsigned int hits_n = hitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		if (hitcollref->at(hit_i).y > ymin && hitcollref->at(hit_i).y <= ymax
				&& hitcollref->at(hit_i).x > xmin && hitcollref->at(hit_i).x <= xmax) {
			HitCollectionNew.addHit(hitcollref->at(hit_i));
		}
	}

	hitCollection = HitCollectionNew;
	return hitCollection.size();
}

unsigned int HitsGenerator::cleanHitsRPhi(double rmin, double rmax, double phimin, double phimax) {
	HitCollection HitCollectionNew(name);
	vector <hit> * hitcollref = hitCollection.getHitCollectionRef();
	unsigned int hits_n = hitCollection.size();
	for (unsigned int hit_i = 0; hit_i < hits_n; hit_i++) {
		double x = hitcollref->at(hit_i).x;
		double y = hitcollref->at(hit_i).y;
		double r = get_r_from_car(x, y);
//		double phi = get_phi_from_car(x, y);
		double phi = acos(x/r);
//		cout << x << " " << y << " " << r << " " << phi << endl;
		if (r >= rmin && r <= rmax
				&& phi >= phimin && phi <= phimax) {
			HitCollectionNew.addHit(hitcollref->at(hit_i));
		}
	}
	hitCollection = HitCollectionNew;
	return hitCollection.size();
}
