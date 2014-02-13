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
#include "TH1D.h"
#include "TH1I.h"
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

struct track {
	double pt;
	double phi;
};

inline double get_x_from_pol(double r, double phi) { return r*cos(phi);};
inline double get_y_from_pol(double r, double phi) { return r*sin(phi);};
inline double get_r_from_car(double x, double y) { return sqrt(x*x + y*y);};
inline double get_phi_from_car(double x, double y) { return atan2(y,x);}; // { return acos(x / get_r_from_car(x, y));}; // { return atan(y/x);};

extern double pi;

template <typename T> void DrawCanvas(T h) {
	TCanvas c("c", "c", 600, 600);
	h.SetFillColor(kBlue);
	h.SetFillStyle(3001);
	h.Draw();
	c.Print(Form("figs/%s.pdf", h.GetName()));
}

extern void DrawCanvas2(TH2D h);

#endif /* COMMONFUNCS_H_ */
