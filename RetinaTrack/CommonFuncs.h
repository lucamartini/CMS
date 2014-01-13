/*
 * CommonFuncs.h
 *
 *  Created on: 13/gen/2014
 *      Author: lucamartini
 */

#ifndef COMMONFUNCS_H_
#define COMMONFUNCS_H_

#include <math.h>

struct hit {
	double x;
	double y;
};

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
};

struct circlePoint {
	double a;
	double b;
	double R;
};

inline double get_x_from_pol(double r, double phi) { return r*cos(phi);};
inline double get_y_from_pol(double r, double phi) { return r*sin(phi);};
inline double get_phi_from_car(double x, double y) { return atan(y/x);};
inline double get_r_from_car(double x, double y) { return sqrt(x*x + y*y);};

extern double pi;

#endif /* COMMONFUNCS_H_ */
