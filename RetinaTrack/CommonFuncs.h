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

#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TPaveText.h"
#include "TEllipse.h"

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

extern void DrawCanvas(TH1D h);
extern void DrawCanvas2(TH2D h);




#endif /* COMMONFUNCS_H_ */
