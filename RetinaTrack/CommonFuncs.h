/*
 * CommonFuncs.h
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#ifndef COMMONFUNCS_H_
#define COMMONFUNCS_H_

#include <math.h>
#include <iostream>

struct hitConf {
	double u;
	double v;
};

struct pqPoint_i {
	int p;
	int q;
};

struct pqPoint {
	double p;
	double q;
	double w;
};

struct circlePoint {
	double a;
	double b;
	double R;
};

struct track {
	double pt;
	double phi;
};

inline double get_x_from_pol(double r, double phi) { return r*cos(phi);};
inline double get_y_from_pol(double r, double phi) { return r*sin(phi);};
inline double get_r_from_car(double x, double y) { return sqrt(x*x + y*y);};
inline double get_phi_from_car(double x, double y) { return acos(x / get_r_from_car(x, y));}; // { return atan(y/x);};

extern double pi;

#endif /* COMMONFUNCS_H_ */
